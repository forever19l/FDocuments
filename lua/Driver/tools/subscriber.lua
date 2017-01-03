#!/usr/bin/env lua


local json = require("dkjson")
local zmq = require("lzmq")
local zassert = zmq.assert
local zpoller = require("lzmq.poller")
local zthreads = require("lzmq.threads")

require("pathManager")
local DriverDir = JoinPath(JoinPath(CurrentDir(), "TestEngine"), "Driver")
package.path = package.path .. ";" .. JoinPath(DriverDir, "?.lua")

require("utils.zhelpers")
local config_utils = require("utils.config_utils")
local time_utils = require("utils.time_utils")
local lapp = require("pl.lapp")
local poller = zpoller.new(2)


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

local ctx = zmq.context()

local subscriber, err = ctx:socket{zmq.SUB,
  subscribe = "";
  connect   = address;
}
zassert(subscriber, err)

print("[Subscriber]: ","Create subscriber with address : ",address);

function OnSubsriber(socket)
  local buf= socket:recv()
--  print(time_utils.get_time_string_ms(),"[Receive]:",buf)
--  io.write(buf)
-- io.write(buf) 
	print(buf)
  
end


poller:add(subscriber, zmq.POLLIN, function()
	OnSubsriber(subscriber)
end)

poller:start()