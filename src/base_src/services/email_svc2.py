import pickle
import os.path
import base64
import mimetypes
# googleapiclient docs here: https://googleapis.github.io/google-api-python-client/docs/epy/index.html
from googleapiclient import errors
from googleapiclient.discovery import build
from googleapiclient.discovery import Resource
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.image import MIMEImage
from email.mime.base import MIMEBase
from email.mime.audio import MIMEAudio

# If modifying these scopes, delete the file token.pickle. https://developers.google.com/gmail/api/auth/scopes
# The modify scope gives access to all read/write operations except deleting files without sending to trash.
SCOPES = ['https://www.googleapis.com/auth/gmail.modify']

def get_service():
    """Shows basic usage of the Gmail API.
    Lists the user's Gmail labels.
    """
    creds = None
    # The file token.pickle stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
        # Save the credentials for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)
    #
    service = build('gmail', 'v1', credentials=creds)
    #
    return service

def create_message(
    sender, to, subject, message_text, file=None):
  """Create a message for an email. # TODO test this fully

  Args:
    sender: Email address of the sender.
    to: Email address of the receiver.
    subject: The subject of the email message.
    message_text: The text of the email message.
    file: The path to the file to be attached.

  Returns:
    An object containing a base64url encoded email object.
  """
  message = MIMEMultipart()
  message['to'] = to
  message['from'] = sender
  message['subject'] = subject

  msg = MIMEText(message_text)
  message.attach(msg)

  if file:
    content_type, encoding = mimetypes.guess_type(file)

    if content_type is None or encoding is not None:
      content_type = 'application/octet-stream'
    main_type, sub_type = content_type.split('/', 1)
    if main_type == 'text':
      fp = open(file, 'r')
      msg = MIMEText(fp.read(), _subtype=sub_type)
      fp.close()
    elif main_type == 'image':
      fp = open(file, 'rb')
      msg = MIMEImage(fp.read(), _subtype=sub_type)
      fp.close()
    elif main_type == 'audio':
      fp = open(file, 'rb')
      msg = MIMEAudio(fp.read(), _subtype=sub_type)
      fp.close()
    else:
      fp = open(file, 'rb')
      msg = MIMEBase(main_type, sub_type)
      msg.set_payload(fp.read())
      fp.close()
    filename = os.path.basename(file)
    msg.add_header('Content-Disposition', 'attachment', filename=filename)
    message.attach(msg)

  return {'raw': base64.urlsafe_b64encode(message.as_bytes()).decode()}

def send_message(service, user_id, message):
  """Send an email message.

  Args:
    service: Authorized Gmail API service instance.
    user_id: User's email address. The special value "me"
    can be used to indicate the authenticated user.
    message: Message to be sent.

  Returns:
    Sent Message.
  """
  try:
    message = service.users().messages().send(userId=user_id, body=message).execute()
    print('Message Id: %s' % message['id'])
    return message
  except errors.HttpError as error:
    print('An error occurred: %s' % error)

def check_inbox(service:Resource, only_unread:bool=False) -> dict:
  """Finds all messages addressed to the security system.

  Args:
      service (Resource): Authorised Gmail API service instance.
      only_unread (bool, optional): Whether to only show unread messages. Defaults to False.

  Returns:
      dict: The threads found in the inbox. Is formatted as {'threads': [], 'resultSizeEstimate': n}. If there are no threads, 'threads' will not exist.
  """
  query = "to:me"
  if only_unread:
    query += " is:unread"
  return service.users().threads().list(userId='me',q="to:me").execute()

def read_threads(service:Resource, threads:list):
  for thread in threads:
    thread_data = service.users.threads().get(userId='me', id=thread['id']).execute()

  pass

if __name__ == '__main__':
    serv = main()
    msg = create_message("jolon.behrent.smtp.test.69420@gmail.com", "jolon.behrent@gmail.com", "test body", "this is a test message")
    send_message(serv, "me", msg)
    # main()



