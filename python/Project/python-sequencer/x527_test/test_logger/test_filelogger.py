import pytest
from x527.loggers import events
from x527.loggers.reporter import Report
from x527.loggers.filelogger import UartLogger, ArmLogger, DataloggerLogger, FlowLogger, FlowPlainLogger

@pytest.fixture
def report_start():
    report = Report()
    report.event = events.ITEM_START
    report.data = {"group": "INTELLIGENT SETUP", "description": "Relay Discharge", "to_pdca": True, "high": "",
                   "low": "", "tid": "NU RELAY DISCHARGER", "unit": ""}
    return report

@pytest.fixture
def report_finish():
    report = Report()
    report.event = events.ITEM_FINISH
    report.data = {"tid": "NU RELAY DISCHARGER", "to_pdca": True, "result": True, "value": "6000"}
    return report

def test_loggerlink(report_start, report_finish):
    f = FlowLogger()
    s = FlowPlainLogger()
    d = DataloggerLogger()
    a = ArmLogger()
    u = UartLogger()

    logger = u
    u.next_logger = a
    a.next_logger = d
    d.next_logger = s
    s.next_logger = f

    logger.log_sequence_start(None, 0, None)
    logger.log_data(None, 0, '2016-03-30 16:44:14:624,0.000354\r\n2016-03-30 16:44:14:625,0.000135\r\n', 'dl')
    logger.log_item_start(None, 0, report_start)
    logger.log_data(None, 0, '[089c1bba223c]io set(1,bit97=1)\x00', 'arm')
    logger.log_data(None, 0, '[089c1bba223c]ACK(DONE;1459340386,599,1459340386,600,1)\x00', 'arm')
    logger.log_data(None, 0, '56F8FBDC02D7@R1 [NAND ASPCommandInterface] AppleASPCmdInterfaceDriverBindingSupport', 'uart')
    logger.log_data(None, 0,
                    'ed:191 (0x9D610D90 - 0x9E6D4810) - coprocId=ANS, epid=6\n56F8FBDC02E2@R1 [NAND ASPCommandInterface] AppleASPCmdInterfaceDriverBindingStart:494 (0x9D610D90 - 0x9E6D4810) - driverStarted=1\n', 'uart')
    logger.log_data(None, 0,
                    '56F8FBDF0113@R1 [NAND ASPCommandInterface] AppleASPCmdInterfaceDriverBindingSupported:191 (0x9D610D90 - 0x9E6D4810) - coprocId=ANS, epid=6\n56F8FBDF011E@R1 [NAND ASPCommandInterface] AppleASPCmdInterfaceDriverBindingStart:494 (0x9D', 'uart')
    logger.log_data(None, 0, '610D90 - 0x9E6D4810) - driverStarted=1\n', 'uart')
    logger.log_data(None, 0, '[8574d49b0229]datalogger open(ADC_NORMAL)\x00', 'arm')
    logger.log_data(None, 0, '[8574d49b0229]ACK(DONE;1459340386,603,1459340386,603,0)\x00','arm')
    logger.log_data(None, 0,
                    '56F8FBE00064@R1 \x1b[2J\x00\x1b[0;0HX527 Diag (factory_x527_antenna2)\n56F8FBE00068@R1 BuildEng build X527Casaval2J0289j (2J0289j). Revision 4288f24.\n56F8FBE0006E@R1 \tBuilt at 2016/03/22 20:29:26\n','uart')
    logger.log_data(None, 0,
                    '56F8FBE00071@R1 script: alias bblib="smokeyshell -p -e \'if BBLib_cfg then print [[no init]] else require [[BBLib.Latest]];BB_Init() end\'"\n56F8FBE0007C@R1 script: alias vbat=pmuadc --read vbat\n56F8FBE00080@R1 script --cont startup.txt\n','uart')
    logger.log_data(None, 0,
                    '56F8FBE0008C@R1 script: alias bbon="meta \'baseband --on --load_firmware; baseband --wait_for_ready --ping --connect --serial_number\'"\n56F8FBE00097@R1 script: alias bbonbypa','uart')
    logger.log_data(None, 0,
                    'ss="meta \'baseband --on --load_firmware; baseband --wait_for_ready --ping --connect --serial_number --bypass en\'"\n56F8FBE000A3@R1 script: alias bboff=baseband --off\n56F8FBE000A7@R1 script: alias erase_rfcal=smokey Tools/RFCalErase --run\n56F8FBE000AC@R1 script: ','uart')
    logger.log_data(None, 0,
                    'alias erase_efs=smokey Tools/EFSErase --run\n56F8FBE000B1@R1 script: alias erase_pac=smokey Tools/RFPacErase --run\n56F8FBE000B6@R1 script: alias bblib="smokeyshell -p -e \'if BBLib_cfg then print [[no init]] else require [[BBLib.Latest]];BB_Init() end\'"\n','uart')
    logger.log_item_finish(None, 0, report_finish)
    logger.log_sequence_end(None, 0, None)
