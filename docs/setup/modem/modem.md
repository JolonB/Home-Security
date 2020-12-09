# Modem Setup

## Requirements

- Raspberry Pi
- Sixfab Cellular IoT HAT
- LTE u.fl antenna
- Micro USB cable
- Twilio Super SIM

## Instructions

### Hardware Setup

1. Inset the SIM card into the IoT HAT. The slot on the HAT is for a micro SIM.
2. Connect the antenna. Ensure the LTE antenna is connected to the LTE connector. If you have a GPS antenna, connect that to the GPS connector.
3. Attach the HAT to the Pi. You may want to use standoffs and longer headers to keep it from touching the board.
4. Connect the USB cable to the Pi and the HAT. The micro-USB side connects to the HAT.

### Software Setup

1. Start raspi-config: `sudo raspi-config` and go to Interface Options > Serial Port.
2. When it says "Would you like a login shell to be accessible over serial?", select No. When it says "Would you like the serial port hardware to be enabled?", select Yes.
3. Leave raspi-config. It will ask if you want to reboot. Select Yes. If it does not ask,reboot the device with `reboot`.

### SIM Card Setup

1. Create and log in to an account on [sixfab connect](connect.sixfab.com). Note: the user name cannot contain any capital letters.
2. Go to SIM > Register a SIM.
3. Enter the SIM's ICCID. This number can be found below the barcode on the SIM packaging or on two lines on the back of the SIM itself.
4. Add payment details in Billing > Payment Details > Add a Payment Method. Sixfab will need to charge US$10 to your account which will be added to your balance.

### Driver Setup

1. Power on the HAT by pressing the PWRKEY button. The STAT LED will light up when the board is powered.
2. Run `modem_setup.sh`. This will install everything needed and move you to the appropriate directory.
3. Run `python3 configureCATM1.py`. Look at the response to the AT+COPS? command. If it is +COPS: 0, then you should retry the script or run `python configureGPRS.py` instead. A good response from AT+COPS? will include the network name. It is better to use CATM1, so try different locations for better signal if you can.
