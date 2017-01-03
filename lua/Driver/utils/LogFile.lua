local _log = {}
local time_utils = require("utils.time_utils")

_log.file = {}
_log.path = {}
_log.mode  ={}

function _log.SetMode(logname, mode)
	_log.mode[logname] = mode
end

function _log.GetMode(logname)
	return _log.mode[logname]
end

function _log.CreateDataLogDir(folder)
	print("\t< Create Log Path > :",folder)
	print(os.execute("mkdir "..folder));
end

function _log.getKey(logname)
	return string.upper(logname).."_LOG_FILENAME"
end

function _log.getPath(logname)
	local key  = _log.getKey(logname)
	print(key)
	return _log.path[key]
end

function _log.open(dir, logname, type)--dir: folder, logname: filename, type:csv or txt...
	if logtype==nil then
		logtype="txt" 
	end
	local p = JoinPath(dir, logname.."."..type)

	local mode = _log.GetMode(logname)
	if mode == nil then
		mode = "w"
	end
	local file = io.open(p,mode)
	if(file == nil) then
		_log.CreateDataLogDir(dir)
		file = io.open(p,mode)
	end
	if(file) then
		local key = _log.getKey(logname)
		_log.file[string.upper(logname)] = file
		_log.path[key] = p
	end
end

function _log.write(logname, conten, timestamp)
	local file = _log.file[string.upper(logname)]
	if file then
		if(timestamp == true) then
			conten = time_utils.get_time_string_ms().."\t"..tostring(conten) .."\n"
		end
		file:write(conten)
		file:flush()
	end
end

function _log.close(logname)
	local file = _log.file[string.upper(logname)]
	if(file and file ~="file (close)") then
		file:close()
		_log.file[string.upper(logname)] = nil
	end
end

return _log
