require "zhelpers"
local zmq = require "lzmq"

print("Connecting to hello world server ...")
local context = zmq.context()
local requester, err = context:socket{zmq.REQ, 
  connect = "tcp://127.0.0.1:6800"
}
zassert(requester, err)

  -- requester:send("forevertest\0")
  -- print("req send")
  -- local buffer = requester:recv()
  -- print("Received World " .. tostring(buffer))
for request_nbr = 0, 9 do
  print ("Sending Hello " .. request_nbr .. "...")
  requester:send("forevertest")
  local buffer = requester:recv()
  print("Received World " .. buffer)
end
