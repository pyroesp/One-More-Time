# One-More-Time
One More Time is a VFD clock for IV-22 displays.

# Project
This is a 6 VFD display clock.  
Based of an ATmega4808, an MP3 player module, and optionally an ESP32.  

# Features
The clock shows time in HHMMSS 24h format.  
It has a couple different "states":  
- Change hour : changes the hours  
- Change minute : changes the minutes  
- Change alarm : allows for changing alarm time and, enable or disable them  
- Change day of week : allows the user to change the day of week  
- Show date : Shows the date on the display  

# Dependencies
This code is only dependent of standard libraries.  

# Testing on windows
Due to how this was programmed, the clock code can be tested on windows.  
See the windows folder for a main file where you can test the different features with A, Z, E and R keys.

# Buttons
Selecting one of the afore mentioned modes is done through buttons.  
Long press is used to enter a mode. Short presses are used to change the value.  

I've also added a hold state and a hold super speed state.  
These would trigger short presses.  

The available buttons are:
- alarm
- hour
- minute
- snooze

You can use them independently or in combination.  
Long presses are used to enter a different clock "mode".  

# Audio
The alarm is a cheap mp3 player module with a 2W speaker.  
You can find them on aliexpress.  

These are controlled through UART.  

# ESP32
The ESP32 is optional.  

First use would be to sync with NTP servers.  
Another option would be to use it's webserver capabilities to change the time and alarms from, for ex., a phone.  

# License
This is licensed under [Creative Commons CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).  
