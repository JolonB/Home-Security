import serial

ser = serial.Serial(
    "/dev/ttyS0",
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=None,
    xonxoff=False,
    rtscts=False,
    write_timeout=None,
    dsrdtr=False,
    inter_byte_timeout=None,
)  # everything after baudrate is default


def write(cmd):
    ser.write(f"{cmd}\r".encode())


def read():
    response = b""
    while ser.in_waiting:
        response += ser.read(ser.in_waiting)
    return response.decode()
