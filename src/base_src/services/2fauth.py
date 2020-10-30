# 2-factor authentication

import time
import hmac
import pyotp
import base64
import hashlib

ENCODING = "UTF-8"


def gen_token(user_info: dict) -> str:
    secret = "{}{}{}".format(user_info["pin"], user_info["rank"], user_info["salt"])
    hashed = (
        hmac.new(secret.encode(ENCODING), digestmod=hashlib.sha1)
        .digest()
        .hex()
        .encode(ENCODING)
    )
    return base64.b32encode(hashed)[:12].decode(ENCODING)


def verify(user_info: dict, code: str) -> bool:
    token = gen_token(user_info)
    totp = pyotp.TOTP(token)

    if code in [totp.now(), totp.at(time.time() - 30)]:
        return True
    return False
