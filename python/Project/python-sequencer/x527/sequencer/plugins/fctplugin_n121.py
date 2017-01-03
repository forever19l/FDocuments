myvalue = 3


def func_from_plugin(params, *args, **kwargs):
    global myvalue
    myvalue = params[0]
    return int(params[0])*2


def exc_from_plugin(*args, **kwargs):
    raise Exception('exception from plugin')


def runtime(params, unit, timeout, sequence):
    from datetime import datetime
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


def init_variables(sequencer):
    try:
        import pypudding
    except ImportError:
        return False
    '''load default GH info into default variables. must be called after load_sequence and clear_variables'''
    gh = pypudding.IPGHStation()
    var = sequencer.sequence.variables
    product = gh[pypudding.IP_PRODUCT]
    var['product'] = gh[pypudding.IP_PRODUCT]
    if 'N121' in product.upper():
        product_gen = 'N121'
    elif 'X988' in product.upper() or 'N111' in product.upper():
        product_gen = 'N111'
    else:
        product_gen = 'UNKNOWN'
    var['product_gen'] = product_gen
    var['stationName'] = gh[pypudding.IP_STATION_TYPE]
    var['stationName_product_gen'] = var['stationName'] + '_' + product_gen
    var['fixturetype'] = 'PANELFIXTURE' if 'PREFCT' in var['stationName'] else 'SIPFIXTURE'
    var['fixturetype_product_gen'] = var['fixturetype'] + '_' + product_gen
    var['fixturename'] = product + '_' + var['fixturetype']
    var['unitStage'] = 'POSTSMT2' if var['stationName'] in ['SMT-QT0', 'COMBINE-FCT'] else 'PRESMT2'
    var['unitStage_product_gen'] = var['unitStage'] + '_' + product_gen
    var['isCalStation'] = 'YES' if var['stationName'] == 'SMT-QT0' else 'NO'
    var['isCalStation_product_gen'] = var['isCalStation'] + '_' + product_gen
    line_num = gh[pypudding.IP_LINE_NUMBER]
    var['line_number'] = line_num
    loc = gh[pypudding.IP_LOCATION]
    var['line_id'] = loc + '-' + line_num.split('/')[0]
    var['line_name'] = loc + '-' + line_num
    var['station_id'] = gh[pypudding.IP_STATION_ID]
    var['channel'] = str(sequencer.site + 1)
    return True
