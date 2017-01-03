import os
import shutil


def setup_module():
    config_dir = os.path.expanduser('~/testerconfig')
    config_file = os.path.join(config_dir, 'zmqports.json')
    
    path = os.path.split(__file__)[0]
    src_path = os.path.join(path, 'zmqports.json')
    shutil.copy(src_path, config_file)


def test_ports():
    from x527 import zmqports
    assert zmqports.PUB_CHANNEL == '101'
    assert zmqports.TEST_ENGINE_PORT == 6100
    assert zmqports.TEST_ENGINE_PUB == 6150
    assert zmqports.SEQUENCER_PORT == 6200
    assert zmqports.SEQUENCER_PUB == 6250
    assert zmqports.TE_PROXY_PUB == 6300
    assert zmqports.SEQUENCER_PROXY_PUB == 6350
    assert zmqports.DATALOGGER_PORT == 6400
    assert zmqports.DATALOGGER_PUB == 6450
    assert zmqports.DEBUG_LOGGER_PORT == 6700
    assert zmqports.SM_PUB == 6880
    assert zmqports.LOGGER_PUB == 6900
    assert zmqports.UART_PUB == 6800
    assert zmqports.ARM_PUB == 6990
    assert zmqports.FIXTURE_PUB == 6470
    assert zmqports.PUB_PORT == 50
    assert zmqports.SM_PORT == 6480
    assert zmqports.SN_PORT == 6330
    assert zmqports.PUB_PORT == 50
