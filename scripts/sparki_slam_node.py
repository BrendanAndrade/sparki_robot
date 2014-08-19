#!/usr/bin/env python

## @file
#
#
#

## @author Brendan Andrade

import sys

import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Range

from sparki_robot.sparki import Sparki

##
#
#
class SparkiSLAMNode(object):

    ##
    #
    #
    def __init__(self, name, port):
        
        self._port = port
        self.sparki = Sparki(port)
        self.range_pub = rospy.Publisher('ranges', Range)
        self.twist_sub = rospy.Subscriber('cmd_vel', Twist, self.twistCB)
        
        self.sparki.addSensorCallback('range', self.rangeCB)

    ##
    #
    #
    def begin(self):
        self.sparki.begin()

    def shutdown(self):
        self.sparki.shutdown()

    ##
    #
    #
    def twistCB(self, data):
        v = data.linear.x
        omega = data.angular.z
        self.sparki.sendMotorVO(v, omega)

    ##
    #
    #
    def rangeCB(self, data):
        range_msg = Range()
        range_msg.header.stamp = rospy.Time.now()
        range_msg.radiation_type = Range.ULTRASOUND
        range_msg.range = float(data[1])
        range_msg.field_of_view = float(data[0])
        self.range_pub.publish(range_msg)

def main(port):
    rospy.init_node('sparki', anonymous=True)
    name = rospy.get_name()
    
    sparki_slam_node = SparkiSLAMNode(name, port)
    rospy.spin()
    rospy.on_shutdown(sparki_slam_node.shutdown())

if __name__ == "__main__":
    args = rospy.myargv(argv=sys.argv)
    main(args[1])
