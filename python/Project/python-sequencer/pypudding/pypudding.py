__version_info__=('1','0','2','0')
__version__='.'.join(__version_info__)

from pypudding_w import *
import pypudding_w
import logging
import re

INSTANT_PUDDING_VERSION = IP_getVersion()

# refer to https://hwtewiki.apple.com:8443/display/dc/PDCA+Data+Structure+Definition
PDCA_TEST_NAME_MAX_LEN = 48
PDCA_SUBTEST_NAME_MANX_LEN = 64
PDCA_SUBSUBTEST_NAME_MANX_LEN = 128
PDCA_UNIT_MAX_LEN = 10
PDCA_FAIL_MSG_MAX_LEN=512
PDCA_VERSION_MAX_LEN = 48
PDCA_LIMIT_MAX_LEN = 48
PDCA_ATTRIBUTE_NAME_MAX_LEN = 64
PDCA_ATTRIBUTE_VALUE_MAX_LEN = 256

units = [name for name in dir(pypudding_w) if name.startswith('IP_UNITS') ]
units = [eval(name) for name in units]

def to_pudding_unit(unit):
    if None==unit:
        return None
    for u in units:
        if unit.upper()==u.upper():
            return u
    return unit

def to_pudding_result(is_pass):
    if is_pass:
        return IP_PASS
    else:
        return IP_FAIL

def check_pdca_len(var, name, max_len):
    length = len(var)
    if length > max_len:
        raise InstantPuddingError(name + ' is too long. Length is ' + str(length) + ', the max length allowed by PDCA is ' + str(max_len) + '. The value is "' + var + '"')

def check_pdca_numberic(val_str):
    pattern = '^[+-]?(\d+(\.\d+)?|\.\d+)([eE][+-]?\d+)?$'
    m = re.match(pattern, val_str)
    if m is None:
        raise InstantPuddingError(val_str + ' is not a numerical value accepted by PDCA')

class InstantPuddingError(Exception):
    def __init__(self, msg):
        super(InstantPuddingError, self).__init__(msg)
        self.message = msg
    def __str__(self):
        return repr(self.message)

class IPReply(object):
    '''reply object from pudding calls'''
    def __init__(self, reply):
        self.reply = reply
        
    def __del__(self):
        try:
            IP_reply_destroy(self.reply)
        except Exception, e:
            pass  #IP_reply_destroy may become None when the main program exits
      
    def get_error_msg(self):
        return IP_reply_getError(self.reply)
    error_msg=property(fget=get_error_msg)
    
    def get_msg_class(self):
        return IP_reply_getClass(self.reply)
    msg_class = property(fget=get_msg_class)
    
    def get_msg_ID(self):
        return IP_reply_getMessageID(self.reply)
    msg_ID=property(fget=get_msg_ID)
    
    def __repr__(self):
        return 'class:' + str(self.msg_class) + ' id:' + str(self.msg_ID) + '; message:' + self.error_msg
    
def IP_reply_assert(aFunc):
    '''decorator to assert the reply represents a successful operation'''
    def asserted_func(*args, **kw):
        reply = aFunc(*args, **kw)
        succeed = IP_success(reply)
        if not succeed:
            raise InstantPuddingError('Error calling ' + aFunc.__name__ + ': ' + str(IPReply(reply)))
        return IPReply(reply)
    asserted_func.__name__=aFunc.__name__
    asserted_func.__doc__=aFunc.__doc__
    return asserted_func
 
def API_check_assert(aFunc):
    '''decorator to assert the return is true'''
    def asserted_func(*args, **kw):
        check = aFunc(*args, **kw)
        if not check:
            raise InstantPuddingError('InstantPudding call ' + aFunc.__name__ + ' returns false')
    asserted_func.__name__=aFunc.__name__
    asserted_func.__doc__=aFunc.__doc__
    return asserted_func
        
