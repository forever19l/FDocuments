__author__ = 'wei'
import cmd
from x527.tinyrpc.proxy.sequencerporxy import SequencerProxy
import zmq
from x527.loggers import ZmqPublisher
from x527 import zmqports
import argparse
from x527.tinyrpc.protocols.jsonrpc import RPCError
import sys
import os
import time
import traceback
from datetime import datetime
import readline
readline.parse_and_bind("bind ^I rl_complete")

def handle_response(afunc):

    def _(*args, **kwargs):
        try:
            reply = afunc(*args, **kwargs)
            if reply is not None:
                print reply
        except RPCError as e:
            print e.message, os.linesep, traceback.format_exc()

    _.__doc__ = afunc.__doc__
    return _

class SequencerDebugger(cmd.Cmd):

    prompt = 'sdb>'
    intro = 'Sequencer debugger'
    sequencer = None
    break_points = []
    this_run = None
    stat = ''

    def do_EOF(self, line):
        '''Ctrl-D to quit sdb without stopping the sequencer server'''
        return True

    @handle_response
    def do_load(self, sequence_db):
        """
        Usage: load [sequence_db_name]
        load a sequencer database. Default loads a randomly generated database"""
        if not sequence_db:
            return 'you must specifcy a sequence database name'
        return self.sequencer.load(sequence_db)

    @handle_response
    def do_abort(self, line):
        '''abort the current running sequence'''
        return self.sequencer.abort(None)

    @handle_response
    def do_run(self, line):
        '''run the whole sequence without regard for any breakpoint'''
        return self.sequencer.run(None)

    def show_sequence_item(self, item, pc):
        line_no = int(item[0])
        if line_no==pc+1:
            sys.stdout.write('-> ')
        else:
            sys.stdout.write('   ')
        test = eval(item[1])
        sys.stdout.write(str(item[0]) + ': ')
        sys.stdout.write('%s  | %s | %s | %s' % (test['GROUP'], test['TID'], test['FUNCTION'], test['DESCRIPTION']))
        if test.has_key('PARAM1'):
            sys.stdout.write(' | ' +test['PARAM1'])
        sys.stdout.write(os.linesep)
        if test.has_key('PARAM2'):
            sys.stdout.write(' | ' +test['PARAM2'])
        sys.stdout.write(' | %s | %s | %s | %s | %s ' % (test['UNIT'], test['LOW'], test['HIGH'], test['KEY'], test['VAL']))

        sys.stdout.write(os.linesep)

    @handle_response
    def do_list(self, lines):
        '''list [lines] around the current sequence item'''
        if len(lines)==0:
            lines = '10'
        reply = self.sequencer.list(lines)
        pc, start, stop = reply[0]
        listings = reply[1:]
        for item in listings:
            self.show_sequence_item(item, pc)
        return

    @handle_response
    def do_break(self, line):
        '''set break point at [line]'''
        if line=='':
            line = '1'
        self.break_points.append(int(line))

    def do_all(self, line):
        '''show all the breakpoints'''
        for b in self.break_points:
            print '  ' + str(b)

    @handle_response
    def do_next(self, line):
        '''show the next line that will be executed'''
        return self.sequencer.s_next(None)

    @handle_response
    def do_skip(self, line):
        '''skip the right next test, equal to jump N+1'''
        reply = self.sequencer.skip(None)
        if reply:
            self.show_sequence_item(reply, -100)
        else:
            print 'skip failed'

    @handle_response
    def do_continue(self, line):
        """continue execution from the current position,
        if you run continue, breakpoints are honored.
        If you use run, breakpoints are not honored"""
        reply = ' '
        if self.this_run is None:
            self.this_run = [b for b in self.break_points]

        while reply is not None:
            next_p = self.sequencer.s_next(None)
            if next_p in self.this_run:
                sys.stdout.write('BREAK: ')
                self.do_list('1')
                self.this_run.remove(next_p)
                return
            reply = self.step_op()

        # we reached the end of the run
        self.this_run = None

    @handle_response
    def do_jump(self, dest):
        '''jumpt to the destination. Destination can be line number, group name or TID'''
        reply = self.sequencer.jump(dest)
        self.show_sequence_item(reply, -100)

    @handle_response
    def do_print(self, var_name):
        '''show the value of [var_name]'''
        return self.sequencer.show(var_name)

    @handle_response
    def do_status(self, line):
        '''return the current running status of the sequence'''
        self.stat = self.sequencer.status(None, timeout=0)
        return self.stat

    @handle_response
    def do_s(self, line):
        '''short name for step'''
        self.do_step(None)

    def step_op(self):
        self.do_list('1')
        t1 = datetime.now()
        reply = self.sequencer.step(None, timeout=0)
        if reply==None:
            print 'reached the end of sequence'
        else:
            t2 = datetime.now()
            print 'execution time: ' + str((t2-t1).total_seconds()) + ' seconds'
        return reply

    @handle_response
    def do_step(self, line):
        '''execute the current line, move PC to the next line'''
        self.step_op()

    @handle_response
    def do_wait(self, timeout):
        '''wait [timeout] seconds for the test sequence to finish'''
        if timeout=='':
            timeout = '0'
        return self.sequencer.wait(None, timeout=int(timeout))

    @handle_response
    def do_loop(self, count):
        '''wait [timeout] seconds for the test sequence to finish'''
        if count == '':
            count = '1'
        count = int(count)
        for i in range(count):
            self.do_run(None)
            start = time.time()
            time.sleep(3)
            while True:
                self.do_status(None)
                print 'Loop {0} of {1} running, time elapsed={2}s'.format(i+1, count, int(time.time()-start))
                if self.stat == 'READY':
                    print 'loop {0} of {1} finished'.format(i+1, count)
                    break
                time.sleep(5)

    def do_quit(self, line):
        '''quit sdb and stop the sequencer server. If you just want to quit sdb without stopping
        the sequencer server you shoudl use ctrl-D'''
        self.sequencer.send_cmd('::stop::', None)
        return True

    '''
    def default(self, line):
        print 'in default line is: ' + line
    '''

if __name__=='__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--site', help='the site of the sequencer to connect to', type=int, default=0)
    parser.add_argument('-p', '--testplan', help='the test plan to load', default=None)
    parser.add_argument('-l', '--loop', help='loop number', default=0)
    args = parser.parse_args()

    site = args.site
    sdb = SequencerDebugger()
    ctx = zmq.Context.instance()
    sequencer = SequencerProxy(site, ZmqPublisher(ctx, "tcp://*:" + str(zmqports.SEQUENCER_PROXY_PUB + site),
                                                "Sequencer Proxy"))
    sdb.sequencer = sequencer
    if args.testplan:
        sdb.do_load(args.testplan)
    if args.loop:
        for i in range(int(args.loop)):
            sdb.do_run(None)
            time.sleep(3)
            while True:
                print datetime.now()
                sdb.do_status(None)
                print 'Loop {} of {} running. status=',sdb.stat
                if sdb.stat == 'READY':
                    print 'loop {0} of {1} finished'.format(i+1,int(args.loop))
                    break
                time.sleep(5)
    sdb.cmdloop()
