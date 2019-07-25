import serial


ser = serial.Serial(
    port='COM5',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0xFFFF)

print("connected to: " + ser.portstr)


while True:
    line = ser.readline()
    line = line.rstrip(b'\r\n')
    elements = line.decode("utf-8").split(';')
    print(elements)

ser.close()