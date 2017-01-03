--- String utils
-- @module utils.string_utils
-- @alias string_utils
local string_utils = {}
-- function string_utils.replace_special_char(txt)

--===========================--
-- Public API functions
--===========================--
--- Replace all special characters with their escaped versions
-- @param txt input text
-- @return text with special characters escaped
function string_utils.replace_special_char(txt)
	txt = tostring(txt)
	txt = string.gsub(txt, "%(", "%%(")
	txt = string.gsub(txt, "%)", "%%)")
	txt = string.gsub(txt, "%-", "%%-")
	txt = string.gsub(txt, "%+", "%%+")
	txt = string.gsub(txt, "%.", "%%.")
	txt = string.gsub(txt, "%[", "%%[")
	txt = string.gsub(txt, "%]", "%%]")
	return txt;
end

--- Replace percent signs with their escaped version
-- @param txt input text
-- @return text with special characters escaped
function string_utils.escape_percent(txt)
	txt = tostring(txt)
	return string.gsub(txt, "%%", "%%%%")
end

return string_utils