class IPUUT(object):
    '''pudding's representation for UUT'''
    #now we allow the user to defer setting the sn. Because there are a lot of cases
    #where the SN is not known until the test has started      
    def __init__(self, sn=None, logger_name='iTest.pudding'):
        self.logger = logging.getLogger(logger_name)
        self.sn = sn
        self.uut = None
        self.final_result = IP_FAIL
        
    def start(self):
        [reply, uut] = IP_UUTStart()
        if not IP_success(reply):
            raise Exception(IPReply(reply))
        self.logger.debug('IP_UUTStart succeeded:' + str(self.sn))
        IP_reply_destroy(reply)
        self.uut = uut
        self.reply_error = None
        self.attributes ={}
        if self.sn:
            self.add_attribute(IP_ATTRIBUTE_SERIALNUMBER, self.sn)
    
    def set_sn(self, sn):
        self.sn = str(sn)
        self.add_attribute(IP_ATTRIBUTE_SERIALNUMBER, self.sn)
        
    def destroy(self):
        if self.uut:
            self.logger.debug('calling IP_UID_destroy: ' + str(self.sn))
            IP_UID_destroy(self.uut)
            self.uut=None
            self.final_result= IP_FAIL
            self.sn = None

    #context manager protocol
    __enter__ = start
    def __exit__(self, t, v, tb):
        ret = False
        if isinstance(v, InstantPuddingError):
            self.reply_error = v
            ret = True
        self.done()
        self.commit()
        self.destroy()
        return ret
    
    def __repr__(self):
        return '\n'.join(['%s:%s' % (k,v) for k,v in self.attributes.items()])

    @IP_reply_assert
    def add_attribute(self, attr_name, attr_value):
        check_pdca_len(attr_name, 'attribute name', PDCA_ATTRIBUTE_NAME_MAX_LEN)
        check_pdca_len(attr_value, 'attribute value', PDCA_ATTRIBUTE_VALUE_MAX_LEN)
        self.attributes[attr_name] = attr_value
        self.logger.debug('calling IP_addAttribute ' + attr_name + ':' + attr_value)
        return IP_addAttribute(self.uut, str(attr_name), str(attr_value)) #name and value could be unicode, then it doesn't translate to char*
        
    @IP_reply_assert
    def cancel(self):
        self.logger.debug('calling IP_UUTCancel')
        return IP_UUTCancel(self.uut)
        
    def done(self):
        self.logger.debug('calling IP_UUTDone sn:' + str(self.sn))
        return IP_UUTDone(self.uut)
        
    def commit(self, pass_fail=None):
        if not pass_fail:
            pass_fail = self.final_result
        if self.sn is None:
            raise InstantPuddingError('trying to commit without an SN')
        self.logger.debug('calling IP_UUTCommit sn:' + str(self.sn) + '; result:' + str(pass_fail))
        return IP_UUTCommit(self.uut, pass_fail)
        
    @IP_reply_assert
    def add_result(self, spec, result):
        if result._result==IP_FAIL:
            if result._message==None:
                raise InstantPuddingError('when result is fail, you must set a fail message')
        self.logger.debug('calling IP_addResult result:' + str(spec) + ' for spec:' + str(spec))
        return IP_addResult(self.uut, spec.spec, result.test_result)
    
    @IP_reply_assert
    def add_blob_file(self, blob_name, blob_file_path):
        self.logger.debug('calling IP_addBlob name:' + blob_name + '; file:' + blob_file_path)
        return IP_addBlob(self.uut, str(blob_name), str(blob_file_path))

    @IP_reply_assert
    def set_start_time(self, raw_time):
        '''
        input must be a floating value representing seconds from the epoch, the value returned by time.time
        '''
        self.logger.debug('calling IP_setStartTime for ' + str(self.sn))
        return IP_setStartTime(self.uut, raw_time)

    @IP_reply_assert
    def set_stop_time(self, raw_time):
        self.logger.debug('calling IP_setStopTime for ' + str(self.sn))
        return IP_setStopTime(self.uut, raw_time)

    #this is not decorated by IP_reply_assert because we are hanlding the error checking ourselves
    #and this function is not working with the InstantPudding library I got
    def get_ghstation_info(self, info_type):
        reply, info = IP_getGHStationInfo(self.uut, info_type)
        if IP_success(reply):
            return info
        else:
            raise InstantPuddingError('Error calling IP_getGHStationInfo: ' + str(IPReply(reply)))

    @IP_reply_assert
    def set_DUT_position(self, fixture_id, fixture_head_id):
        self.logger.debug('calling IP_setDUTPosition sn:' + str(self.sn) + '; fixture_id:' + str(fixture_id) + '; head_id:' + str(fixture_head_id))
        return IP_setDUTPos(self.uut, str(fixture_id), str(fixture_head_id))

    def amIOkay(self):
        if self.sn: #only check am I Okay if we have a serial number
            reply = IP_amIOkay(self.uut, bytes(self.sn)+'\0')
            succeed = IP_success(reply)
            if not succeed:
                raise InstantPuddingError(str(IPReply(reply)))

    def __del__(self):
        self.destroy()
        
class IPTestSpec(object):

    def __init__(self, test_name, subtest_name=None, limits=None, unit=None, priority=IP_PRIORITY_REALTIME, subsubtest_name=None):
        self.spec = IP_testSpec_create()
        self.set_test_name(test_name)
        self._subtest_name = None
        self._subsubtest_name = None
        if limits:
            self.limits = limits
        else:
            self._limits = 'NA'
        self.priority = priority
        if unit:
            self.set_unit(unit)
        else:
            self._unit = 'NA'
        if subtest_name:
            self.set_subtest_name(subtest_name)
        if subsubtest_name:
            self.set_subsubtest_name(subsubtest_name)

            
    def __repr__(self):
        ret = self._test_name
        if self._subtest_name:
            ret = ret + '::' + str(self._subtest_name)
        if self._subsubtest_name:
            ret = ret + '::' + str(self._subsubtest_name)
        ret = ret +  '; limits:' + str(self._limits) + '; unit:' + self._unit + '; priority ' + str(self._priority)
        return ret
       
    def __del__(self):
        IP_testSpec_destroy(self.spec)
        
    @API_check_assert
    def set_test_name(self, test_name):
        self._test_name = str(test_name)
        check_pdca_len(self._test_name, 'test name', PDCA_TEST_NAME_MAX_LEN)
        return IP_testSpec_setTestName(self.spec, self._test_name)
    test_name = property(fset=set_test_name)
    
    @API_check_assert
    def set_subtest_name(self, subtest_name):
        self._subtest_name = str(subtest_name)
        check_pdca_len(self._subtest_name, 'subtest name', PDCA_SUBTEST_NAME_MANX_LEN)
        return IP_testSpec_setSubTestName(self.spec, self._subtest_name)
    subtest_name = property(fset=set_subtest_name)
    
    @API_check_assert
    def set_subsubtest_name(self, subsubtest_name):
        self._subsubtest_name = str(subsubtest_name)
        check_pdca_len(self._subsubtest_name, 'subsubtest name', PDCA_SUBSUBTEST_NAME_MANX_LEN)
        return IP_testSpec_setSubSubTestName(self.spec, self._subsubtest_name)
    subsubtest_name = property(fset=set_subsubtest_name)
    
    @API_check_assert
    def set_limits(self, limits):
        str_low = str(limits['low_limit'])
        str_high = str(limits['high_limit'])
        check_pdca_numberic(str_low)
        check_pdca_numberic(str_high)
        self._limits = limits
        return IP_testSpec_setLimits(self.spec, str_low, str_high)
    limits = property(fset=set_limits)
    
    @API_check_assert
    def set_priority(self, priority):
        self._priority = priority
        return IP_testSpec_setPriority(self.spec, priority)
    priority = property(fset=set_priority)
    
    @API_check_assert
    def set_unit(self, unit):
        self._unit = str(unit)
        check_pdca_len(self._unit, 'unit', PDCA_UNIT_MAX_LEN)
        return IP_testSpec_setUnits(self.spec, self._unit)
    unit = property(fset=set_unit)
    
    @API_check_assert
    def set_as_non_test_item(self):
        return IP_testSpec_setAsNonTestItem(self.spec)
        
class IPTestResult(object):
    def __init__(self, result, value=None, message=None):
        self.test_result = IP_testResult_create()
        self._message = None
        self.set_result(result)
        if message:
            self.set_message(message)
        if value:
            self.set_value(value)

        self._value = str(value)

    def __del__(self):
        IP_testResult_destroy(self.test_result)
        
    @API_check_assert
    def set_result(self, result):
        self._result = result
        if result == IP_FAIL:
            if self._message is None:  # not a fail message
                self._message = 'FAIL' # we give the fail result a default fail message
        return IP_testResult_setResult(self.test_result, self._result)
    result=property(fset=set_result)
    
    @API_check_assert
    def set_message(self, msg):
        self._message = str(msg)
        check_pdca_len(self._message, 'fail message', PDCA_FAIL_MSG_MAX_LEN)
        return IP_testResult_setMessage(self.test_result, str(self._message))
    message = property(fset=set_message)
        
    @API_check_assert
    def set_value(self, value):
        self._value = value
        check_pdca_numberic(str(value))
        return IP_testResult_setValue(self.test_result, str(value))
    value = property(fset=set_value)      

class IPGHStation(object):
    
    def __init__(self):
        [reply, self.uut] = IP_UUTStart()
        IP_reply_destroy(reply)
        
    def __getitem__(self, info_type):
        reply, info = IP_getGHStationInfo(self.uut, info_type)
        if IP_success(reply):
            if info[-1]=='\0':
                info = info[:-1]
            return info
        else:
            raise InstantPuddingError('Error calling IP_getGHStationInfo: ' + str(IPReply(reply)))      
    
    def __del__(self): 
        try:
            IP_UUTCancel(self.uut)
            IP_UID_destroy(self.uut)
        except Exception, e:
            pass #IP_UUTCancel may become null at the end of main function exit
        
