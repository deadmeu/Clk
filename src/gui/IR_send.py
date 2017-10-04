import serial
import time

#(device, baud, bytesize, parity checking, stopbits
ser = serial.Serial('COM5', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
v1 = bytearray((2863311530).to_bytes(4, 'big', signed=False))
v2 = bytearray([170, 171])

while(1):
    
    ser.write(v1)
    print("sent")
    time.sleep(0.1)
