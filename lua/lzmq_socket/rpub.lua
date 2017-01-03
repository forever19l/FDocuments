--[[
1、loadcell及plcdata是由测试平台连接还是自动化平台。
    1、1、自动化平台的话，数据传输问题（plcdata，3维力+1时间。打包数据还是一组一组的传输。loadcell？）
    1、2、测试平台自己连接获取数据，自动化平台需要发送当前状态、动作。（修改量小，但是plcdata需要通讯协议）

1、数据传输，
2、机台状态error code，由自动化平台控制还是由测试平台控制。
3、测试结果及测试项。
]]

--一直发，由SUB做选择，过滤。如果没有SUB连接则drop发送的消息
local zmq = require "lzmq"
local zmqt = require("lzmq.timer")
require "zhelpers"

local context = zmq.context()
local publisher, err = context:socket{zmq.PUB, bind = "tcp://*:6800"}
-- local responder, err = context:socket{zmq.REP, bind = "tcp://*:7000"}
zassert(publisher, err)
-- zassert(responder, err)

sleep(1)
print("(!!!!!!!!!)")
-- zassert(publisher:sendr("plc_rotate_ready_/Users/mac/Downloads/Left_JoyStickCircle__2016-10-09_00-03-08.csv\0"))
zassert(publisher:send("mypubtest\0"))
-- zassert(publisher:send("plc_rotate_ready_/Users/mac/Desktop/BU2/Oculus_p1_p2/2016-6-30_250_Pcs_Raw_Data/11_WMTCL22Q5005D9__2016-06-30-16-22-21/Left_JoyStick_Circle_WMTCL22Q5005D9_2016-06-30\ 04\:21\:56.csv\r\n"))
-- local t = 1
-- while 1 == t do
--     local buffer = zassert(responder:recv())
--     print("Received " .. buffer)
--     sleep (1) -- Do some 'work'
--     zassert(responder:send("World"))
--     if #buffer > 1 then
--         break
--     end
-- end