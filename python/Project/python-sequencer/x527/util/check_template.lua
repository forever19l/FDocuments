--arg1 is the log file for diags output, arg2 is the template_file, key is the thing to parse for with {{}}

local _PDCA_KEY_MATCH_ = "{{(.-)}}"

function replace_special_char(txt)
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

local function addTimeStampMask(str)
  local t = {}
  local x = 0;
  local mask = "([0-9A-F]+@[RT]%d%s+)"
  for v in string.gmatch(str..'\n', "(.-)[\r\n]+") do
    -- print(#v,v)
    if(x ==0 and #v==0) then 
      t[#t+1] = v--the first line, if value is empty, do not add time stamp
    else
      t[#t+1] = mask .. v
    end
    x=x+1
  end
  if string.match(string.sub(str,-1),"[\r\n]") then
      t[#t+1]=''
    end
  return table.concat(t,"[\r\n]+")
end

--- Parse diags template file and populate the match table with patterns
-- @param diags_command Diags command parsing template that should be processed
-- @return TemplatePatternTable reference
local function parse_template_file(temp_file)--par = file Name
  local fileName = temp_file
  local file = io.open(temp_file)
  local patternTable = {}
  if(file) then
    local keyTable = {}
    local str = file:read("*a")
    file:close()

    for key in string.gmatch(str, _PDCA_KEY_MATCH_) do
      table.insert(keyTable, key)
    end


    local strMatch = ""
    local sig = 1
    for i=1, #keyTable do
      st, ed = string.find(str, "{{"..keyTable[i].."}}", sig)
      txt = string.sub(str, sig, st-1)
      txt = replace_special_char(txt)

      local endStr = string.sub(str, ed+1, ed+1)
      endStr = replace_special_char(endStr)
      local endStr2 = string.sub(str, ed+2, ed+2)

      if(endStr2 ~= "{") then
        endStr2 = replace_special_char(endStr2)
      else
        endStr2 = ""
      end

      matchTmp = strMatch .. txt .. "(.-)" .. endStr .. endStr2--string.sub(str, ed+1, ed+1)
      strMatch = strMatch .. txt .. ".-" .. endStr--string.sub(str, ed+1, ed+1)
      --matchTmp = addTimeStampMask(matchTmp)
      patternTable[keyTable[i]] = matchTmp--patternTable={"ABC"="xxxx(.-)",...}, "ABC", there is no "{{}}""
      sig = ed + 2
      -- print(keyTable[i], matchTmp)
    end


  else
    error("ERRCODE[-5]Failed to open template file: "..fileName)
  end

  return patternTable
end
	
function load_diags_response(diags_log)
     local f = io.open(diags_log)
     local response = f:read('*a')
     f:close()
     return response
end
	  
function check_temp(diags_log, template_file, key)
      local key_name = string.match(key, _PDCA_KEY_MATCH_)
      local pattern_table = parse_template_file(template_file)

      print('pattern to match for '..key_name..':')
      print('///////////////////////////////////////////')
      print(pattern_table[key_name])
      print('///////////////////////////////////////////')
      print('')
      print('diags output:')
      print('===========================================')
      diags_response = load_diags_response(diags_log)
      print(diags_response)
      print('===========================================')
      print('')

      local value = string.match(diags_response, pattern_table[key_name])
      if value==nil then
         print('    could not match')
      else
	 print('    MATCHED:'..value)
      end
--read diags log
--read template_file
--parse
 
end

diags_log = select(1, ...)
template_file = select(2, ...)
key = select(3, ...)

check_temp(diags_log, template_file, key)