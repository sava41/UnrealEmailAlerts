# Unreal Editor Email Alerts

## Description
 Unreal engine plugin to send editor notifications via email. Notifications that get sent are filterable through a UI
 ![image](/Resources/ScreenshotUI.png)

 ## Usage
 This plugin uses the [Twilio Sendgrid API](https://www.twilio.com/sendgrid/email-api). You will need to create an account and link an email address to use the plugin. 
 Add the api key and sender email by creating the file `\Source\EmailAlerts\Public\Twilio.h`. File contents should look something like this:
 ```
 #pragma once
constexpr auto TWILIO_EMAIL = "SenderAccount@example.com";
constexpr auto SENDGRID_API_KEY = "23h34berghkh43jk34hjh34jk5h3j4k";
```

Compile the plugin and you should by ready to go :) The plugin tab can be opened through the Window menu

Sendgrid pricing isnt great so if someone wants to help me set up a custom email server somehow for free or a low cost, please hit me up
