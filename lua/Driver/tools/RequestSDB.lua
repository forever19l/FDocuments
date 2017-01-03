#!/usr/bin/env lua
package.path = package.path..";".."./Driver/?.lua"

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

local context = zmq.context();	
local request_zmq, err = context:socket(zmq.REQ, {connect = address})
zassert(request_zmq, err)
	
print(time_utils.get_time_string_ms(),"[Sequencer] : Create REQ,connnect to :",address);
	
	function DoStep(item)
		local ret,err= sequence_zmq:send(item);
		assert(ret,err);
		local msg = sequence_zmq:recv();	--return
	end
while true do
	io.write("REQUEST>");
	local cmd = io.read("*line")
	if (cmd and #cmd>0) then 
		t={id=123,jsonrpc="1.0"};
		t["function"]="step"
		t["params"]="/Users/Ryan/Project/x527_fct/test_plan_debug__01.csv"
		print(json.encode(t))
--		str = "{\"function\": \"load\", \"params\": [\"/Users/Ryan/Project/x527_fct/test_plan_debug__01.csv\"], \"jsonrpc\": \"1.0\", \"id\": \"454e46239ea111e5bf813c15c2be22ec\"}"--json.encode(t);
--		str = [[{"function": "load\", "params": [\"/Users/Ryan/Project/x527_fct/test_plan_debug__01.csv\"], \"jsonrpc\": \"1.0\", \"id\": \"454e46239ea111e5bf813c15c2be22ec\"}]]--json.encode(t);
--		str = [[{"jsonrpc" : "1.0","id" : "846930886","function" : "load","params" : ["/Users/Ryan/Project/x527_fct/test_plan_debug__01.csv"]}]]
		str = [[
		{'FUNCTION': 'calculate', 'GROUP': 'PMU-TEMP', 'DESCRIPTION': '1.23121E+11', 'VAL': 'unsupported_for_p1', 'LOW': '-5', 'HIGH': '5', 'TIMEOUT': '', 'KEY': '{{comment}}', 'TID': 'PMU_TEMP_370_CALC_PMU_TEMP_DELTA_BUBO', 'VALIDATE': '', 'BLOCK': '123132231', 'UNIT': 'C', 'PARAM2': '{{pmu_temp_delta_bubo}}'}
		]]
		
		str = [==[
		{"FUNCTION": "calculate", "GROUP": "CAL", "DESCRIPTION": "Convert V to mV", "VAL": "", "LOW": "1760", "HIGH": "1840", "TIMEOUT": "", "KEY": "", "PARAM1": "smokeyshell .e "print.[[halt_ldo1_cal_value]]*1000."", "TID": "CAL_LDO1_110_CALC_HALT_LDO1_CAL_VALUE_MV", "VALIDATE": "", "BLOCK": "LDO1", "UNIT": "mV", "PARAM2": "{{halt_ldo1_cal_value_mv}}"}
		]==]
		
		print("!!!!!!!!!!!!!!")
		print (json.decode(str))
		t = json.decode(str)
		for v in pairs(json.decode(str)) do
			print(v,"~~~~~",t[v])
		end
		print("!!!!!!!!!!!!!!")
		
		--sendToSeqMsg = json.encode({jsonrpc = msg_obj.jsonrpc, id = msg_obj.id,  error = {code=errcode,message=errmsg}})
		print(str)
		local ret,err=request_zmq:send(str)
		if (ret) then
			print(time_utils.get_time_string_ms(),"[SEND]:",str);
			local buf = request_zmq:recv();
			print(time_utils.get_time_string_ms(),"[RECV]:",buf);
		else
			print("[ERROR]:",err)
		end
	end
end