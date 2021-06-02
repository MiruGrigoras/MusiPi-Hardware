#!/usr/bin/env python
import time
import serial

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)
counter = 0
while 1:
    w = 'c'
    ser.write(w.encode())
    time.sleep(0.1)
    counter += 1
    r = ser.read()
    print(r)
    time.sleep(0.1) 