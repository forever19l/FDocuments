--- Sequence utils
-- @module utils.sequence_utils
-- @alias sequence_utils
local string_utils = require("utils.string_utils")

--===========================--
-- Unit exponents
--===========================--
local unit_exponents = {
	NM 		= -9,
	UM 		= -6,
	MM 		= -3,
	CM 		= -2,
	DM 		= -1,
	M 		= 0,
	UV 		= -6,
	MV 		= -3,
	V 		= 0,
	UA 		= -6,
	MA 		= -3,
	A 		= 0,
	UW 		= -6,
	MW 		= -3,
	W 		= 0,
	HZ 		= 0,
	KHZ 	= 3,
	MHZ 	= 6,
	GHZ 	= 9,
	MOHM 	= -3,
	OHM 	= 0,
	KOHM 	= 3,
	S 		= 0,
	MS		= -3,
	VPP		= 0,
	MVPP	= -3,
	PECT    = -2,
	BASE    = 0,
}

--===========================--
-- Exported module
--===========================--
local sequence_utils = {}
-- function sequence_utils.check_numerical_limits(value, sequence)
-- function sequence_utils.convert_units(value, from_units, to_units)

--- Compare a value against numerical limits in a sequence object
-- @param result result value from test
-- @param sequence FCT test sequence table
-- @return pass/fail boolean
function sequence_utils.check_numerical_limits(result, sequence)
	do return true end
	local passing = true

    if (tonumber(sequence.low) and tonumber(result) < tonumber(sequence.low)) or
            (tonumber(sequence.high) and tonumber(result) > tonumber(sequence.high)) then
        passing = false
    end

    return passing
end

--- Match expected text (from sequence object) against diags command output
-- @param result response string from diags command
-- @param sequence FCT test sequence table
-- @return pass/fail boolean
function sequence_utils.check_string_match_limit(result, sequence)
	local passing = true

    if sequence.param2 and sequence.param2 ~= "" and string.match(result, string_utils.replace_special_char(sequence.param2)) == nil then
        passing = false
    end

    return passing
end

--- Compare parsed diags parameter against string limits in a sequence object
-- @param result parsed diags parameter
-- @param sequence FCT test sequence table
-- @return pass/fail boolean
function sequence_utils.check_string_low_high_limit(result, sequence)
	do return true end
	local passing = true

    if sequence.low ~= nil and sequence.low ~= "" and string.match(result, string_utils.replace_special_char(sequence.low)) == nil then
        passing = false
    elseif sequence.high ~= nil and sequence.high ~= "" and string.match(result, string_utils.replace_special_char(sequence.high)) == nil then
        passing = false
    end

    return passing
end

--- Convert value from one unit to another. 
-- Can be used to guarantee consistent units for a specific function.
-- Does not guarantee that volts are converted to volts, seconds to seconds, etc.
-- @param value number to convert
-- @param from_units units to convert from (e.g. mV)
-- @param to_units units to convert to (e.g. V)
-- @return number converted from from_units -> to_units
function sequence_utils.convert_units(value, from_units, to_units)
	if from_units == nil or to_units == nil then 
		error("Units are not specified but are required.", 2)
	end
	
	local from_u = unit_exponents[string.upper(from_units)]
	local to_u = unit_exponents[string.upper(to_units)]

	if from_u == nil or to_u == nil then
		error("Invalid units!", 2)
	end

	local delta_exponent = from_u - to_u
	return tonumber(value) * math.pow(10, delta_exponent)
end

return sequence_utils
