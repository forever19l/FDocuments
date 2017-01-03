x527/827 Test Sequencer
=====

To install the python requirements do

    easy_install pip
    pip install -r requirements.txt

Pre-requisites to launch lua test engine

    download Lua 5.1.4 source package
	make macosx
	make install
	install luarocks and all dependent libs in dependancies.sh (or copy all lib files from overlay package)
	sudo dependancies.sh

To launch engine and sequencer manually

	clone tm_platform_v2 repository
	connect to fixture through ethernet with IP in subnet of 169.254.1.x
	launch test engine by "lua LuaDriver/Driver/test_engine_C_zmq.lua -u 0" (UUT 0)
	clone python-sequencer repository add path to $PYTHONPATH
	launch sequencer by "python x527/sequencer/sequencer.py -c -s 0" (UUT 0)
	launch sdb to control sequencer by "python x527/sdb.py -s 0"
	load test plan csv file in sdb and run
