# Base Source Code Directory

This directory contains the code that will be uploaded to the central device.

## Configuration

To set up the config file, run

```shell
cp config_example.py config_secret.py
```

and edit the details in config_secret.py.
**Do not** edit the `config_example.py` file because it is not being gitignored.

The `config_example.py` file has variable named `email_recipients`.
It is a dict containing the addresses to send emails to. Mapped to each address is a dict which contains:

- a PIN, which is used to verify users when they respond to an email
