#!/usr/bin/env python
import time
import serial
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)
counter = 0
try:
    while 1:
        if counter < 5:
            print('1:')
            GPIO.output(17, 0)
            GPIO.output(27, 0)
            w = 'a'
        else:                  
            print('2:')
            GPIO.output(17, 1)
            GPIO.output(27, 0)
            w = 'b';
        #GPIO.output(17, 1)
        #GPIO.output(27, 0)
        w = chr(ord('a')+counter)
        ser.write(w.encode())
        time.sleep(0.5) 
        counter += 1
        r = ser.read()
        print(r)
        time.sleep(0.5)
        if counter == 9:
            counter=0
except KeyboardInterrupt:
    GPIO.cleanup()