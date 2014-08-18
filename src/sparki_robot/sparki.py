#!/usr/bin/env python

##@file
#
#
#

## @author Brendan Andrade

from twisted.internet import reactor
from twisted.internet.serialport import SerialPort
from sparki_robot.sparki_twisted import SparkiProtocol

class Sparki(object):

    ##
    #
    #
    def __init__(self, port):

        self._sensor_observers = {'range': [None], 'accelerometer': [None]}
        self._sensor_codes = {'r': 'range', 'a': 'accelerometer'}

        self._port = port
        self._protocol = SparkiProtocol()
        self._protocol.setLineCallback(self.handleMessage)
        self._ser = SerialPort(self._protocol, self._port, reactor, baudrate=9600)
        
    def begin(self):
        reactor.run()
        

        
    ##
    #
    #
    def close(self):
        self._ser.stop()
        reactor.stop()
        
    ##
    #
    #
    def send(self, message):
        try:
            self._ser.sendLine(message)
        except:
            raise

    ##
    #
    #
    def sendMotorVO(self, lin_vel, ang_vel):
        message = "U V " + str(lin_vel) + " " + str(ang_vel)
        self.send(message)

    ##
    #
    #
    def addSensorCallback(self, sensor, callback):
        self._sensor_observers[sensor].append(callback)

    
    ## 
    #
    #
    def handleMessage(self, data):
        output = data.split()
        for observer in self._sensor_observers[self._sensor_codes[output[0]]]:
            if observer is not None:
                observer(output[1:])

