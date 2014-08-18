#!/usr/bin/env python

##@file
#
#
#

## @author Brendan Andrade

from twisted.internet.serialport import SerialPort
from twisted.protocols import basic

class SparkiProtocol(basic.LineReceiver):
    delimiter = '\n'
    _line_callback = None

    def setLineCallback(self, callback):
        self._line_callback = callback

    def lineReceived(self, line):
        self._line_callback(line)
        
