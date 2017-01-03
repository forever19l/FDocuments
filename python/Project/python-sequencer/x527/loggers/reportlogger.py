from pypudding import InstantPuddingError
import traceback
import levels
import os
import events
import reporter

class DUT(object):
    def __init__(self, index):
        self.index = index
        self.sn = ''
        self.running_item = None

class ReportLogger(object):

    index = 0
    @staticmethod
    def next_index():
        ReportLogger.index += 1
        return ReportLogger.index

    def log_sequence_start(self, ts, site, report):
        return

    def log_item_start(self, ts, site, report):
        return

    def log_item_finish(self, ts, site, report):
        return

    def log_attribute_found(self, ts, site, report):
        return

    def log_sequence_end(self, ts, site, report):
        return

    def close(self):
        return


def call_next(aFunc):
    def chained_func(*args):
        ss = args[0]
        try:
            aFunc(*args)
        except Exception as e:
            error = e.message
            if not isinstance(error, InstantPuddingError):
                error = error + os.linesep + traceback.format_exc()
            ss.publisher.publish('error in logger: ' + error, level=levels.CRITICAL)
        if ss.next_logger is not None:
            try:
                next_func = ss.next_logger.__getattribute__(aFunc.__name__)
                next_func(*args[1:])
            except AttributeError:
                pass
    return chained_func

