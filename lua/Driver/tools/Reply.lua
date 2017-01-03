#!/usr/bin/env lua
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

ctx_reply = zmq.context()
--zthreads.set_context(context)

--Create zmq connect to engine.
local reply_ip = address

reply_zmq, err = ctx_reply:socket(zmq.REP, {bind = reply_ip})
zassert(reply_zmq, err)
print(time_utils.get_time_string_ms(),"[REPLY] : Create reply server:",reply_ip);

local poller = zpoller.new(2);

while true do
	local msg = reply_zmq:recv();
	print(time_utils.get_time_string_ms(),"[RECV] :",msg);
	zassert(reply_zmq:send("OK"));
end


local function OnRequest()
print("asfasdfasd")
	local msg = reply_zmq:recv();
	print(time_utils.get_time_string_ms(),"[REPLY] :",msg);
	zassert(reply_zmq:send("OK"));
end

poller:add(reply_zmq, zmq.POLLIN, OnRequest);

poller:start();