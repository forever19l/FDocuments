local modname=...;
local M={};
_G[modname]=M;
package.loaded[modname]=M;


function FCT_log(fmt,...)
    Log("[Tester] : "..tostring(fmt),ID);
end
function M.ClearBuffer(szDeviceName)
    return SIMPLECOM:ClearBuffer(szDeviceName)
end


function M.SetDetectString(DeviceName,par)
    return SIMPLECOM:SetDetectString(DeviceName,tostring(par))
end

function M.WaitForString(DeviceName,timeout)
    return SIMPLECOM:WaitForString(DeviceName,timeout)
end

function M.WriteString(DeviceName,par)
    return SIMPLECOM:WriteString(DeviceName,par);
end

function M.ReadString(DeviceName)
    return SIMPLECOM:ReadString(DeviceName);
end

function M.SendReceive(DeviceName,cmd,timeout)
	M.WriteString(DeviceName,cmd);
	M.WaitForString(DeviceName,timeout);
	return M.ReadString(DeviceName);
end
