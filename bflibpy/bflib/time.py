# author: Brando
# date: 6/5/23

from datetime import datetime

class Time:

    def __init__(self, string):
        """
        string: HH:MM:SS.ffffff
        """
        self._datetime = datetime.strptime(string, '%H:%M:%S.%f')

    def epoch(self):
        return self._datetime.timestamp()

def elapsedtime(before, after):
    result = 0 

    a = Time(after)
    b = Time(before)

    result = a.epoch() - b.epoch()

    return result;

