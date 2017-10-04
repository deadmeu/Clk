import serial

#(device, baud, bytesize, parity checking, stopbits
ser = serial.Serial(None, 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)

values = bytearray([4, 9, 62, 144, 56, 30, 147, 3, 210, 89, 111, 78, 184, 151, 17, 129])
ser.write(values)
