-- require "zhelpers"
local zmq = require "lzmq"

print("Connecting to hello world server ...")
local context = zmq.context()
local requester, err = context:socket{zmq.REQ, 
  connect = "tcp://127.0.0.1:8100"
}
zmq.assert(requester, err)

requester:send("test\0")
local buffer = requester:recv()
-- for request_nbr = 0, 9 do
--   print ("Sending Hello " .. request_nbr .. "...\0")
--   requester:send("Hello")
--   local buffer = requester:recv()
--   print("Received World " .. request_nbr)
-- end
