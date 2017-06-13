#include "FileUtil.h"
#include "Util.h"
#include "Exception.h"
#include "Buffer.h"

#include <fstream>
#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <objbase.h>
#include <io.h>
#else
#include <pthread.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#endif

FileUtil::FileUtil()
{
}

FileUtil::FileUtil(const std::string& filePath)
{
	SetPath(filePath);
}

FileUtil::~FileUtil()
{
}

void FileUtil::SetPath(const std::string& filePath)
{
	_path = filePath;
	std::string::size_type n = _path.size();
	if (n > 1 && (_path[n - 1] == '\\' || _path[n - 1] == '/') && !((n == 3 && _path[1] == ':')))
	{
		_path.resize(n - 1);
	}
	_upath = Util::StringToWString(_path);
}

bool FileUtil::IsExist()
{
#ifdef WIN32
	DWORD attr = GetFileAttributesW(_upath.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_NOT_READY:
		case ERROR_INVALID_DRIVE:
			return false;
		default:
			HandleLastError(_path);
		}
	}
	return true;
#else
	struct stat st;
	return stat(_path.c_str(), &st) == 0;
#endif
}

bool FileUtil::Create()
{
#ifdef WIN32
	HANDLE hFile = CreateFileW(_upath.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	else if (GetLastError() == ERROR_FILE_EXISTS)
		return false;
	else
		HandleLastError(_path);

	return false;
#else
	int n = open(_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		HandleLastError(_path);
	return false;
#endif
}

bool FileUtil::GetAllData(char*& buffer, int& len)
{
	int size = GetFileSize();
	FILE* fp = fopen(_path.c_str(), "rb");
	if (fp == NULL){
		printf("Open file Error.\n");
		return false;
	}

	len = size;
	buffer = new char[len];

	size_t rsize = fread(buffer, 1, len, fp);
	if (rsize != len){
		if (ferror(fp)) {
			printf("Error form reading file.\n");
			delete buffer;
			buffer = NULL;
			len = -1;
			fclose(fp);
			return false;
		}
	}
	fclose(fp);
	return true;
}

bool FileUtil::GetAllData(std::string& str)
{
	char* buff = NULL;
	int fileLen = 0;
	bool flag = GetAllData(buff, fileLen);
	if (!flag){
		return false;
	}

	str.assign(buff, fileLen);
	delete buff;
	buff = NULL;
	fileLen = 0;
	return true;
}

int FileUtil::GetFileSize()
{
#ifdef WIN32
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (GetFileAttributesExW(_upath.c_str(), GetFileExInfoStandard, &fad) == 0)
		HandleLastError(_path);
	LARGE_INTEGER li;
	li.LowPart = fad.nFileSizeLow;
	li.HighPart = fad.nFileSizeHigh;
	return li.QuadPart;
#else
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return st.st_size;
	else
		HandleLastError(_path);
	return 0;
#endif
}

bool FileUtil::Rename(const std::string& newFileName)
{
	std::string::size_type pos = _path.find_last_of("\\");
	if (pos == std::string::npos){
		pos = _path.find_last_of("/");
		if (pos == std::string::npos){
			return false;
		}
	}

	std::string path = _path.substr(0, pos);
	path += "/" + newFileName;

#ifdef WIN32
	std::wstring upath = Util::StringToWString(path);
	if (MoveFileExW(_upath.c_str(), upath.c_str(), MOVEFILE_REPLACE_EXISTING) == 0)
		HandleLastError(_path);
	SetPath(path);
	return true;
#else
	if (rename(_path.c_str(), path.c_str()) != 0)
		HandleLastError(_path);
	SetPath(path);
	return true;
#endif
}

bool FileUtil::MoveTo(const std::string& newPath)
{
	CopyTo(newPath);
	Delete();
	SetPath(newPath);
	return true;
}

bool FileUtil::CopyTo(const std::string& newPath)
{
#ifdef WIN32
	std::wstring upath = Util::StringToWString(newPath);
	if (CopyFileW(_upath.c_str(), upath.c_str(), FALSE) == 0)
		HandleLastError(_path);
	return true;
#else
	int sd = open(_path.c_str(), O_RDONLY);
	if (sd == -1) 
		HandleLastError(_path);

	struct stat st;
	if (fstat(sd, &st) != 0)
	{
		close(sd);
		HandleLastError(_path);
	}
	const long blockSize = st.st_blksize;

	int dd = open(newPath.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (dd == -1)
	{
		close(sd);
		HandleLastError(newPath);
	}
	Buffer<char> buffer(blockSize);
	try
	{
		int n;
		while ((n = read(sd, buffer.begin(), blockSize)) > 0)
		{
			if (write(dd, buffer.begin(), n) != n)
				HandleLastError(newPath);
		}
		if (n < 0)
			HandleLastError(_path);
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0)
	{
		close(dd);
		HandleLastError(newPath);
	}
	if (close(dd) != 0)
		HandleLastError(newPath);
	return true;
#endif
	
}

bool FileUtil::Delete()
{
#ifdef WIN32
	if (DeleteFileW(_upath.c_str()) == 0)
		HandleLastError(_path);
	return true;
#else
	int rc = unlink(_path.c_str());
	if (rc) 
		HandleLastError(_path);
	return true;
#endif
	return true;
}


void FileUtil::HandleLastError(const std::string& path)
{
#ifdef WIN32
	DWORD err = GetLastError();
	switch (err)
	{
	case ERROR_FILE_NOT_FOUND:
		throw FileNotFoundException(path, err);
	case ERROR_PATH_NOT_FOUND:
	case ERROR_BAD_NETPATH:
	case ERROR_CANT_RESOLVE_FILENAME:
	case ERROR_INVALID_DRIVE:
		throw PathNotFoundException(path, err);
	case ERROR_ACCESS_DENIED:
		throw FileAccessDeniedException(path, err);
	case ERROR_ALREADY_EXISTS:
	case ERROR_FILE_EXISTS:
		throw FileExistsException(path, err);
	case ERROR_INVALID_NAME:
	case ERROR_DIRECTORY:
	case ERROR_FILENAME_EXCED_RANGE:
	case ERROR_BAD_PATHNAME:
		throw PathSyntaxException(path, err);
	case ERROR_FILE_READ_ONLY:
		throw FileReadOnlyException(path, err);
	case ERROR_CANNOT_MAKE:
		throw CreateFileException(path, err);
	case ERROR_DIR_NOT_EMPTY:
		throw DirectoryNotEmptyException(path, err);
	case ERROR_WRITE_FAULT:
		throw WriteFileException(path, err);
	case ERROR_READ_FAULT:
		throw ReadFileException(path, err);
	case ERROR_SHARING_VIOLATION:
		throw FileException("sharing violation", path, err);
	case ERROR_LOCK_VIOLATION:
		throw FileException("lock violation", path, err);
	case ERROR_HANDLE_EOF:
		throw ReadFileException("EOF reached", path, err);
	case ERROR_HANDLE_DISK_FULL:
	case ERROR_DISK_FULL:
		throw WriteFileException("disk is full", path, err);
	case ERROR_NEGATIVE_SEEK:
		throw FileException("negative seek", path, err);
	default:
		throw FileException(path, err);
	}
#else
	switch (errno)
	{
	case EIO:
		throw IOException(path, errno);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, errno);
	case EACCES:
		throw FileAccessDeniedException(path, errno);
	case ENOENT:
		throw FileNotFoundException(path, errno);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, errno);
	case EISDIR:
		throw OpenFileException("not a file", path, errno);
	case EROFS:
		throw FileReadOnlyException(path, errno);
	case EEXIST:
		throw FileExistsException(path, errno);
	case ENOSPC:
		throw FileException("no space left on device", path, errno);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, errno);
	case ENOTEMPTY:
		throw DirectoryNotEmptyException(path, errno);
	case ENAMETOOLONG:
		throw PathSyntaxException(path, errno);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, errno);
	default:
		throw FileException(std::strerror(errno), path, errno);
	}
#endif
}
