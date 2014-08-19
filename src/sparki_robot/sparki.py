#!/usr/bin/env python

##@file
#
#
#

## @author Brendan Andrade

from twisted.internet import reactor
from twisted.internet.serialport import SerialPort
from sparki_robot.sparki_twisted import SparkiProtocol

from threading import Thread

class Sparki(object):

    ##
    #
    #
    def __init__(self, port):

        self._sensor_observers = {'range': [None], 'accelerometer': [None], 'error': [self._message_error], 'overflow': [self._overflow_error]}
        self._sensor_codes = {'r': 'range', 'a': 'accelerometer', 'e': 'error', 'o': 'overflow'}

        self._port = port
        self._protocol = SparkiProtocol()
        self._protocol.setLineCallback(self._handleMessage)
        self._ser = SerialPort(self._protocol, self._port, reactor, baudrate=9600)
        self._reactor_thread = Thread(target = reactor.run, args=(False,))
        self._reactor_thread.daemon = True
        self._reactor_thread.start()

    ##
    #
    #
    def shutdown(self):
        reactor.stop()

    ##
    #
    #
    def send(self, message):
        try:
            self._ser.write(message)
        except:
            raise

    ##
    #
    #
    def sendMotorVO(self, lin_vel, ang_vel):
        message = "U V " + str(lin_vel) + " " + str(ang_vel) + "\n"
        self.send(message)

    ##
    #
    #
    def addSensorCallback(self, sensor, callback):
        self._sensor_observers[sensor].append(callback)

    
    ## 
    #
    #
    def _handleMessage(self, data):
        output = data.split()
        for observer in self._sensor_observers[self._sensor_codes[output[0]]]:
            if observer is not None:
                observer(output[1:])

    ##
    #
    #
    def _message_error(self):
        print "Submitted bad serial message to Sparki"

    ##
    #
    #
    def _overflow_error(self):
        print "Sparki receive buffer overflow"
