from datetime import datetime

myvalue = 3

def func_from_plugin(params, *args, **kwargs):
    global myvalue
    myvalue = params[0]
    return int(params[0])*2

def exc_from_plugin(*args, **kwargs):
    raise Exception('exception from plugin')

def runtime(params, unit, timeout, sequence):
    if len(unit) == 0:
        raise Exception('Unit is missing')
    if len(params[0]) == 0:
        raise Exception('Param1(group) is missing')
    for group in sequence.groups:
        if group.name == params[0]:
            start = group.start_time
            stop = group.stop_time
            if start is None or stop is None:
                # raise Exception('start or stop time is None')
                return 0
            elif isinstance(start, datetime) and isinstance(stop, datetime):
                if start > stop:
                    raise Exception('time delta is minus')
                delta = stop - start
                if unit.lower() == 's':
                    return int(round(delta.total_seconds()))
                elif unit.lower() == 'ms':
                    return int(round(delta.total_seconds()*1000))
                elif unit.lower() == 'us':
                    return int(round(delta.total_seconds()*1000000))
                else:
                    raise Exception('Unit not recognized')
            else:
                raise Exception('start or stop time is not datetime instance')
    return 0
