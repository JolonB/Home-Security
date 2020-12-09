import time
import serial
import argparse
import RPi.GPIO as GPIO
from cellulariot import cellulariot

USER_BUTTON = 22
USER_LED = 27
BG96_ENABLE = 17
BG96_POWERKEY = 24
STATUS = 23
RESET = 18

# Cellular Modes
AUTO_MODE = 0
GSM_MODE = 1
CATM1_MODE = 2
CATNB1_MODE = 3

CTRL_Z = "\x1A"

parser = argparse.ArgumentParser()
parser.add_argument("--boot", dest="boot", action="store_true")
parser.add_argument("--gprs", dest="gprs", action="store_true")
parser.add_argument("--power_on", dest="pon", action="store_true")
parser.add_argument("--power_off", dest="poff", action="store_true")
parser.add_argument("command")

args = parser.parse_args()

ser = serial.Serial()


def setupGPIO():
    GPIO.setmode(GPIO.BCM)
    # GPIO.setwarnings(False)
    GPIO.setup(BG96_ENABLE, GPIO.OUT)
    GPIO.setup(BG96_POWERKEY, GPIO.OUT)
    GPIO.setup(STATUS, GPIO.IN)
    GPIO.setup(USER_BUTTON, GPIO.IN)
    GPIO.setup(USER_LED, GPIO.OUT)
    GPIO.setup(RESET, GPIO.OUT)

    ser.port = "/dev/ttyS0"
    ser.baudrate = 115200
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.bytesize = serial.EIGHTBITS


def get_on_status():
    return not GPIO.input(STATUS)


def wake_sleep(on: bool):
    if get_on_status() != on:
        GPIO.output(BG96_POWERKEY, 1)
        if on:
            while get_on_status() != on:
                ...
        else:
            time.sleep(1)
        GPIO.output(BG96_POWERKEY, 0)


def power(on: bool):
    if on:
        GPIO.output(BG96_POWERKEY, 0)
    else:
        GPIO.output(BG96_POWERKEY, 1)


def reset():
    GPIO.output(RESET, 0)
    time.sleep(0.25)  # must be a time between 150 and 460 ms
    GPIO.output(RESET, 1)


def write(cmd: str):
    print(f"Sending {cmd}")
    if ser.isOpen() == False:
        ser.open()
    compose = ""
    compose = str(cmd) + "\r"
    ser.reset_input_buffer()
    ser.write(compose.encode())


def read():
    if ser.isOpen() == False:
        ser.open()
    response = ""
    while ser.inWaiting():
        response += ser.read(ser.inWaiting()).decode("utf-8", errors="ignore")
        time.sleep(0.1)


if __name__ == "__main__":
    setupGPIO()

    if args.pon:
        power(on=True)

    write(args.command)

    time.sleep(5)

    print(read())

    if args.poff:
        power(on=False)

    GPIO.cleanup()