""" Here is a example thread. The subject line is "test body", the message sent by the service is "this is a test message", and the reply is "This is my response"
  {'id': '175fe58945c74cb1', 'historyId': '7448', 'messages': [{'id': '175fe58945c74cb1', 'threadId': '175fe58945c74cb1', 'labelIds': ['UNREAD', 'SENT'], 'snippet': 'this is a test message', 'payload': {'partId': '', 'mimeType': 'multipart/mixed', 'filename': '', 'headers': [{'name': 'Received', 'value': 'from 592253985866 named unknown by gmailapi.google.com with HTTPREST; Wed, 25 Nov 2020 02:40:19 -0500'}, {'name': 'Content-Type', 'value': 'multipart/mixed; boundary="===============6056899260745066414=="'}, {'name': 'MIME-Version', 'value': '1.0'}, {'name': 
'to', 'value': 'jolon.behrent@gmail.com'}, {'name': 'From', 'value': 'jolon.behrent.test.smtp.69420@gmail.com'}, {'name': 'subject', 'value': 'test body'}, {'name': 'Date', 'value': 'Wed, 25 Nov 2020 02:40:19 -0500'}, {'name': 'Message-Id', 'value': '<CAKZXe3yC8r-4XoNNQvciS1vbaWAixRimxMPLhW_zXrHb1fxT8A@mail.gmail.com>'}], 'body': {'size': 0}, 'parts': [{'partId': '0', 'mimeType': 'text/plain', 'filename': '', 'headers': [{'name': 'Content-Type', 'value': 'text/plain; charset="us-ascii"'}, {'name': 'MIME-Version', 'value': '1.0'}, {'name': 'Content-Transfer-Encoding', 'value': '7bit'}], 'body': {'size': 22, 'data': 'dGhpcyBpcyBhIHRlc3QgbWVzc2FnZQ=='}}, {'partId': '1', 'mimeType': 'image/jpeg', 'filename': 'penthouse.jpg', 'headers': [{'name': 'Content-Type', 'value': 'image/jpeg'}, {'name': 'MIME-Version', 'value': '1.0'}, {'name': 'Content-Transfer-Encoding', 'value': 'base64'}, {'name': 'Content-Disposition', 'value': 'attachment; filename="penthouse.jpg"'}], 'body': {'attachmentId': 'ANGjdJ-VorS4bIuG0-SEw_6JV9bETmDZJ4Hb_fv-3H1XV42K52tHL20Q6F2HF-aV7DXskQ45NfSMyZJikMaFXc614TTEL7D6tmwgToBDTw2ZnNvoRjg9c9aFwKJrd3KCw4FeWsaYo7R84x4wHxteM4zLct50QJNU4zq6vClmuGszHBHLPGZBuEO23cpOu51Olqf9H-t8z8HyAXAipYwaj_5MJ8SSYPzkXkt9MajaCkk7i2jo0SXFjxsVDf1UU1Bf1xHo7wHrDNpDrilIy1fLQU4dba6uBPaYvR7h7GpOQFaGEB9Nt3c4FcTKIewWvlQ-bTH3UO0I5jszFT3VPCSmmxlP7E37V1E1lIRKiLTdSmzfoMGuRTcJlc_UHXPFEVOh80n5zbIi-Vj4nq9hLA4Z', 'size': 221397}}]}, 'sizeEstimate': 299895, 'historyId': '7448', 'internalDate': '1606290019000'}, {'id': '175fe6f4fa7b23ec', 'threadId': '175fe58945c74cb1', 'labelIds': ['UNREAD', 'IMPORTANT', 'CATEGORY_PERSONAL', 'INBOX'], 'snippet': 'This is my response On Wed, 25 Nov 2020, 8:40 PM , &lt;jolon.behrent.test.smtp.69420@gmail.com&gt; wrote: this is a test message', 'payload': {'partId': '', 'mimeType': 'multipart/alternative', 'filename': '', 'headers': [{'name': 'Delivered-To', 'value': 'jolon.behrent.test.smtp.69420@gmail.com'}, {'name': 'Received', 'value': 'by 2002:ab3:5d92:0:0:0:0:0 with SMTP id v18csp280999ltc;        Wed, 25 Nov 2020 00:05:09 -0800 (PST)'}, {'name': 'X-Received', 'value': 'by 2002:a05:6602:2d49:: with SMTP id d9mr1651612iow.109.1606291509217;        Wed, 25 Nov 2020 00:05:09 -0800 (PST)'}, {'name': 'ARC-Seal', 'value': 'i=1; a=rsa-sha256; t=1606291509; cv=none;        d=google.com; s=arc-20160816;        b=LkQki5Ke1hsVKh+eLtJR6LRVjsIERVGiwp7rZLdOFoz3mJfS1z1au3kBTC+3Gm+jVT         y++NtVOCElKdEBt21cL0ivJMSr4INo5Zrjsvw/q7L3tzIPylkBscUsJfVwJBk7wYahlD         4rpC70Nril2ChLHnbClWdIh2mUCYOwZd9hb3D+GJTva2Owe+vQKLnB0BH1FOUlUt0kvs         bhqBRcu0S5PBu/bCUiJrecBtLIv4Jl8xDhhLU6bNPPnnPw5RfFS/hPczjy3JjQu3St0K         K0T7hVyEj4wCbGFiNpNbNYW61TGailHq/dBUlpHD5TG8aTxMfp3kmJhSdbayM8700yzw         N2DA=='}, {'name': 'ARC-Message-Signature', 'value': 'i=1; a=rsa-sha256; c=relaxed/relaxed; d=google.com; s=arc-20160816;        h=to:subject:message-id:date:from:in-reply-to:references:mime-version         :dkim-signature;        bh=1DIcUObMOSsCiJKrlnbO0PvtNkWdxuq2AbAbjbfacmU=;        b=in1sWCgPV+FH69wR7IYlX+Xhxabr2UEUs5XGH9O0QMk9nnbxrSBqRrtcfstAhyj1cs         IriJDNfRkGnXvngTy3hRWWymQqoXeJJvkhuYK9YgDjX/6YHQZXAIBxPBTxfsPdMEcKV6         uchv4/8mvw4AytAdDitKo5NlIjlt5EzZlrcPRL1ViuMc+hHnEiqcaA/4qaWK/SGjGpOi         bCyn72wZk3AeknltJGXuEK7qKfnjZF2UF4pRVlNwsu4cTIaRuG0kRJLc93mUl2EvcIdM         Pq/smtyNvixpkvNiykE5kBwwWe8pfMFknKSOa4WLRX7BCIBCf4GWUiroj/rBmX7EgqFF         9DRA=='}, {'name': 'ARC-Authentication-Results', 'value': 'i=1; mx.google.com;       dkim=pass header.i=@gmail.com header.s=20161025 header.b=n5bjVAqS;       spf=pass (google.com: domain of jolon.behrent@gmail.com designates 209.85.220.41 as permitted sender) smtp.mailfrom=jolon.behrent@gmail.com;       dmarc=pass (p=NONE sp=QUARANTINE dis=NONE) header.from=gmail.com'}, {'name': 'Return-Path', 'value': '<jolon.behrent@gmail.com>'}, {'name': 'Received', 'value': 'from mail-sor-f41.google.com (mail-sor-f41.google.com. [209.85.220.41])   
     by mx.google.com with SMTPS id q10sor626771iow.33.2020.11.25.00.05.09        for <jolon.behrent.test.smtp.69420@gmail.com>        (Google Transport Security);        Wed, 25 Nov 2020 00:05:09 -0800 (PST)'}, {'name': 'Received-SPF', 'value': 'pass (google.com: domain of jolon.behrent@gmail.com designates 209.85.220.41 as permitted sender) client-ip=209.85.220.41;'}, {'name': 'Authentication-Results', 'value': 'mx.google.com;       dkim=pass header.i=@gmail.com header.s=20161025 header.b=n5bjVAqS;       spf=pass (google.com: domain of jolon.behrent@gmail.com designates 209.85.220.41 as permitted sender) smtp.mailfrom=jolon.behrent@gmail.com;       dmarc=pass (p=NONE sp=QUARANTINE dis=NONE) header.from=gmail.com'}, {'name': 'DKIM-Signature', 'value': 'v=1; a=rsa-sha256; c=relaxed/relaxed;        d=gmail.com; s=20161025;        h=mime-version:references:in-reply-to:from:date:message-id:subject:to;        bh=1DIcUObMOSsCiJKrlnbO0PvtNkWdxuq2AbAbjbfacmU=;        b=n5bjVAqSAlNop1Z5vHBrxLIRIJ1MqHQaqCVInmqNls6DVfsTRVC+/C+h7jk4vkOLmc         urv0JwU1appM2AX2+QAkPg5yrFEtQRulRASI8e/xbRKMHKyrpUNcP0NRBsStx15ybz8z        
 e+IsVxSMkPYKhdtuaYv1p+YSyamK8X8gGmLw7ang2jcJXGVZukBBUh5s1WgCsho+0vOc         KcbWXn3GurjkNDXJimOb08LsINzaCLPRDGdzPunb9eJEhztYCdNCR/46yJS3PDh7eNRm         Bs2sf6uvjjflom9r04nheygSXd3RrQ6fdt0v8Y7+b3UY0+33+6S9OjxxcSo3SF5pv8Cd         X0Dg=='}, {'name': 'X-Google-DKIM-Signature', 'value': 'v=1; a=rsa-sha256; c=relaxed/relaxed;        d=1e100.net; s=20161025;        h=x-gm-message-state:mime-version:references:in-reply-to:from:date         :message-id:subject:to;        bh=1DIcUObMOSsCiJKrlnbO0PvtNkWdxuq2AbAbjbfacmU=;        b=aILNtKeLX8fd5Zg82PW4bcUIhYCI43Iqn5CmM9naXUfIr6DGayVutWZ7pILZLJzIJ2         lHom33nD8+9YqCvD/GYvhR/V1JrvFcP2PO8DyU0QwXsgg4FTVm39l0pDcJFTJvwBjhuE         nLG28OCGmrmWNOeQvAjDoEUK/6V2XGeZzmww7Tt80sHJ45LVy1xDkV72Tyf4EGOvI51z         LouiiT9pNGBCMqccTS13GJ18xf3HrvnQg4ANxFL6dFK7jW4RKsbZTd/6YpWy7IXG7vH7         78jvLI7Eev8ziHzVe4VdnHFWd/wxl2Vuo4B0qvNpSWZ6NS4RxxEL23QssR84+BRHgm5W         rvgA=='}, {'name': 'X-Gm-Message-State', 'value': 'AOAM53124Tl970bCi6vvPbVtp2nHdTtFVjjL+sLytpUUR6iYxU8sOVa1 ivyKtK0yBcjs+l5YRko00MTnJH3aLpppvF8zETI+enMSScY='}, {'name': 'X-Google-Smtp-Source', 'value': 'ABdhPJyT+v+Khv8feWTjA/oN4LKNYG9J6saDI7PbIiddMCoR/+DOPom2SnABCfvN3d+lSv1gX/lhNbz9HtIHeva5SV0='}, {'name': 'X-Received', 'value': 'by 2002:a5d:9c47:: with SMTP id 7mr1738634iof.112.1606291508591; Wed, 25 Nov 2020 00:05:08 -0800 (PST)'}, {'name': 'MIME-Version', 'value': '1.0'}, {'name': 'References', 'value': '<CAKZXe3yC8r-4XoNNQvciS1vbaWAixRimxMPLhW_zXrHb1fxT8A@mail.gmail.com>'}, {'name': 'In-Reply-To', 'value': '<CAKZXe3yC8r-4XoNNQvciS1vbaWAixRimxMPLhW_zXrHb1fxT8A@mail.gmail.com>'}, {'name': 'From', 'value': 'Jolon Behrent <jolon.behrent@gmail.com>'}, {'name': 'Date', 'value': 'Wed, 25 Nov 2020 21:04:56 +1300'}, {'name': 'Message-ID', 'value': '<CAFApG4bgPPCEcesTZ5yz8XsbSZ1uhQaeQ6jPthtj6DQ1osp=Vw@mail.gmail.com>'}, {'name': 'Subject', 'value': 'Re: test body'}, {'name': 'To', 'value': 'jolon.behrent.test.smtp.69420@gmail.com'}, {'name': 'Content-Type', 'value': 'multipart/alternative; boundary="000000000000c67e0a05b4e9e2ed"'}], 'body': {'size': 0}, 'parts': [{'partId': '0', 'mimeType': 'text/plain', 'filename': '', 'headers': [{'name': 'Content-Type', 'value': 'text/plain; charset="UTF-8"'}], 'body': {'size': 136, 'data': 'VGhpcyBpcyBteSByZXNwb25zZQ0KDQpPbiBXZWQsIDI1IE5vdiAyMDIwLCA4OjQwIFBNICwgPGpvbG9uLmJlaHJlbnQudGVzdC5zbXRwLjY5NDIwQGdtYWlsLmNvbT4NCndyb3RlOg0KDQo-IHRoaXMgaXMgYSB0ZXN0IG1lc3NhZ2UNCj4NCg=='}}, {'partId': '1', 'mimeType': 'text/html', 'filename': '', 'headers': [{'name': 'Content-Type', 'value': 'text/html; charset="UTF-8"'}, {'name': 'Content-Transfer-Encoding', 'value': 'quoted-printable'}], 'body': {'size': 411, 'data': 'PGRpdiBkaXI9ImF1dG8iPlRoaXMgaXMgbXkgcmVzcG9uc2U8L2Rpdj48YnI-PGRpdiBjbGFzcz0iZ21haWxfcXVvdGUiPjxkaXYgZGlyPSJsdHIiIGNsYXNzPSJnbWFpbF9hdHRyIj5PbiBXZWQsIDI1IE5vdiAyMDIwLCA4OjQwIFBNICwgJmx0OzxhIGhyZWY9Im1haWx0bzpqb2xvbi5iZWhyZW50LnRlc3Quc210cC42OTQyMEBnbWFpbC5jb20iPmpvbG9uLmJlaHJlbnQudGVzdC5zbXRwLjY5NDIwQGdtYWlsLmNvbTwvYT4mZ3Q7IHdyb3RlOjxicj48L2Rpdj48YmxvY2txdW90ZSBjbGFzcz0iZ21haWxfcXVvdGUiIHN0eWxlPSJtYXJnaW46MCAwIDAgLjhleDtib3JkZXItbGVmdDoxcHggI2NjYyBzb2xpZDtwYWRkaW5nLWxlZnQ6MWV4Ij50aGlzIGlzIGEgdGVzdCBtZXNzYWdlPGJyPg0KPC9ibG9ja3F1b3RlPjwvZGl2Pg0K'}}]}, 'sizeEstimate': 5625, 'historyId': '7448', 'internalDate': '1606291496000'}]}
  """

"""
VGhpcyBpcyBteSByZXNwb25zZQ0KDQpPbiBXZWQsIDI1IE5vdiAyMDIwLCA4OjQwIFBNICwgPGpvbG9uLmJlaHJlbnQudGVzdC5zbXRwLjY5NDIwQGdtYWlsLmNvbT4NCndyb3RlOg0KDQo-IHRoaXMgaXMgYSB0ZXN0IG1lc3NhZ2UNCj4NCg==
is base64 for
b'This is my response\r\n\r\nOn Wed, 25 Nov 2020, 8:40 PM , <jolon.behrent.test.smtp.69420@gmail.com>\r\nwrote:\r\n\r\n> this is a test message\r\n>\r\n'
"""