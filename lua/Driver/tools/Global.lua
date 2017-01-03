require "lfs"
package.cpath = package.cpath..";"..CurrentDir().."/lib/?.dylib"

require "libGlobal"

local msg = {}
msg["start"] = "intelligentgroup.uut.starttest"
msg["end"] = "intelligentgroup.uut.endtest"
msg["restart"] = "intelligentgroup.restart"
msg["terminal"] = "intelligentgroup.terminal"

print("\n\n")
while true do
	io.write("Notification Name >>")
	local key = io.read()
	if key == "quit" then
		return 0
	end
	io.write("Notification Slot >>")
	local slot = io.read()
	postDistributedNotification(msg[key], slot)
end
