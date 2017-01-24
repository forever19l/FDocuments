
local g_TableFunction={};

local function ListTbaleFunction(buffer)
	for p,v in string.gmatch(buffer,"function%s*M%.((.-)%(.-%))") do
		--print(v,p)
		local f={};
		f.name=v;
		f.proto=p;
		g_TableFunction[#g_TableFunction+1]=f;
	end
end


function Start(filepath)
	local f = io.open(filepath);
	local buffer = f:read("*all");
	f:close();
	ListTbaleFunction(buffer);
	return g_TableFunction;
end

do return end;

local filepath = "/Ryan/code/EditCode/Control1/TestManager/FCT/j72_FCT/script/J72_FCT.lua"
Start(filepath);
--[[
print("List function : ");
for i,v in ipairs(g_TableFunction) do
	print(v.name,v.proto);
end
print("List function finish!");
--]]