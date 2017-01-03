require "pathManager"
package.cpath = package.cpath..";"..CurrentDir().."/lib/?.dylib"

require "libGlobal"

local msg = {}
msg["start"] = "intelligentgroup.uut.starttest"
msg["end"] = "intelligentgroup.uut.endtest"
msg["restart"] = "intelligentgroup.restart"
msg["terminal"] = "intelligentgroup.terminal"

print("\n\n")
while true do
	io.write("lock >>")
	local key = io.read()
	if key == "quit" then
		return 0
	end
	LockInstrument(key)
	io.write("unlock >>")
	io.read()
	UnlockInstrument(key)
end
