import re
import os
from datetime import datetime


test_pattern = re.compile('==Test: (.+)\n')
subtest_pattern = re.compile('==SubTest: (.+)\n')
timestamp_pattern = '(\d{4}/\d{2}/\d{2} \d{2}:\d{2}:\d{2}\.\d{5}) '
arm_cmd_pattern = re.compile(timestamp_pattern+'\[Tester\]: \[\w*\]((?!ACK).+)\n?')
arm_ack_pattern = re.compile(timestamp_pattern+'\[Tester\]: \[\w*\]((?=ACK).+)\n?')
result_pattern = re.compile(timestamp_pattern+'  (PASS|FAIL|SKIP|UNKNOWN)\n')
limit_value_pattern = re.compile(' {26}lower:(\S*); upper:(\S*); value:(.*)\n')
diag_pattern = re.compile('(?:(\d{4}/\d{2}/\d{2} \d{2}:\d{2}:\d{2}\.\d{3}) {11})?((?(1).*|[0-9A-Z]{12}@[0-9A-Z]{2}.*)\r?)\n')

STAT_START = -1
STAT_INIT_CMD = 0
STAT_TEST = 2
STAT_ARM_CMD = 4
STAT_LIMIT = 6
STAT_RESULT = 7
STAT_END = 8

sequence = {}

class TestItem(object):
    def __init__(self):
        self.group = None
        self.low = None
        self.high = None
        self.value = None
        self.sequence = []


class DiagLine(object):
    def __init__(self):
        self.delay = 0
        self.context = ''


class ArmCmd(object):
    def __init__(self):
        self.command = ''
        self.response = ''
        self.delay = 0


def str2hex(data):
    for char in data:
        print hex(ord(char)),
    print ''


def timediff(ts1, ts2):
    a = datetime.strptime(ts1, '%Y/%m/%d %H:%M:%S.%f')
    b = datetime.strptime(ts2, '%Y/%m/%d %H:%M:%S.%f')
    delta = b-a
    mic2sec = round(float(delta.microseconds)/1000000, 3)
    time = delta.seconds+mic2sec
    return time


def c2py(c_str):
    if len(c_str) == 0:
        return None
    elif c_str[-1] == '\0':
        return c_str[:-1]
    else:
        return c_str


def get_test(line):
    if len(line) == 0:
        return None
    else:
        m = test_pattern.search(line)
        if m:
            return c2py(m.group(1))
        else:
            return None


def get_subtest(line):
    if len(line) == 0:
        return None
    else:
        m = subtest_pattern.search(line)
        if m:
            return c2py(m.group(1))
        else:
            return None


def get_arm_command(line):
    if len(line) == 0:
        return None
    else:
        m = arm_cmd_pattern.search(line)
        if m:
            if m.group(1):
                return c2py(m.group(1)), c2py(m.group(2))
            return None, c2py(m.group(2))
        else:
            return None


def get_arm_ack(line):
    if len(line) == 0:
        return None
    else:
        m = arm_ack_pattern.search(line)
        if m:
            if m.group(1):
                return c2py(m.group(1)), c2py(m.group(2))
            return None, c2py(m.group(2))
        else:
            return None


def get_result(line):
    if len(line) == 0:
        return None
    else:
        m = result_pattern.search(line)
        if m:
            return c2py(m.group(2))
        else:
            return None


def get_limit_value(line):
    if len(line) == 0:
        return None
    else:
        m = limit_value_pattern.search(line)
        if m:
            return c2py(m.group(1)), c2py(m.group(2)), c2py(m.group(3))
        else:
            return None


def get_diag(line):
    if len(line) == 0:
        return None
    else:
        m = diag_pattern.search(line)
        if m.group(2):
            new = '543210AF003A@R0 '+c2py(m.group(2))
            if m.group(1):
                return c2py(m.group(1)), new
            return None, new
        else:
            return None


def parse_log_line(line):
    if line == '\n' or line == '\0\n':
        return 'empty', line
    test = get_test(line)
    if test:
        return 'test', test
    subtest = get_subtest(line)
    if subtest:
        return 'subtest', subtest
    cmd = get_arm_command(line)
    if cmd:
        return 'cmd', cmd
    ack = get_arm_ack(line)
    if ack:
        return 'ack', ack
    res = get_result(line)
    if res:
        return 'result', res
    limit = get_limit_value(line)
    if limit:
        return 'limit', limit
    diag = get_diag(line)
    if diag:
        return 'diag', diag
    return 'unknown', line


