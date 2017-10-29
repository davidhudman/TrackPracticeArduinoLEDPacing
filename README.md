# Track Practice LED Pacing #

This system is used for pacing track athletes. See our track workout functionality below: 

[![](https://img.youtube.com/vi/brJRkPgrA1k/0.jpg)](https://www.youtube.com/watch?v=brJRkPgrA1k) 

Some of our basic features [here](https://www.youtube.com/watch?v=GPfT1HgIodE) as well.

This project provides a way for lights to pace runners as they go around a running track. It allows for users to send inputs (paces - like a 3:59 mile) to the lights through their smartphones.

[![](https://github.com/davidhudman/TrackPracticeArduinoLEDPacing/blob/master/Screenshots/TrackPracticeSS02.JPG)](https://github.com/davidhudman/TrackPracticeArduinoLEDPacing)

--------
### Access and Control ###

* Input and output can be sent and viewed by the user through the mobile web app. With the Arduino Yun setup, the web app is hosted on the Yun and made available to the user when connected to the shared wifi network - much like a splash page when connecting to Starbucks wifi.

## Other Input Methods ##
* If you have the bluetooth chip, input/output can be sent/received through an app that is no longer on the Google Play Store. It appears that it can be downloaded from [here](http://arduino-bluetooth-terminal.soft112.com/), but that link has not been verified.
* I've build some nodeJS functionality to talk over the serial ports (USB) and a wired connection to the Arduino. It uses the serialport library and the servi library.

--------

Version: 1.1

This setup requires the following items:

* Arduino Yun
* Micro SD card
* Password-protected Wifi network that you can connect to
* WS2801 Addressable RGB LEDs (or WS2812 or WS2811, it's possible that more will work too)

Version: 1.0

This setup requires the following items:

* Arduino Micro
* HC-05 Bluetooth Module - [Setup Explained](http://www.instructables.com/id/Arduino-AND-Bluetooth-HC-05-Connecting-easily/)
* WS2801 Addressable RGB LEDs (or WS2812 or WS2811, it's possible that more will work too)

---------

### End User Instructions ###

Create your pacer through an intuitive web app.

-------------

### How do I get set up? ###

* The Arduino Yun has its own special .ino file to handle wireless input, and this is the preferred method. (For other Arduino boards, the standard .ino file is uploaded via your USB port to the Arduino)
* The database is a simple Pacer.db text file that uses SQLite configuration and is located in the Arduino Yun file system with the other PHP files that run queries on it.
* In order for the web app to work, the HTML files must be made available to the user by putting the files on the Arduino Yun file system in a publically accessible folder.


### Who do I talk to for help? ###

* Create an issue in the "Issues" tab above.