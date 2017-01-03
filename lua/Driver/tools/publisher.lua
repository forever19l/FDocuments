#!/usr/bin/env lua
--package.path = package.path..";".."./Driver/?.lua"
require("pathManager")
local DriverDir = JoinPath(CurrentDir(),"Driver")
package.path = package.path..";"..JoinPath(DriverDir,"?.lua")


require("utils.zhelpers")
local json = require("dkjson")
local zmq = require("lzmq")
local zassert = zmq.assert
local zpoller = require("lzmq.poller")
local zthreads = require("lzmq.threads")
local config_utils = require("utils.config_utils")
local time_utils = require("utils.time_utils")
local lapp = require("pl.lapp")


local args = lapp [[
X527 Test Engine

This is the interface between the test sequencer and the test fixture HW and DUT.
It does all of the required limits checking and conditional execution. Options (such
as IP address, ports, log file names, etc.) are set in config files. These files
are, by default, stored in the config/ directory.
    
    -a,--address    (default "tcp://127.0.0.1:6666")   Config module to load.
    -u,--uut        (default 1)                     UUT slot number (used for IP/Port selection)
    <updates...>    (default "")                    Series of X=Y pairs to update the CONFIG table with
]]

local address = args.address;

local ctx_pub = zmq.context();
local pub_zmq,err = ctx_pub:socket(zmq.PUB,{bind = address})
zassert(pub_zmq,err);
print("Create publisher server: ",address);

local function PublisherMsg(msg)
	assert(pub_zmq:send(msg))
	print("[Sequencer]: Send Message:",msg);
end

while true do
	io.write("Publisher>");
	local cmd = io.read("*line")
	if (cmd and #cmd>0) then
		--local ret,err=pub_zmq:send(cmd.."\r\n")
		--local ret,err=pub_zmq:sendx("101",cmd.."\r\n")
		local ret,err=pub_zmq:send("101",zmq.SNDMORE)
		local ret,err=pub_zmq:send(cmd.."\r\n")
		if (ret) then
			print(time_utils.get_time_string_ms(),"[SEND]:",cmd);
		else
			print("[ERROR]:",err)
		end
	end
end