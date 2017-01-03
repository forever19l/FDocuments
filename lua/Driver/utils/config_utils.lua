--- Global data dictionary utilities
-- @module utils.config_utils
-- @alias config_utils
local time_utils = require("utils.time_utils")
local lapp = require("pl.lapp")
local json = require("dkjson")
local config_utils = {}
local loggers = {}

--- Get a "[ADDRESS]:[PORT]" string for a given service from a config file.
-- Assumes that the address variable's name is [name]_ADDRESS and that the port is [name]_BASE_PORT (+ uut #).
-- @param CONFIG config module
-- @param name name of the service address to get
-- @param uut UUT #
-- @return address string
-- @usage sequencer_address = config_utils.get_addr(CONFIG, "SEQUENCE", CONFIG.ID)
function config_utils.get_addr(CONFIG, name, uut)
    return CONFIG[name .. "_ADDRESS"] .. ":" .. tostring(CONFIG[name .. "_BASE_PORT"] + tonumber(uut))
end

function config_utils.get_svr_addr(CONFIG, name, uut)
    return CONFIG["SEQUENCE_ADDRESS"] .. ":" .. tostring(CONFIG[name .. "_BASE_PORT"] + tonumber(uut))
end
--- Get the a logger with file output defined by the external config file ([name]_LOG_FILENAME).
-- Logger must implement a "write" function. The current function will automatically append a 
-- timestamp and print to console as well.
--
-- Example: "2015-10-15 17:01:14.862:	:: iBoot for n65, Copyright 2007-2015, Apple Inc."
-- @param CONFIG config module
-- @param log_type name of logger to create. Each logger will only be created once, and the same logger will be returned on another function call.
-- @param log_file_prefix prefix to append to the log filename in teh config file. Examples could include a serial name or a directory.
-- @treturn log_writer logger object
-- @usage DUTLogger = config_utils.get_log_writer(CONFIG, "DUT", "FNXXfake-serial-number4N7C")
-- @see log_writer
function config_utils.get_log_writer(CONFIG, log_type, log_file_prefix)
	if log_file_prefix == nil then
		log_file_prefix = ""
	end

	-- Only create one logger per file
	if loggers[log_type] ~= nil then
		return loggers[log_type]
	end

	local filename = CONFIG[log_type.."_LOG_FILENAME"]
	if filename == nil then return nil end
	local filename = log_file_prefix .. filename

	local file = io.open(filename, "w")
	if file == nil then error("Could not open file! Make sure you have the right permissions.") end

	file:setvbuf("line") -- Flus data to file after every line

	-- FIXME: Do I want to put this into /another/ seperate utils file?

	--- Log Writer
	-- @table log_writer
	-- @tfield function write write(text to print, optional prefix for console output, should we skip newline at beginning)
	-- @tfield function flush force the file to flush
	-- @tfield function close force close the file
	log_writer = {
		write = function (text, prefix, skip_start_newline)
					if prefix == nil then prefix = log_type..":\t" end
					local time_string = time_utils.get_time_string_ms()
					
					text = text:gsub("\r\n","\n"):gsub("\r","\n")
					local output_string = ""

					-- Iterate over all the lines to print.
					for line in text:gmatch("[^\n]*") do
						if line ~= "" then
							if skip_start_newline then
								output_string = tostring(line)
								skip_start_newline = nil
							else
								output_string = output_string .. "\n" .. time_string .. ":\t" .. tostring(line)
							end
							-- print(prefix .. time_string .. ":\t" .. tostring(line))
						end
					end

					-- file:write(output_string)
				end,

		flush = function ()
					file:flush()
				end,

		close = function ()
					file:close()
				end
	}

	loggers[log_type] = log_writer
	return log_writer
end

--- Given command line arguments, import the correct config file and add any additional parameters.
-- @param cmd_args processed command line arguments table 
-- @tparam string cmd_args.config config module to import
-- @tparam number cmd_args.uut UUT test slot number
-- @return CONFIG populated config table
-- @usage CONFIG = config_utils.get_config(arg, "config.pathfinder")
function config_utils.get_config(cmd_args)
	local config_file = "socket_zmq.json"
	local CONFIG = require(config_file)
	CONFIG.ID = cmd_args.uut

	-- populate optional variables
	for _, opt in pairs(cmd_args.updates) do
		local key, val
		key, val = opt:match("(.*)=(.*)")

		-- Empty vals are a nil assignment.
		if val == "" then val = nil end

		-- Empty keys should be ignored (this is a bad command line)
		if key and key ~= "" then
			CONFIG[key] = val
		elseif opt ~= "" then
			lapp.error("Unable to process CONFIG update \""..tostring(opt).."\"")
		end
	end

	-- give config data to caller
	return CONFIG
end

function config_utils.get_zmq_port(port_name)
	local file = "/Users/Ryan/testerconfig/zmqports.json"
	local f = io.open(file,"r");
	local buf = f:read("*all");
	local cap = string.format("\"%s\":%%s*(%%d+)",port_name);
	print("cap : ",cap);
	local port = tostring(buf):match(cap)
	f:close();
	return port;
end

return config_utils