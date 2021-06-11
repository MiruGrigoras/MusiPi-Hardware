import time
import serial
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)

serialConnection = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)


def ConvertToBinary(decNo):
    print("{} = {},{}".format(decNo, decNo //2, decNo%2))
    GPIO.output(17, decNo // 2)
    GPIO.output(27, decNo % 2)   

while(1):
    ConvertToBinary(1);
    time.sleep(1);
    ConvertToBinary(2);
    time.sleep(1);
    

