# Email Setup

## Prerequisites

- A new Gmail account. It should not be used for anything else.

## Instructions

The following instructions are not the most secure way to set up an email service and more secure instructions can be found [here](https://developers.google.com/gmail/api/quickstart/python) (though additional work will be required to get this working).
It is vital that you keep any information that you store in `config_secret.py` hidden from anyone.

1. In a browser that is logged into the new Gmail account, go to [this page](https://myaccount.google.com/lesssecureapps) and allow less secure apps.

<!-- OLD INSTRUCTIONS

Refer to [this API guide from Google](https://developers.google.com/gmail/api/quickstart/python) for getting set up.
Most instructions below will be copies of the instructions on that page but there will be important links on that page, so make sure you have it open.

1. On the guide, click the "Enable Gmail API" button in [Step 1](https://developers.google.com/gmail/api/quickstart/python#step_1_turn_on_the).
1. Name the project "HomeSec" and agree to the T&C's.
1. Select "Desktop app" for the OAuth client and click "Create".
1. Click "Download client configuration" and use the contents of the file as the `oauth_cred` parameter in `src/base_src/config_secret.py` (refer to [`src/base_src/README.md`](src/base_src/README.md) if this hasn't been set up yet). -->
