# Sending Notifications

There are three ways I am considering sending notifications (which are to be received via SMS and email):

- WiFi
- Ethernet
- LTE

WiFi and Ethernet essentially achieve the exact same thing (except Ethernet will be faster).
LTE will be beneficial as it allows alerts to be sent even in the event of a power failure (assuming that the device has a backup power supply).
The downside of LTE is that it will cost, so it could be better to use it as a backup, however, the cost of sending a text/email will be small and infrequent enough that there may not be much reason to worry about it (especially the text messaging, as that would require subscribing to another service to send it).  
Internet-based SMS services do exist (such as [BulkSMS](https://www.bulksms.com/pricing/)), and they are very affordable but will require an Internet connection, so an LTE solution would still be needed.
Due to the limited number of messages that will be sent, it is easier to simply use a modem for all messages, as the cost will be low regardless.
Therefore, it is probably best to send notifications over an LTE (or similar) modem.

## Modem and SIM

### SIM Card

#### Hologram

[Hologram SIM cards](https://www.hologram.io/) seem like a good choice for sending notifications.
They can typically connect to multiple mobile network providers and will use the one with the best coverage.
This has the added benefit of having it switch to a different network provider if one goes down.  
It is possible to pay for a monthly plan, but the infrequency of data transmission will mean that using one of their [flexible data plans](https://www.hologram.io/pricing/flexible-data) will be better.
The prices (likely in USD) for flexible data are as follows:

| | Price ($) |
| --- | --- |
| Monthly cost per SIM | 1.50 |
| Cost per MB of data | 0.40 |
| Cost per outbound SMS | 0.19 |
| Cost per SIM | 5.00 |

This equates to just $18 per year, assuming no messages are sent.

These can also be purchased from suppliers such as Mouser and Digikey, making them easier to buy with other parts (as in, you can get free/combined shipping this way).

#### Super SIM

[Super SIMs](https://www.twilio.com/iot/supersim) are a global SIM card which is [even cheaper](https://www.twilio.com/iot/supersim/pricing) than Hologram SIM cards (at least in New Zealand it is).
The prices are shown below:

| | Price ($) |
| --- | --- |
| Monthly cost per SIM | 2.00 |
| Cost per MB of data | 0.10 |
| Cost per outbound SMS | 0.05 |
| Cost per SIM | 3.00 |

There equates to $2.00 per year, assuming no messages are sent.

#### SIM Conclusion

It is clear that the Hologram SIM cards are cheaper over long-term use assuming not many messages are sent.
Regardless, it may be best to test both types of SIM card in order to evaluate which works the best.

### Modem

All of the modems listed below call themselves "worldwide" modules, so I expect that these will work anywhere.

#### Hologram Nova

There is a lot of documentation for using Hologram SIM cards with these modems.

##### R410

Uses Cat-M1 or NB-IoT so the speed of the data transmission is lower than usual (and coverage can be poor indoors).
NB-IoT is relatively new, so is unlikely to be phased out soon.
I know that some NB-IoT modems (like the Telit ME910C1, which I would not recommend using) *can* send text messages to phones.
This module is also cheaper than the U201.

##### U201

Uses 2G/3G which could be phased out in the coming years.
More expensive than the R410, but is more likely to be able to send SMS directly to a device.

#### Sixfab

##### Cellular IoT Hat

The Sixfab IoT uses the Quectel BG96 modems and is compatible with Raspberry Pi.
It can use Cat-M1, NB-IoT, or eGRPS.
Twilio provides a [lot of documentation](https://www.twilio.com/docs/iot/supersim/getting-started-super-sim-raspberry-pi-sixfab-cellular-iot-hat) for getting these set up with Super SIMs.

#### Modem Conclusion

Overall, I am inclined to work with the R410 as it is relatively inexpensive and was designed for Raspberry Pi (although, the Sixfab one also was), however, I have heard more about Hologram than Sixfab, which hopefully means greater support :crossed_fingers:.
I do not think it is worth selecting the U201 due to it only working on 2G/3G networks.
