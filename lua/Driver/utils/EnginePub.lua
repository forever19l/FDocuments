local _Engine_Pipe = {}

_Engine_Pipe.pipe = nil

local time_utils = require("utils.time_utils")

function _Engine_Pipe.send(msg,timeflag)
	if _Engine_Pipe.pipe then
		msg = tostring(msg)
		if timeflag==true then
			msg = time_utils.get_time_string_ms().."\t"..msg
		end
		_Engine_Pipe.pipe:send(msg.."\n\0")
	end
end

return _Engine_Pipe
