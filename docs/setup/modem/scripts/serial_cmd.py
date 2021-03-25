import time
import serial

PORT = "/dev/ttyS4"


def read(ser: serial.Serial, end: str, timeout: int):
    if isinstance(end, str):
        end = end.encode()

    string = []
    final_chars = len(end)
    timeout = time.time() + timeout
    while time.time() <= timeout:
        result = ser.read()
        if result:
            string.append(result)
            if b"".join(string[-final_chars:]) == end:
                break
    return b"".join(string)


with serial.Serial(PORT, 115200, timeout=1) as ser:
    while True:
        comm = input(">> ") + "\r"
        ser.write(comm.encode())
        response = read(ser, b"OK", 5)
        if response:
            print(response.decode())
        else:
            print("---No response---")
