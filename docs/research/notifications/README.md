# Notifications

There needs to be a way to notify users if something has been detected.
This can be achieved with:

- app notification
- text message
- messenger (or similar service) bot
- email

The most important factors in the decision are *accessibility*, *development time*, *disabling*, and *price*.

Accessibility is the ability to receive the notification at any time.
Users should be informed of suspicious activity regardless of whether they have a connection to the internet.
The types of devices it is accessible on is also important.

Development time is how long it takes to get this set up for the first time. Ideally, I won't need to spend too long setting it up, and can spend more time working on the notifications themselves.

Disabling refers to the ability to disable the alert temporarily in the event of a false alarm.

Price is important as it determines how expensive the notifications will be.
In some cases, it may be the price of a monthly plan (or similar), rather than a per-notification price.

## App Notification

It is not very accessible as it requires the user to have an Internet connection to receive the notification, and will also need to be developed for lots of different devices (Android, iOS, laptops).  
This solution involves developing an app to configure the device, which will result in a higher development time.  
It would work great for disabling the security system remotely (and locally) as it could all be done from within the app.  
It is likely to be free.  

## Text Message

Only accessible to phones, although does not require an Internet connection.
Users may not notice the text message immediately if only one text is sent, though we could consider sending multiple messages or calling the user if that is possible.  
Development time is likely to be short.  
Can disable the alert by replying to the text.  
Will have to pay for a plan (monthly cost) or use the standard texting rate depending on service used.  

## Messenger Bot

Not very accessible as it requires an Internet connection, although it is very device agnostic.  
May require some initial development time.
May be more difficult to develop to make it work for different users.  
Disabling can be done with a call-to-action button, for example, it may say "Mute Notifications".  
May have a cost involved.  

## Email

Not very accessible as it requires an Internet connection, although it is very device agnostic.
Users also may not receive notifications at all.  
Development time is likely to be short (using SMTP).  
Can disable the alert by replying to the email.  
Free.

## Conclusions

Everything in the table below is ranked from 1 to 3, with 1 being the best and 3 being the worst.

| Decision Factors | | App | Text | Bot | Email |
| --- | --- | --- | --- | --- | --- |
| Accessibility | | 3 | 2 | 2 | 2 |
| Development Time | | 3 | 1 | 2 | 1 |
| Disabling | | 1 | 1 | 1 | 1 |
| Price | | 1 | 2 | ? | 1 |

Email appears to be the clear winner, however, the problem of not being accessible without an Internet connection is a big one.
The best course of action is to send a text message and an email as that will provide the most accessibility while keeping the development time and price low.
