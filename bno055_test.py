# bno055_test.py Simple test program for MicroPython bno055 driver

# Copyright (c) Peter Hinch 2019
# Released under the MIT licence.
import os
import machine
import time
from bno055 import *

i2c = machine.I2C(0, sda=machine.Pin(0), scl=machine.Pin(1))  # EIO error almost immediately

imu = BNO055(i2c)

calibrated = False
try:
    with open('calibrated_data.txt', 'rb') as f:
        
        data_calib = f.read()
        imu.set_offsets(data_calib)
        calibrated = imu.calibrated()
        print(calibrated)
        f.close()
except:
    calibrated = False
    
    
    
while True:
    time.sleep(0.1)
    print('Heading  {:4.2f} roll {:4.2f} pitch {:4.2f}'.format(*imu.euler()))