def parse_log(sequence, path):
    last_state = STAT_START
    last_cmd = []
    last_test = []
    pwd = os.path.split(__file__)[0]
    path = os.path.join(pwd, path)
    if os.path.exists(path):
        with open(path, 'r') as f:
            lines = f.readlines()
        for line in lines:
            s = parse_log_line(line)
            if s[0] == 'unknown' or s[0] == 'empty' or s[0] == 'test':
                continue
            if s[0] == 'subtest':
                sequence[s[1]] = TestItem()
                last_state = STAT_TEST
                last_cmd = []
                last_test = s[1]
                continue
            if s[0] == 'cmd':
                if last_state == STAT_START:
                    last_state = STAT_INIT_CMD
                    last_cmd = s[1]
                    if '__enter__' not in sequence:
                        sequence['__enter__'] = TestItem()
                        last_test = '__enter__'
                elif last_state == STAT_TEST:
                    last_state = STAT_ARM_CMD
                    last_cmd = s[1]
                # todo: exit command sequence may be required for test engine after end_test
                else:
                    continue
            if s[0] == 'ack':
                if last_state == STAT_INIT_CMD:
                    new = ArmCmd()
                    new.command = last_cmd[1]
                    new.response = s[1][1]
                    new.delay = timediff(last_cmd[0], s[1][0])
                    sequence[last_test].sequence.append(new)
                    last_state = STAT_START
                    last_cmd = s[1]
                    continue
                if last_state == STAT_ARM_CMD:
                    new = ArmCmd()
                    new.command = last_cmd[1]
                    new.response = s[1][1]
                    new.delay = timediff(last_cmd[0], s[1][0])
                    sequence[last_test].sequence.append(new)
                    last_state = STAT_TEST
                    last_cmd = s[1]
                    continue
                continue
            if s[0] == 'diag':
                if last_state == STAT_INIT_CMD:
                    new = DiagLine()
                    new.context = s[1][1]
                    new.delay = timediff(last_cmd[0], s[1][0])
                    sequence[last_test].sequence.append(new)
                    last_state = STAT_START
                    last_cmd = s[1]
                    continue
                if last_state == STAT_ARM_CMD:
                    new = DiagLine()
                    new.context = s[1][1]
                    new.delay = timediff(last_cmd[0], s[1][0])
                    sequence[last_test].sequence.append(new)
                    last_state = STAT_TEST
                    last_cmd = s[1]
                    continue
                if last_state == STAT_TEST:
                    new = DiagLine()
                    new.context = s[1][1]
                    if last_cmd and s[1][0]:
                        if last_cmd[0]:
                            new.delay = timediff(last_cmd[0], s[1][0])
                    else:
                        new.delay = 0
                    sequence[last_test].sequence.append(new)
                    last_cmd = s[1]
                    continue
                continue
    else:
        print 'Flow log file not exist!'
        return False


def dump_sequence(sequence, path):
    pwd = os.path.split(__file__)[0]
    path = os.path.join(pwd, path)
    with open(path, 'w') as f:
        for test in sequence:
            f.write('TEST:\t'+test+'\r\n')
            for item in sequence[test].sequence:
                if isinstance(item, ArmCmd):
                    f.write('\tARM command:\t\t'+item.command+'\r\n')
                    f.write('\tARM response:\t'+str(item.delay)+'\t'+item.response+'\r\n')
                if isinstance(item, DiagLine):
                    f.write('\tDiag:\t\t'+str(item.delay)+'\t'+item.context+'\r\n')
    print 'dump finished'


def make_response(sequence, tid, command):
    response = []
    cmd_found = False
    i = 0
    if tid in sequence:
        for item in sequence[tid].sequence:
            if isinstance(item, ArmCmd):
                if cmd_found:
                    break
                if command == item.command:
                    if i > 0:
                        response = []   # clear diag context if matched command is not the first command
                    response.append(['ARM', item.delay, item.response])
                    cmd_found = True
                i += 1
            if isinstance(item, DiagLine):
                if cmd_found or i == 0:
                    response.append(['DUT', item.delay, item.context])
    else:
        print 'TID unknown !!!'
    return response

if __name__ == '__main__':
    sequence = {}
    parse_log(sequence, 'flow.txt')
    dump_sequence(sequence, 'dump.txt')
