import mysql.connector
from mysql.connector import errorcode
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

#cod luat de pe github.com/sanpingz/mysql-connector

config = {
    'user':  'root',
    'password': 'parola',
    'host': '127.0.0.1',
    'database': 'musipi'
}

class RoomsNumberNotSupported(Exception):
    pass

class RoomIdNotExisting(Exception):
    pass

def ConvertToBinary(decNo):
    if decNo < 4:
        print("{} = {},{}".format(decNo, decNo //2, decNo%2))
        GPIO.output(17, decNo // 2)
        GPIO.output(27, decNo % 2)    
    else:
        raise RoomsNumberNotSupported

conn = cursor = None
try:
    conn = mysql.connector.connect(**config)
    
except mysql.connector.Error as err:
    if err.errno == errorcode.ER_BAD_DB_ERROR:
        print('The required database does not exist')
    elif err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print('The access has been denied')
    else:
        print('Error: ', err)
else:
    doorsList = []
    while(1):
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM doors;")
        doorsList = list(cursor.fetchall())
        for door in doorsList:
            try:
                ConvertToBinary(door[3])
                charToSend = '?'
                serialConnection.write(charToSend.encode())
                charReceived = serialConnection.read()
                #I should be receiving 1/2
                print(charReceived)
                if charReceived == b'1':
                #the user crossed from room_1 to room_2
                    print(door[1], door[2])
                    cursor.execute("UPDATE rooms SET rooms.people = rooms.people + 1 WHERE rooms.id = {}".format(door[1]))
                    cursor.execute("UPDATE rooms SET rooms.people = rooms.people - 1 WHERE rooms.id = {}".format(door[2]))
                    conn.commit()
                elif charReceived == b'2':
                #the user crossed from room_2 to room_1
                    print(door[1], door[2])
                    cursor.execute("UPDATE rooms SET rooms.people = rooms.people - 1 WHERE rooms.id = {}".format(door[1]))
                    cursor.execute("UPDATE rooms SET rooms.people = rooms.people + 1 WHERE rooms.id = {}".format(door[2]))
                    conn.commit()
                cursor.execute("SELECT active FROM rooms WHERE rooms.id = {}".format(door[1]))
                currentRoom = cursor.fetchall()
                if cursor.rowcount == 0:
                    raise RoomIdNotExisting
                currentRoomActiveStatus = cursor.fetchone()
                if currentRoomActiveStatus == 1:
                    #send 'o' for keeping the speaker opened
                    charToSend = 'o'
                    serialConnection.write(charToSend.encode())
                elif currentRoomActiveStatus == 0:
                    #send 'c' for closing the speaker
                    charToSend = 'c'
                    serialConnection.write(charToSend.encode())
                charReceived = serialConnection.read()
                #I should be receiving 'k' for ok
                print(charReceived)
                
            except RoomsNumberNotSupported:
                print('The physical system does not support more than 4 rooms. Please purchase additional equipment for an upgrade.')
            except RoomIdNotExisting:
                print('Requested room id cannot be found in the database.')
        time.sleep(0.1)            
       
finally:
    if cursor:
        cursor.close()
    if conn:
        conn.close()