-- package.cpath = package.path..";".."C:\\Project\\CameraTest2\\Debug\\?.dll"
-- require("lzmq.timer")
math.randomseed(os.time())
math.random(1,1000)

require "CameraTest2"

a = 1
b = 2
i = 0
while true do
    print(i.." >> "..tostring(GetCaptureStatus()).."\n")
    -- Delay(10)
    print("index = "..i.." ; result = "..test(a,b).."\n")
    a = math.random(1,999)
    b = math.random(1,999)
    i = i + 1
end
