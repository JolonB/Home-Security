# Modem Setup

## Requirements

- Raspberry Pi
- Sixfab Cellular IoT HAT
- LTE u.fl antenna
- Micro USB cable
- SIM card (Twilio, Hologram, local IoT compatible SIM card)

## Instructions

### Hardware Setup

1. Inset the SIM card into the IoT HAT. The slot on the HAT is for a micro SIM.
1. Connect the antenna. Ensure the LTE antenna is connected to the LTE connector. If you have a GPS antenna, connect that to the GPS connector.
1. Attach the HAT to the Pi. You may want to use standoffs and longer headers to keep it from touching the board.
1. Connect the USB cable to the Pi and the HAT. The micro-USB side connects to the HAT.

### Software Setup

1. Start raspi-config: `sudo raspi-config` and go to Interface Options > Serial Port.
1. When it says "Would you like a login shell to be accessible over serial?", select No. When it says "Would you like the serial port hardware to be enabled?", select Yes.
1. Leave raspi-config. It will ask if you want to reboot. Select Yes. If it does not ask,reboot the device with `reboot`.

### SIM Card Setup

1. Create and log in to an account on [sixfab connect](https://connect.sixfab.com). Note: the user name cannot contain any capital letters.
1. Go to SIM > Register a SIM.
1. Enter the SIM's ICCID. This number can be found below the barcode on the SIM packaging or on two lines on the back of the SIM itself.
1. Add payment details in Billing > Payment Details > Add a Payment Method. Sixfab will need to charge US$10 to your account which will be added to your balance.

1. Go to the [Twilio Buy a Number page](https://www.twilio.com/console/phone-numbers/search). You may need to create an account if this hasn't been done yet.

### Driver Setup

1. Power on the HAT by pressing the PWRKEY button. The STAT LED will light up when the board is powered.
1. Run `modem_setup.sh`. This will install everything needed and move you to the appropriate directory.
1. Run `python3 configureCATM1.py`. Look at the response to the AT+COPS? command. If it is +COPS: 0, then you should retry the script or run `python configureGPRS.py` instead. A good response from AT+COPS? will include the network name. It is better to use CATM1, so try different locations for better signal if you can.

### Connecting to Modem from PC

If you want to connect to the modem from a PC, you need to:

1. Connect the 5V and Vbus jumpers to allow power through the USB.
1. Connect the USB cable to your computer (or to the Raspberry Pi).
1. Press the `PWRKEY` button and wait for the modem to boot.
1. Three new serial connections will appear. Connect to the Quectel USB AT Port (this is probably the lowest numbered port) with `picocom -b 115200 /dev/ttyX` where `X` is the port number.
1. Type `ATE1` and press enter to turn on command echo.
1. Send any commands that you need to send.
1. To exit picocom, press `Ctrl+A` `Ctrl+X`.

You can also connect to the modem using QNavigator from Quectel.

### Testing SMS Capabilities

If you want to use SMS and you are developing this system, you may want to manually test your SIM card (at least until I make a script to automate it).
You must know the number for the SIM card, which should be given to you when the SIM card is set up.

You can do test this by connecting to the modem in whatever way you like and:

1. Send `AT+COPS?` to verify you are connected to the network.
1. Send `AT+CMGF=1` to set the message format to text, or `AT+CMGF=0` to set it to PDU (this will be unreadable).
1. Send `AT+CMGL="ALL"` (text) or `AT+CMGL=4` (PDU) to view all of the messages.

<!-- TODO find how to send SMS maybe even try using skinny sim -->

### Shutdown

The modem can be shutdown with `AT+QPOWD`.
The module will output `POWERED DOWN` to indicate that the shutdown procedure has finished.
It is best to wait 1 second after the output to remove power.
If no signal is received after 65 seconds, the power can be disconnected.
