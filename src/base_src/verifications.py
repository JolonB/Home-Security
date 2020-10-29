# Verify config files
# Should be used when first booting up (and maybe periodically while running) to ensure none of the config files are invalid

import config_const
import config_example as config_secret


def check_addr(email_addr: str) -> bool:
    return "@" in email_addr


def contains_keys(name: str, d: dict, *keys: tuple):
    for value, type_ in keys:
        assert value in d, "missing {} in `{}`".format(value, name)
        if type_:
            assert isinstance(d[value], type_), "{} in {} is not {}".format(
                d[value], name, type_
            )


def verify():
    # `email` checks
    contains_keys(
        "email", config_secret.email, ("address", str), ("password", str), ("port", int)
    )
    assert check_addr(config_secret.email["address"]), "address in `email` is not valid"

    # `email_recipients` checks
    for addr, key in config_secret.email_recipients.items():
        assert check_addr(addr), "address {} in `email_recipients` is not valid".format(
            addr
        )
        assert isinstance(
            key, int
        ), "key {} in `email_recipients` is not integer".format(key)

    # `mobile_recipients` checks
    for phone, key in config_secret.mobile_recipients.items():
        assert isinstance(
            phone, str
        ), "phone number {} in `mobile_recipients` is not string".format(phone)
        assert isinstance(
            key, int
        ), "key {} in `mobile_recipients` is not integer".format(key)

    # `user_info` checks
    for key, values in config_secret.user_info.items():
        assert isinstance(key, int), "key {} in `user_info` is not integer".format(key)
        contains_keys(
            "user_info[{}]".format(key),
            values,
            ("rank", int),
            ("pin", str),
            ("salt", str),
        )
        assert (
            values["rank"] in config_const.ranks.values()
        ), "rank {} in {} is not in config_const.ranks".format(values["rank"], values)

    # `two_fact_auth` checks
    assert isinstance(config_secret.two_fact_auth, bool)

    # `notif_kw` checks
    for key, value in config_secret.notif_kw.items():
        assert (
            value in config_const.keywords.values()
        ), "keyword {} in notif_kw is not in config_const.ranks".format(value)
