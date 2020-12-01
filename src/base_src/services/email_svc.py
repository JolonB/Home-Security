from email.parser import BytesParser as Parser
import smtplib
import imaplib
import email
import ssl
import sys

sys.path.append(
    "."
)  # TODO there must be a better way to do this # pylint: disable=fixme
import config_secret


def send_alert():
    # TODO implement this # pylint: disable=fixme
    pass


def check_inbox():
    # TODO implement this # pylint: disable=fixme
    pass


def _format_message(subject: str, body: str):
    return "Subject: {}\n\n{}".format(subject, body)


def _get_email_config():
    return (
        config_secret.email["address"],
        config_secret.email["password"],
        config_secret.email["port"],
        config_secret.email_recipients,
    )


def _send_email(msg: str):
    # Get config information
    base_addr, password, port, recipients = _get_email_config()

    context = ssl.create_default_context()

    with smtplib.SMTP_SSL("smtp.gmail.com", port, context=context) as server:
        server.login(base_addr, password)
        server.sendmail(base_addr, recipients.keys(), msg)


def _gen_valid_recipients(recipients: list) -> str:
    """Recursively generate a list of recipients.

    Args:
        recipients (list): The recipients to format

    Returns:
        str: The valid recipients formatted for the mail.search function
    """
    len_rec = len(recipients)
    # Check base case
    if len_rec == 1:
        return '(FROM "{}")'.format(recipients[0])

    first_half = recipients[: len_rec // 2]
    second_half = recipients[len_rec // 2 :]
    return "(OR {} {})".format(
        _gen_valid_recipients(first_half), _gen_valid_recipients(second_half)
    )


def _parse_msg(parsedbytes: email.message) -> dict:
    msg_full = (
        parsedbytes.get_payload()[0].get_payload()
        if parsedbytes.is_multipart()
        else parsedbytes.get_payload()
    )
    msg_full = msg_full.split("\r\n", 1)
    return {
        "from": parsedbytes.get("From"),
        "to": parsedbytes.get("To"),
        "subject": parsedbytes.get("Subject"),
        "header": msg_full[0],
        "body": msg_full[1],
    }


def _rec_email():
    # Get config information
    base_addr, password, port, recipients = _get_email_config()

    mail = imaplib.IMAP4_SSL("imap.gmail.com")
    mail.login(base_addr, password)
    mail.select("inbox")

    result, data = mail.search(
        None, "UNSEEN", _gen_valid_recipients(list(recipients.keys()))
    )

    id_list = data[0].split()  # get list of email IDs

    # Return early if no new mail is found
    if not id_list:
        return None

    parser = Parser()

    responses = []
    for d in id_list:
        result, data = mail.fetch(d, "(RFC822)")
        parsedbytes = parser.parsebytes(data[0][1])
        responses.append(_parse_msg(parsedbytes))

    return responses

if __name__ == "__main__":
    _send_email("this is a test")