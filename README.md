# README #

### What is this for? ###

Pacing track athletes. See demo of [using the app in a workout](https://www.youtube.com/watch?v=brJRkPgrA1k) and of the [basic features](https://www.youtube.com/watch?v=GPfT1HgIodE)

This allows for lights to pace runners as they go around a running track. It allows for users to send inputs (paces - like 70 seconds per lap) to the lights through their smartphones. These are Microsoft Visual Studio project files, but the main one is the .ino file. This .ino file is uploaded via your USB port to the Arduino Micro.

--------
### Access and Control ###

* Input and output can be sent and viewed by the user through the user web app mobile web app. With the Arduino Yun setup, the web app is hosted on the Yun and made available to the user when connected to the shared wifi network - much like a splash page when connecting to Starbucks wifi.
* If you have the bluetooth chip, input/output can be sent/received through an app that is no longer on the Google Play Store. It appears that it can be downloaded from [here](http://arduino-bluetooth-terminal.soft112.com/), but that link has not been verified.

*I am currently building a web interface with [this repository](https://bitbucket.org/davidhudman/nodetrackpractice/overview). It uses nodeJS to talk over the serial ports (USB) and a wired connection to the Arduino. It uses the serialport library and the servi library.

--------


Version: 1.0

This setup requires the following items:

* Arduino Micro
* HC-05 Bluetooth Module - [Setup Explained](http://www.instructables.com/id/Arduino-AND-Bluetooth-HC-05-Connecting-easily/)
* WS2801 Addressable RGB LEDs (or WS2812 or WS2811, it's possible that more will work too)

Version: 1.1

This setup requires the following items:

* Arduino Yun
* Micro SD card
* Password-protected Wifi network that you can connect to
* WS2801 Addressable RGB LEDs (or WS2812 or WS2811, it's possible that more will work too)

---------

### End User Instructions ###

Create your pacer through an intuitive web app.

-------------

### How do I get set up? ###

* Summary of set up
* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

------------

### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact