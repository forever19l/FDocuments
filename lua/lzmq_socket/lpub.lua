local zmq = require "lzmq"
local zmqt = require("lzmq.timer")
require "zhelpers"

local context = zmq.context()
local REQ, err = context:socket{zmq.REQ, bind = "tcp://*:6800"}
local REP, err = context:socket{zmq.REP, bind = "tcp://*:7000"}
zassert(REQ, err)
zassert(REP, err)

sleep(1)
print("(!!!!!!!!!)")
-- zassert(publisher:send("plc_rotate_ready_/Users/mac/Downloads/Left_JoyStickCircle__2016-10-09_00-03-08.csv\0"))
zassert(REQ:send("testing\r\n"))
-- zassert(publisher:send("plc_rotate_ready_/Users/mac/Desktop/BU2/Oculus_p1_p2/2016-6-30_250_Pcs_Raw_Data/11_WMTCL22Q5005D9__2016-06-30-16-22-21/Left_JoyStick_Circle_WMTCL22Q5005D9_2016-06-30\ 04\:21\:56.csv\r\n"))
local x = zassert(REQ:recv())
print("REQ recv :"..x)
-- local t = 1
-- while 1 == t do
--   local buffer = zassert(responder:recv())
--   print("Received " .. tostring(buffer))
--   sleep (1) -- Do some 'work'
--   zassert(responder:send("World"))
--   if #buffer > 1 then
--       t = 0
--   end
-- end