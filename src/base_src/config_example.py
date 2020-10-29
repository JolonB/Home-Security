# The address to send notifications from

from config_const import ranks, keywords

# Credentials for email sending service
email = {
    "address": "name@gmail.com",
    "password": "password",
    "port": 465,  # this can normally be left at 465
}

# The addresses to send notifications to and their security codes
email_recipients = {
    "name1@address1.com": 1,
    "name2@address2.com": 2,
}
# The mobile numbers to send notifications to
mobile_recipients = {
    "number1": 1,
    "number2": 2,
}

# The information about registered users. The key maps to values in email_/mobile_recipients
user_info = {
    1: {"rank": ranks["ADMIN"], "pin": "1234", "salt": "put some salt here"},
    2: {"rank": ranks["USER"], "pin": "5678", "salt": "put different salt here"},
}
# Two factor authentication. Set to True if enabled
two_fact_auth = True

# Notification keywords. Change the keys if you want to provide some security through obscurity
notif_kw = {
    "DISABLE": keywords["DISABLE"],
    "ENABLE": keywords["ENABLE"],
    "MUTE": keywords["MUTE"],
}
