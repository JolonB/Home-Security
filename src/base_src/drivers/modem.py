import logging
import config_running as conf

log = logging.Logger()

class Modem():
    # Declare GPIO pins
    BG96_ENABLE = 17
    BG96_POWERKEY = 24
    RESET = 18
    STATUS = 23
    USER_BUTTON = 22
    USER_LED = 27

    def __init__():
        # print out modem information:
        ### ATI
        ### AT+GSN
        # set RAT search sequence: Cat-M1->NB-IoT->GSM:
        ### AT+QCFG="nwscanseq",020301,1
        # set RAT searching: to auto
        ### AT+QCFG="nwscanmode",0,1
        # set network category:
        ### AT+QCFG="iotopmode",2,1
        # set APN
        ### something with CGDCONT and "super" as apn
        # reset transmitter
        ### something with CFUN
        pass

    # Function for sending at comamand to module
	def sendATCommOnce(self, command):
		if (ser.isOpen() == False):
			ser.open()		
		self.compose = ""
		self.compose = str(command) + "\r"
		ser.reset_input_buffer()
		ser.write(self.compose.encode())
		#debug_print(self.compose)

    def sendATComm(self, command, desired_response, timeout = None):
            if timeout is None:
                timeout = self.timeout
            self.sendATCommOnce(command)
            f_debug = False
            timer = millis()
            while 1:
                if( millis() - timer > timeout): 
                    self.sendATCommOnce(command)
                    timer = millis()
                    f_debug = False
                self.response =""
                while(ser.inWaiting()):
                    try: 
                        self.response += ser.read(ser.inWaiting()).decode('utf-8', errors='ignore')
                        delay(100)
                    except Exception as e:
                        debug_print(e.Message)
                    # debug_print(self.response)	
                if(self.response.find(desired_response) != -1):
                    debug_print(self.response)
                    return self.response # returns the response of the command as string.
                    break
    
    def shutdown(self):
        # send shutdown command
        ### AT+QPOWD
        # check for STATUS pin set low and URC "POWERED DOWN" and wait 1 more second, or after 65 second timeout passed
        #   fully shutdown, i.e. cut power (if possible)
        ...