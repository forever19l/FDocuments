--- Time utils
-- @module utils.time_utils
-- @alias time_utils_module
local zpoller = require "lzmq.poller"
local ztimer = require "lzmq.timer"

local time_utils_module = {}
-- function time_utils.delay(sequence)
-- function time_utils.get_unix_time_ms()
-- function time_utils.get_time_delta_ms(start_time)
-- function time_utils.get_time_string_ms()

local poller = zpoller.new(1)

--- Delay (not a busywait)
-- @param ms
-- @return status (false if there was an error)
function time_utils_module.delay(ms)
	local result, err = poller:poll(ms)
	return err == nil
end

--- Get unix time in MS
-- @return time in ms
function time_utils_module.get_unix_time_ms()
	return ztimer.absolute_time()
end

--- Get difference between a unix timestamp and the current time
-- @param start_time unix timestamp(in ms)
-- @return time difference in ms
function time_utils_module.get_time_delta_ms(start_time)
	return ztimer.absolute_elapsed(start_time)
end

--- Get a formatted time string for the current time.
-- Format will be used across all log files and print outputs. It is currently YEAR-MONTH-DAY 24HOUR:MIN:SEC.MS
-- @return formatted time string
-- @return unix time in ms
function time_utils_module.get_time_string_ms()
	local t = time_utils_module.get_unix_time_ms()
	local t_ms = t - 1000 * math.floor(t/1000)
	return os.date("%Y-%m-%d_%H-%M-%S-", os.time())..string.format("%03d",t_ms)
end

return time_utils_module