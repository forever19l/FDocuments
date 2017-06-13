

#ifndef Foundation_Exception_INCLUDED
#define Foundation_Exception_INCLUDED


#include <stdexcept>
#include <string>

class Exception: public std::exception
	/// This is the base class for all exceptions defined
	/// in the Poco class library.
{
public:
	Exception(const std::string& msg, int code = 0);
		/// Creates an exception.

	Exception(const std::string& msg, const std::string& arg, int code = 0);
		/// Creates an exception.

	Exception(const std::string& msg, const Exception& nested, int code = 0);
		/// Creates an exception and stores a clone
		/// of the nested exception.

	Exception(const Exception& exc);
		/// Copy constructor.
		
	~Exception() throw();
		/// Destroys the exception and deletes the nested exception.

	Exception& operator = (const Exception& exc);
		/// Assignment operator.

	virtual const char* name() const throw();
		/// Returns a static string describing the exception.
		
	virtual const char* className() const throw();
		/// Returns the name of the exception class.
		
	virtual const char* what() const throw();
		/// Returns a static string describing the exception.
		///
		/// Same as name(), but for compatibility with std::exception.
		
	const Exception* nested() const;
		/// Returns a pointer to the nested exception, or
		/// null if no nested exception exists.
			
	const std::string& message() const;
		/// Returns the message text.
			
	int code() const;
		/// Returns the exception code if defined.
		
	std::string displayText() const;
		/// Returns a string consisting of the
		/// message name and the message text.

	virtual Exception* clone() const;
		/// Creates an exact copy of the exception.
		///
		/// The copy can later be thrown again by
		/// invoking rethrow() on it.
		
	virtual void rethrow() const;
		/// (Re)Throws the exception.
		///
		/// This is useful for temporarily storing a
		/// copy of an exception (see clone()), then
		/// throwing it again.

protected:
	Exception(int code = 0);
		/// Standard constructor.

	void message(const std::string& msg);
		/// Sets the message for the exception.

	void extendedMessage(const std::string& arg);
		/// Sets the extended message for the exception.
		
private:
	std::string _msg;
	Exception*  _pNested;
	int			_code;
};


//
// inlines
//
inline const Exception* Exception::nested() const
{
	return _pNested;
}


inline const std::string& Exception::message() const
{
	return _msg;
}


inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}


inline int Exception::code() const
{
	return _code;
}


//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define POCO_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
	class CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		Exception* clone() const;												\
		void rethrow() const;														\
	};

#define POCO_DECLARE_EXCEPTION(CLS, BASE) \
	POCO_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

#define POCO_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}


//
// Standard exception classes
//
POCO_DECLARE_EXCEPTION(LogicException, Exception)
POCO_DECLARE_EXCEPTION(AssertionViolationException, LogicException)
POCO_DECLARE_EXCEPTION(NullPointerException, LogicException)
POCO_DECLARE_EXCEPTION(NullValueException, LogicException)
POCO_DECLARE_EXCEPTION(BugcheckException, LogicException)
POCO_DECLARE_EXCEPTION(InvalidArgumentException, LogicException)
POCO_DECLARE_EXCEPTION(NotImplementedException, LogicException)
POCO_DECLARE_EXCEPTION(RangeException, LogicException)
POCO_DECLARE_EXCEPTION(IllegalStateException, LogicException)
POCO_DECLARE_EXCEPTION(InvalidAccessException, LogicException)
POCO_DECLARE_EXCEPTION(SignalException, LogicException)
POCO_DECLARE_EXCEPTION(UnhandledException, LogicException)

POCO_DECLARE_EXCEPTION(RuntimeException, Exception)
POCO_DECLARE_EXCEPTION(NotFoundException, RuntimeException)
POCO_DECLARE_EXCEPTION(ExistsException, RuntimeException)
POCO_DECLARE_EXCEPTION(TimeoutException, RuntimeException)
POCO_DECLARE_EXCEPTION(SystemException, RuntimeException)
POCO_DECLARE_EXCEPTION(RegularExpressionException, RuntimeException)
POCO_DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
POCO_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)
POCO_DECLARE_EXCEPTION(NoThreadAvailableException, RuntimeException)
POCO_DECLARE_EXCEPTION(PropertyNotSupportedException, RuntimeException)
POCO_DECLARE_EXCEPTION(PoolOverflowException, RuntimeException)
POCO_DECLARE_EXCEPTION(NoPermissionException, RuntimeException)
POCO_DECLARE_EXCEPTION(OutOfMemoryException, RuntimeException)
POCO_DECLARE_EXCEPTION(DataException, RuntimeException)

POCO_DECLARE_EXCEPTION(DataFormatException, DataException)
POCO_DECLARE_EXCEPTION(SyntaxException, DataException)
POCO_DECLARE_EXCEPTION(CircularReferenceException, DataException)
POCO_DECLARE_EXCEPTION(PathSyntaxException, SyntaxException)
POCO_DECLARE_EXCEPTION(IOException, RuntimeException)
POCO_DECLARE_EXCEPTION(ProtocolException, IOException)
POCO_DECLARE_EXCEPTION(FileException, IOException)
POCO_DECLARE_EXCEPTION(FileExistsException, FileException)
POCO_DECLARE_EXCEPTION(FileNotFoundException, FileException)
POCO_DECLARE_EXCEPTION(PathNotFoundException, FileException)
POCO_DECLARE_EXCEPTION(FileReadOnlyException, FileException)
POCO_DECLARE_EXCEPTION(FileAccessDeniedException, FileException)
POCO_DECLARE_EXCEPTION(CreateFileException, FileException)
POCO_DECLARE_EXCEPTION(OpenFileException, FileException)
POCO_DECLARE_EXCEPTION(WriteFileException, FileException)
POCO_DECLARE_EXCEPTION(ReadFileException, FileException)
POCO_DECLARE_EXCEPTION(DirectoryNotEmptyException, FileException)
POCO_DECLARE_EXCEPTION(UnknownURISchemeException, RuntimeException)
POCO_DECLARE_EXCEPTION(TooManyURIRedirectsException, RuntimeException)
POCO_DECLARE_EXCEPTION(URISyntaxException, SyntaxException)

POCO_DECLARE_EXCEPTION(ApplicationException, Exception)
POCO_DECLARE_EXCEPTION(BadCastException, RuntimeException)


#endif // Foundation_Exception_INCLUDED
