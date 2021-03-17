import logging
import serial
import time
import re
import config_running as conf
import RPi.GPIO as GPIO

# Initialise logger
log = logging.Logger()

# Initialise serial port
ser = serial.Serial()

# Declare GPIO pins
BG96_ENABLE = 17
BG96_POWERKEY = 24
RESET = 18
STATUS = 23
USER_BUTTON = 22
USER_LED = 27

# Default desired responses
OK = "OK\r\n"
ERROR = "ERROR\r\n"

def init():
    # Print out modem information:
    log.info()
    response = send_at_comm("ATI", OK)
    values = parse_response(response, "(?:(?i)ati\s+)(\S+)(?:\s)(\S+)(?:\s+)(?:\S+\:\s)(\S+)")
    log.info(f"Modem Model: {values[0]} {values[1]}\r\nRevision: {values[2]}")
    response = send_at_comm("ATI", OK)
    values = parse_response(response, "(?:(?i)at\+gsn\s+)(\d+)")
    log.info(f"IMEI: {values[0]}")
    # set RAT search sequence: Cat-M1->NB-IoT->GSM:
    send_at_comm("AT+QCFG=\"nwscanseq\",020301,1", OK) # TODO maybe just do GNSS mode
    # set RAT searching to auto:
    send_at_comm("AT+QCFG=\"nwscanmode\",0,1", OK)
    # set network category:
    send_at_comm("AT+QCFG=\"iotopmode\",2,1", OK)
    # set APN
    # TODO change APN based on response to 'AT+CRSM=176,28486,0,0,17'
    send_at_comm("AT+CGDCONT=1,\"IP\",\"super\"", OK)
    # reset transmitter
    send_at_comm("AT+CFUN=4", OK)
    send_at_comm("AT+CFUN=1", OK)
    pass

def deinit(): 
    GPIO.clearGPIOs()
    
def setupGPIO():
    GPIO.setmode(GPIO.BCM)
    #GPIO.setwarnings(False)
    GPIO.setup(BG96_ENABLE, GPIO.OUT)
    GPIO.setup(BG96_POWERKEY, GPIO.OUT)
    GPIO.setup(STATUS, GPIO.IN)
    GPIO.setup(USER_BUTTON, GPIO.IN)
    GPIO.setup(USER_LED, GPIO.OUT)

def send_msg(contents):
    pass

# Function for sending at comamand to module
def send_at_comm_once(command):
    if not ser.isOpen():
        ser.open()		
    compose = ""
    compose = str(command) + "\r"
    ser.reset_input_buffer()
    ser.write(compose.encode())
    log.info("Message sent {}:".format(compose))

def send_at_comm(command, desired_response, timeout = None):
        if timeout is None:
            timeout = timeout
        send_at_comm_once(command)
        f_debug = False
        timer = time.time()
        while 1:
            if time.time() - timer > timeout: 
                send_at_comm_once(command)
                timer = time.time()
                f_debug = False
            response =""
            while(ser.inWaiting()):
                try: 
                    response += ser.read(ser.inWaiting()).decode('utf-8', errors='ignore')
                    delay(100) # TODO make async
                except Exception as e:
                    log.exception(e.Message)
                # debug_print(response)	
            if(response.find(desired_response) != -1):
                log.info("Response received: {}".format(response))
                return response # returns the response of the command as string.

def parse_response(response, regex):
    m = re.match(regex, response)
    log.debug(f"Got \"{m}\" from searching for \"{repr(regex)}\" in \"{repr(response)}\"")
    return m.groups()

def shutdown():
    # send shutdown command
    ### AT+QPOWD
    # check for STATUS pin set low and URC "POWERED DOWN" and wait 1 more second, or after 65 second timeout passed
    #   fully shutdown, i.e. cut power (if possible)
    ...

# Function for enable BG96 module
def enable():
    GPIO.output(BG96_ENABLE,1)
    log.debug("BG96 module enabled!")

# Function for powering down BG96 module and all peripherals from voltage regulator 
def disable():
    GPIO.output(BG96_ENABLE,0)
    log.debug("BG96 module disabled!")

# Function for powering up or down BG96 module
def powerUp():
    GPIO.output(BG96_POWERKEY,1)
    time.sleep(1)
    GPIO.output(BG96_POWERKEY,0)
    time.sleep(2)
    log.debug("BG96 module powered up!")
    