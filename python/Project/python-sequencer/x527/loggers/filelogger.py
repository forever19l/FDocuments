import re
import pypudding
from datetime import datetime
from reportlogger import *
from publisher import NoOpPublisher
from x527.sequencer.sequencer import SKIP_ITEM


class CSVRun(object):

    def __init__(self, ts):
        self.sn = ''
        self.items = []
        self.start_ts = ts
        self.result = ''
        self.fail_list = ''


class CSVLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(CSVLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.publisher = publisher
        gh_station = pypudding.IPGHStation()
        self.product = gh_station[pypudding.IP_PRODUCT]
        self.station_type = gh_station[pypudding.IP_STATION_TYPE]
        self.station_id = gh_station[pypudding.IP_STATION_ID]
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = self.product + '_' + self.station_type + '_' + ts + '_all.csv'
        self.log_path = os.path.join(self.log_folder, filename)
        self.log_f = open(self.log_path, 'w+')
        self.header_written = False
        self.log_f.write('Product,SerialNumber,Station_ID,Site_ID,PASS/FAIL,Error_Message,Failed_List,Test Start Time,'
                         'Test Stop Time,')
        self.log_f.flush()
        self.runs = [None for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        return

    @call_next
    def log_sequence_start(self, ts, site, report):
        self.runs[site] = None
        self.runs[site] = CSVRun(str(datetime.now()))

    @call_next
    def log_item_start(self, ts, site, report):
        item = dict(tid=report.data['group']+'@'+report.data['tid'],
                    low=report.data['low'], high=report.data['high'], unit=report.data['unit'], result='')
        self.runs[site].items.append(item)

    @call_next
    def log_item_finish(self, ts, site, report):
        run = self.runs[site]
        item = run.items[-1]
        if 'value' in report.data:
            result = str(report.data['value'])
        else:
            result = str(report.data['error'])
        item['result'] = result.replace(',', ';')
        if report.data['result'] < 1:
            run.fail_list += item['tid'] + ';'

    @call_next
    def log_attribute_found(self, ts, site, report):
        if 'mlbsn' in report.data['name'].lower():
            self.runs[site].sn = report.data['value']

    @call_next
    def log_sequence_end(self, ts, site, report):
        run = self.runs[site]
        if not self.header_written:
            skip_fields = ',,,,,,,,,'
            for item in run.items:
                self.log_f.write(item['tid'] + ',')
            self.log_f.write(os.linesep + 'Upper Limited----------->' + skip_fields)
            for item in run.items:
                self.log_f.write(item['high'] + ',')
            self.log_f.write(os.linesep + 'Lower Limited----------->' + skip_fields)
            for item in run.items:
                self.log_f.write(item['low'] + ',')
            self.log_f.write(os.linesep + 'Measurement unit------>' + skip_fields)
            for item in run.items:
                self.log_f.write(item['unit'] + ',')
            self.log_f.write(os.linesep)
            self.log_f.flush()
            self.header_written = True

        if report.data['result'] > 0:
            run.result = 'PASS'
        else:
            run.result = 'FAIL'
        msg = ''
        if 'error_msg' in report.data:
            msg = report.data.get('error_msg', '')
        if ',' in msg:
            msg = msg.replace(',', ';')
        line = '{0},{1},{2},{3},{4},{5},{6},{7},{8},'.format(
                                                            self.product,     # Product
                                                            run.sn,           # SerialNumber
                                                            self.station_id,  # Station_ID
                                                            site,             # Site_ID
                                                            run.result,       # PASS/FAIL
                                                            msg,              # Error_Message
                                                            run.fail_list,    # Failed_List
                                                            run.start_ts,     # Test Start Time
                                                            datetime.now(),   # Test Stop Time
                                                            )
        for item in run.items:
            line += str(item['result']) + ','
        self.log_f.write(line + os.linesep)
        self.log_f.flush()

    @call_next
    def close(self):
        self.log_f.close()


class PivotLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(PivotLogger, self).__init__()
        self.log_folder = log_folder
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        file_path = os.path.join(log_folder, ts + '_pivot_acc.csv')
        log_f = open(file_path, 'w+')
        log_f.write('index, site, sn, group, tid, unit, low, high, month, day, hour, minute, second, microsec,'
                    'result, value, fail_msg\n')
        log_f.flush()
        self.log_f_acc = log_f
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.log_f = [None for i in range(site_count)]
        self.uut_sn = ['' for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        return

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        self.devices[site] = DUT(ReportLogger.next_index())
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_pivot.csv'
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        log_f.write('site,sn,group,tid,unit,low,high,month,day,hour,minute,second,microsec,result,value,fail_msg\n')
        log_f.flush()
        self.log_f[site] = log_f

    @call_next
    def log_item_start(self, ts, site, report):
        data = report.data
        item = dict(group=data['group'], tid=data['tid'], unit=data['unit'],
                    low=data['low'], high=data['high'], result='', msg='')
        self.devices[site].running_item = item

    @call_next
    def log_item_finish(self, ts, site, report):
        dut = self.devices[site]
        item = dut.running_item
        self.log_f[site].write('%d,%s,%s,%s,%s,%s,%s,' %
                               (site, dut.sn, item['group'], item['tid'], item['unit'], item['low'], item['high'])
                               )
        dt = datetime.strptime(ts, '%m-%d_%H:%M:%S.%f')
        self.log_f[site].write(datetime.strftime(dt, '%m,%d,%H,%M,%S,%f,'))
        self.log_f_acc.write('%d,%d,%s,%s,%s,%s,%s,%s,' %
                             (dut.index, site, dut.sn, item['group'], item['tid'],
                              item['unit'], item['low'], item['high'])
                             )
        self.log_f_acc.write(datetime.strftime(dt, '%m,%d,%H,%M,%S,%f,'))
        res = report.data['result']
        if res == True:
            self.log_f[site].write('Pass' + ',')
            self.log_f_acc.write('Pass' + ',')
        elif res == SKIP_ITEM:
            self.log_f[site].write('Skip' + ',')
            self.log_f_acc.write('Skip' + ',')
        else:
            self.log_f[site].write('Fail' + ',')
            self.log_f_acc.write('Fail' + ',')
        if report.data.has_key('error'):
            error = str(report.data['error'])
            if ',' in error:
                error = error.replace(',', ';')
            self.log_f[site].write(',' + error)
            self.log_f_acc.write(',' + error)
        else:
            value = str(report.data['value'])
            if ',' in value:
                value = value.replace(',', ';')
            self.log_f[site].write(value + ',')
            self.log_f_acc.write(value + ',')
        self.log_f[site].write(os.linesep)
        self.log_f[site].flush()
        self.log_f_acc.write(os.linesep)
        self.log_f_acc.flush()
        self.uut_sn[site] = str(dut.sn)

    @call_next
    def log_attribute_found(self, ts, site, report):
        if 'mlbsn' in report.data['name'].lower():
            self.devices[site].sn = report.data['value']

    @call_next
    def log_sequence_end(self, ts, site, report):
        self.log_f[site].close()

    @call_next
    def close(self):
        self.log_f_acc.close()


class FlowLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(FlowLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]
        self.uart_buffer = ['' for i in range(site_count)]

    @staticmethod
    def fpga_timestamp(ts):
        sec = ts[:8]
        usec = ts[8:]
        str_ts = str(int(sec, 16))
        return str(datetime.fromtimestamp(float(str_ts))) + '.' + str(int(usec, 16)).rjust(3, '0')

    @call_next
    def log_data(self, ts, site, report, id):
        if 'uart' in id:
            s = self.uart_buffer[site] + report  # concat with last time leftover string
            self.uart_buffer[site] = ''
            s = s.replace('\r', '\n')  # unify the line ending char
            lines = s.split('\n')  # split per screen lines, last slice have no original line ending
            ts_pattern = re.compile('([A-F0-9]{12})@R[01] (.*)')  # FPGA timestamp
            log = self.logfile[site]
            if log is None:
                return
            for i in range(len(lines)):
                if i < len(lines) - 1:
                    m = ts_pattern.search(lines[i])
                    if m:
                        log.write(self.fpga_timestamp(m.group(1)) + ' ' * 10 + m.group(2) + os.linesep)
                    else:
                        log.write(lines[i] + os.linesep)
                else:
                    self.uart_buffer[site] = lines[i]
            log.flush()
        elif 'arm' in id:
            ts = str(datetime.now())
            log = self.logfile[site]
            if log is None:
                return
            log.write(ts + ' ' * 5 + report + os.linesep)
            log.flush()

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        self.devices[site] = DUT(ReportLogger.next_index())
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_flow.txt'
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f
        self.uart_buffer[site] = ''

    @call_next
    def log_item_start(self, ts, site, report):
        data = report.data
        item = dict(group=data['group'], tid=data['tid'], unit=data['unit'], low=data['low'], high=data['high'],
                    result='', msg='')
        self.devices[site].running_item = item
        log = self.logfile[site]
        if log is None:
            return
        log.write('==Test: ' + item['group'] + os.linesep)
        log.write('==SubTest: ' + item['tid'] + os.linesep)
        log.flush()

    @call_next
    def log_item_finish(self, ts, site, report):
        item = self.devices[site].running_item
        if 'error' in report.data:
            value = str(report.data['error'])
        else:
            value = str(report.data['value'])
        log = self.logfile[site]
        log.write('\t\t\tlower:{0}; upper:{1}; value:{2}'.format(item['low'], item['high'], value) + os.linesep)
        res = report.data['result']
        if res == True:
            result = 'PASS'
        elif res == SKIP_ITEM:
            result = 'SKIP'
        else:
            result = 'FAIL'
        log.write(str(datetime.now()) + ' '*3 + result + os.linesep)
        log.flush()

    @call_next
    def log_sequence_end(self, ts, site, report):
        if len(self.uart_buffer[site]) > 0:
            self.logfile[site].write(self.uart_buffer[site] + os.linesep)
            self.uart_buffer[site] = ''
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return


class FlowPlainLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(FlowPlainLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        return

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        self.devices[site] = DUT(ReportLogger.next_index())
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_flow_plain.txt'
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f

    @call_next
    def log_item_start(self, ts, site, report):
        data = report.data
        item = dict(group=data['group'], tid=data['tid'], unit=data['unit'], low=data['low'], high=data['high'],
                    result='', msg='')
        self.devices[site].running_item = item
        log = self.logfile[site]
        log.write('==Test: ' + item['group'] + os.linesep)
        log.write('==SubTest: ' + item['tid'] + os.linesep)
        log.flush()

    @call_next
    def log_item_finish(self, ts, site, report):
        item = self.devices[site].running_item
        if 'error' in report.data:
            value = str(report.data['error'])
        else:
            value = str(report.data['value'])
        log = self.logfile[site]
        log.write('\t\t\tlower:{0}; upper:{1}; value:{2}'.format(item['low'], item['high'], value) + os.linesep)
        res = report.data['result']
        if res == True:
            result = 'PASS'
        elif res == SKIP_ITEM:
            result = 'SKIP'
        else:
            result = 'FAIL'
        log.write(str(datetime.now()) + ' '*3 + result + os.linesep)
        log.flush()

    @call_next
    def log_sequence_end(self, ts, site, report):
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return


class UartLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(UartLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]
        self.uart_buffer = ['' for i in range(site_count)]

    @staticmethod
    def fpga_timestamp(ts):
        sec = ts[:8]
        usec = ts[8:]
        str_ts = str(int(sec, 16))
        return str(datetime.fromtimestamp(float(str_ts))) + '.' + str(int(usec, 16)).rjust(3, '0')

    @call_next
    def log_data(self, ts, site, report, id):
        if 'uart' in id:
            s = self.uart_buffer[site] + report     # concat with last time leftover string
            self.uart_buffer[site] = ''
            s = s.replace('\r', '\n')         # unify the line ending char
            lines = s.split('\n')                # split per screen lines, last slice have no original line ending
            ts_pattern = re.compile('([A-F0-9]{12})@R[01] (.*)')    # FPGA timestamp
            log = self.logfile[site]
            if log is None:
                return
            for i in range(len(lines)):
                if i < len(lines)-1:
                    m = ts_pattern.search(lines[i])
                    if m:
                        log.write(self.fpga_timestamp(m.group(1)) + ' ' * 10 + m.group(2) + os.linesep)
                    else:
                        log.write(str(datetime.now()) + ' ' * 10 + lines[i] + os.linesep)
                else:
                    self.uart_buffer[site] = lines[i]
            log.flush()

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_uart.txt'
        self.devices[site] = DUT(ReportLogger.next_index())
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f
        self.uart_buffer[site] = ''

    @call_next
    def log_item_start(self, ts, site, report):
        return

    @call_next
    def log_item_finish(self, ts, site, report):
        return

    @call_next
    def log_sequence_end(self, ts, site, report):
        if len(self.uart_buffer[site]) > 0:
            self.logfile[site].write(self.uart_buffer[site] + os.linesep)
            self.uart_buffer[site] = ''
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return


class ArmLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(ArmLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        if 'arm' in id:
            ts = str(datetime.now())
            log = self.logfile[site]
            if log is None:
                return
            log.write(ts + ' '*5 + report + os.linesep)
            log.flush()

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_hw.txt'
        self.devices[site] = DUT(ReportLogger.next_index())
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f

    @call_next
    def log_item_start(self, ts, site, report):
        return

    @call_next
    def log_item_finish(self, ts, site, report):
        return

    @call_next
    def log_sequence_end(self, ts, site, report):
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return


class DataloggerLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(DataloggerLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.devices = {i: None for i in range(site_count)}
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]
        self.buffer = ['' for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        if 'dl' in id:
            log = self.logfile[site]
            if log is None:
                return
            s = self.buffer[site] + report
            lines = s.split('\r\n')
            for i in range(len(lines)):
                if i < len(lines) -1:
                    if len(lines[i]) > 0:
                        log.write(lines[i] + os.linesep)
                else:
                    self.buffer[site] = lines[i]
            log.flush()

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        self.devices[site] = DUT(ReportLogger.next_index())
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_datalogger.csv'
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f
        self.buffer[site] = ''

    @call_next
    def log_item_start(self, ts, site, report):
        return

    @call_next
    def log_item_finish(self, ts, site, report):
        return

    @call_next
    def log_sequence_end(self, ts, site, report):
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return


class EngineLogger(ReportLogger):

    def __init__(self, log_folder='/tmp', next_logger=None, site_count=6, publisher=NoOpPublisher()):
        super(EngineLogger, self).__init__()
        self.log_folder = log_folder
        self.next_logger = next_logger
        self.publisher = publisher
        self.log_path = ['' for i in range(site_count)]
        self.logfile = [None for i in range(site_count)]

    @call_next
    def log_data(self, ts, site, report, id):
        if 'engine' in id:
            log = self.logfile[site]
            if log is None:
                return
            if report[-1] != os.linesep:
                report = report + os.linesep
            report = str(datetime.now()) + ' '*3 + report
            log.write(report)
            log.flush()

    @call_next
    def log_sequence_start(self, ts, site, report):
        gh_station = pypudding.IPGHStation()
        product = gh_station[pypudding.IP_PRODUCT]
        station_type = gh_station[pypudding.IP_STATION_TYPE]
        ts = datetime.strftime(datetime.now(), '%m-%d-%H-%M-%S')
        filename = product + '_' + station_type + '_UUT{}_'.format(site) + ts + '_engine.txt'
        self.log_path[site] = os.path.join(self.log_folder, filename)
        log_f = open(self.log_path[site], 'w+')
        if self.logfile[site] is not None:
            self.logfile[site].close()
        self.logfile[site] = log_f

    @call_next
    def log_item_start(self, ts, site, report):
        return

    @call_next
    def log_item_finish(self, ts, site, report):
        return

    @call_next
    def log_sequence_end(self, ts, site, report):
        self.logfile[site].close()
        self.logfile[site] = None

    @call_next
    def log_attribute_found(self, ts, site, report):
        return

    @call_next
    def close(self):
        return