class DummyIPUUT(object):
    '''
    Dummy IPUUT implementation to help with unit testing
    '''
    class Action(object):
        '''
        helper object to record the msgs sent to objects of class DummyIPUUT
        '''
        def __init__(self, msg, *args):
            self.msg_type=DummyIPUUT.MESSAGES[msg.strip().lower()]
            self.args = args
        
        def __eq__(self, other):
            if self.msg_type == other.msg_type and self.args==other.args:
                return True
            else:
                return False
        
        def __ne__(self, other):
            return not self.__eq__(other)
        
    def __getattr__(self, name):
        self.current_action = DummyIPUUT.Action(name)
        
    def record(self, *args):
        self.current_action.args = args
        return IPReply(0)
    
    def __init__(self, sn, logger_name='iTest.pudding'):
        self.logger_name = logger_name
        self.sn = sn
        self.final_result = IP_FAIL
        self.called_actions = []
        self.current_action = None
        
    def start(self):
        self.called_actions = []
        self.called_actions.append(DummyIPUUT.Action('start'))
    
    def destroy(self):
        self.called_actions.append(DummyIPUUT.Action('destroy'))
        self.final_result= IP_FAIL
        self.sn = None

    #context manager protocol
    __enter__ = start
    def __exit__(self, t, v, tb):
        ret = False
        if isinstance(v, InstantPuddingError):
            self.reply_error = v
            ret = True
        self.done()
        self.commit()
        self.destroy()
        return ret
    
    def __repr__(self):
        return ''

    #this is not decorated by IP_reply_assert because we are hanlding the error checking ourselves
    #and this function is not working with the InstantPudding library I got
    def get_ghstation_info(self, info_type):
        self.called_actions.append(DummyIPUUT.Action('destroy', info_type))
        return 'test info'

    def __del__(self):
        self.destroy()
        
if __name__=='__main__':
    logger = logging.getLogger('iTest.pudding')
    logger.setLevel(logging.DEBUG)
    logger.addHandler(logging.StreamHandler())
    
    print 'instantPudding version:' + INSTANT_PUDDING_VERSION
    
    gh_station = IPGHStation()
    print 'station number: ' + str(gh_station[IP_STATION_NUMBER])
    print 'station line number: ' + str(gh_station[IP_LINE_NUMBER])
    print 'station mac address: ' + str(gh_station[IP_MAC])
    print 'station spider cap ip:' + str(gh_station[IP_SPIDERCAB_IP])
    del gh_station

    
    uut = IPUUT('YM123456789')
    uut.start()
    uut.add_attribute(IP_ATTRIBUTE_STATIONSOFTWARENAME, "PyPudding")
    uut.add_attribute(IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  '1.0')

    #pass fail test passing:
    spec = IPTestSpec('PassingTest')
    result = IPTestResult(IP_PASS)
    uut.add_result(spec, result)

    #pass fail test failing
    try:
        spec = IPTestSpec('FailingTest')
        result = IPTestResult(IP_FAIL)
        uut.add_result(spec, result)
    except InstantPuddingError as e:
        print '=====Planted error:' + str(e)

    #parametric test pass
    spec = IPTestSpec('Parametric data test', subtest_name='pass_test', limits={'low_limit':0, 'high_limit':9}, unit='ly')
    result = IPTestResult(IP_PASS, 4)
    uut.add_result(spec, result)

    #parametric test fail
    spec = IPTestSpec('Parametric data test', subtest_name='fail_test', limits={'low_limit':0, 'high_limit':9}, unit='ly')
    result = IPTestResult(IP_FAIL, '', 'value out of range')
    uut.add_result(spec, result)
    uut.add_attribute('VBOOST_LEFT', '0x01')
    uut.add_blob_file('myself', 'pypudding.py')
    
    uut.done()
    uut.commit(IP_PASS)
    del uut
    exit()

    uut2 = IPUUT('YM123456788')
    #with IPUUT('YM123456788') as uut2:
    with uut2:
        uut2.add_attribute(IP_ATTRIBUTE_STATIONSOFTWARENAME, "PyPudding")
        uut2.add_attribute(IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  '1.0')
        uut2.final_result = IP_PASS
        uut2.set_DUT_position(IP_FIXTURE_ID_2, IP_FIXTURE_HEAD_ID_3)
    if uut2.reply_error:
        print uut2.reply_error

    uut3 = IPUUT('YM123456787')
    with uut3:
        uut3.add_attribute(IP_ATTRIBUTE_STATIONSOFTWARENAME, "PyPudding")
        uut3.add_attribute(IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  '1.0')
        uut3.final_result = IP_PASS
        uut3.set_start_time(0)
        uut3.set_stop_time(2)
        uut3.amIOkay()
    if uut3.reply_error:
        print uut3.reply_error
