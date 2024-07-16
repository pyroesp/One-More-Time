# One-More-Time
One More Time is a VFD clock with six IV-22 displays.
Based of an ATmega4808, an MP3 player module, and optionally an ESP32.  

# Features
The clock shows time in HHMMSS 24h format.  
When the alarm goes off it should play an MP3 from a mini MP3 payer module.  

There are 7 alarms that can be configured independently, for each day of the week.  

# VFD driver
![driver](/img/vfd_driver.jpg)  
![display](/img/vfd_disp_0.jpg)  

# Buttons
The available buttons are: alarm, hour, minute and snooze.  
Long press is used to enter a mode. Short presses are used to change the value.  

I've also added a hold state and a hold super speed state. (These would trigger short presses.)
You can use the buttons independently or in combination.  

# Modes
## Change alarm 
Long press the alarm button to enter the change alarm mode.  
In this mode, use the hour button to increase the hour and use the minute button to increase the minute.  
Use the alarm button to turn on or off the alarm, indicated by the dot on the VFD.
Lastly, use the snooze button to change the alarm weekday.

## Change hour
Long press the hour button to enter the change hour mode.  
In this mode, use the hour button to change the hours of the clock.  

## Change minute
Same as change hour but with the minute button.  

## Change day of week
Long press the snooze button to enter the change day of week mode.  
Use short presses to change the weekday so that the alarms are applied to the correct day of the week.  

## Exit modes
To exit any mode, release all buttons and wait for the timeout to return to the clock display.  

# Testing on windows
Due to how this was programmed, the clock code can be tested on windows.  
See the windows folder for a main file where you can test the different features with A, Z, E and R keys. 

# Audio
The alarm is a cheap mp3 player module with a 2W speaker.  
You can find them on aliexpress.  

These are controlled through UART.  

# ESP32
The ESP32 is optional.  

First use would be to sync with NTP servers.  
Another option would be to use it's webserver capabilities to change the time and alarms from a browser.  

# Dependencies
This code only uses standard libraries and the XC8/AVR standard headers.  

# License
This is licensed under [Creative Commons CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).  
