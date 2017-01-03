import re
import argparse
import time
from Queue import Queue
from datetime import datetime
from x527.emulator import *
from socket_server import PortServer, CmdServer, SeqListener
from log_replay import sequence, parse_log, dump_sequence, make_response


boost = False
verbose = False
queue = Queue()
tid = None


def ts():
    if verbose:
        return str(datetime.now())+'\t'+'Main'+'\t'
    return ''


def parse_arm_cmd(cmd_str):
    arm_cmd_pattern = re.compile(r'\[([\S|\d]*)\]([\S| ]+)(\(.*\))')
    res = arm_cmd_pattern.search(cmd_str)
    if res:
        uid = res.group(1)
        command = res.group(2)
        param = res.group(3)
        return uid, command, param
    else:
        print 'Invalid ARM command:', cmd_str
        return None, None, None


def send_response(response):
    for item in response:
        # print '>>', item
        if not boost and item[1]:
            time.sleep(item[1])
        if item[0] == 'ARM':
            print ts()+'ARM <-', item[2]
            if not arm_con.send(item[2]):
                print ts()+'lua engine closed connection unexpectedly !'
        elif item[0] == 'DUT':
            print ts()+'DUT <-', item[2]
            if not dut.send(item[2]+'\n'):
                print ts()+'lua engine closed connection unexpectedly !'
        else:
            print ts()+'unknown target for response !'


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-r', '--replay', help='replay flow log to emulate specific sequence')
    parser.add_argument('-b', '--boost', help='boost to ignore any delay', action='store_true', default=False)
    parser.add_argument('-v', '--verbose', help='enable debug verbosity', action='store_true', default=False)
    args = parser.parse_args()
    if args.boost:
        print 'boost mode'
        boost = True
    if args.verbose:
        print 'verbose mode'
        verbose = True

    parse_log(sequence, 'flow.txt')
    dump_sequence(sequence, 'dump.txt')

    arm_con = CmdServer('ARM', HOST, CONPORT, queue, verbose)
    arm_con.start()
    arm_echo = PortServer('Echo', HOST, ECHOPORT, queue, verbose)
    arm_echo.start()
    dut = CmdServer('DUT', HOST, DUTPORT, queue, verbose)
    dut.start()
    datalogger = PortServer('DL', HOST, DATALOGGERPORT, queue, verbose)
    datalogger.start()
    seq_listener = SeqListener('tcp://localhost:', 0, queue, verbose)
    seq_listener.start()

    while True:
        source, data = queue.get(True)
        # print source, ':', data
        if source == 'TID':
            tid = data
            if tid in ['BOOT_DIAGS_100_DETE', 'SLEEP1_ADC_130_DETE', 'SLEEP1_ADC_220_DETE', 'SLEEP2_ADC_260_DETE']:
                response = make_response(sequence, tid, '')
                send_response(response)
        elif source == 'ARM':
            arm_echo.send(data)
            uuid, cmd, param = parse_arm_cmd(data.strip())
            if cmd is None:
                continue
            # tid = seq_listener.tid
            if tid is None:
                tid = '__enter__'
            # print ts()+'uuid:', uuid, 'cmd:', cmd+param
            response = make_response(sequence, tid, cmd+param)
            if len(response) == 0:      # retry if tid received with delay
                time.sleep(1)
                tid = seq_listener.tid
                response = make_response(sequence, tid, cmd+param)
            # print ts()+'response: '+str(response)
            if len(response) > 0:
                send_response(response)
            else:   # default mock response in case command not exists in flow
                if cmd in ['dac set', 'system set', 'datalogger close', 'codec close pdm']:
                    response = [['ARM', 0, 'ACK(DONE)\r\n']]
                if cmd in ['io set']:
                    response =[['ARM', 0, 'ACK(DONE;1452588839,839,1452588839,843,4)\r\n']]
                send_response(response)
        elif source == 'DUT':
            response = make_response(sequence, tid, '')
            if len(response) == 0:
                head = '543210AF003A@R0 '
                if 'smokeyshell' in data:
                    response = [['DUT', 0, head+data], ['DUT', 0, head+'PASS'], ['DUT', 0, head+'[001C0529:0401E626] :-)']]
            send_response(response)
