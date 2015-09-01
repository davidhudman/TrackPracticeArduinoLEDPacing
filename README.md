# README #

(This README would normally document whatever steps are necessary to get your application up and running.)

### What is this repository for? ###

* Quick summary
* Version
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

This allows for lights to pace runners as they go around a running track. It allows for users to send inputs (paces - like 70 seconds per lap) to the lights through their smartphones. These are Microsoft Visual Studio project files, but the main one is the .ino file. This .ino file is uploaded via your USB port to the Arduino Micro.

--------
### Access and Control ###

Input and output can be sent and viewed by the user through the Arduino IDE's serial monitor. If you have the bluetooth chip, input/output can be sent/received through an app that is no longer on the Google Play Store. It appears that it can be downloaded from [here](http://arduino-bluetooth-terminal.soft112.com/), but that link has not been verified.

I am currently building a web interface with [this repository](https://bitbucket.org/davidhudman/nodetrackpractice/overview). It uses nodeJS to talk over the serial ports (USB) and a wired connection to the Arduino.

--------


Version: 1.0

This setup requires the following items:

* Arduino Micro
* HC-05 Bluetooth Module - [Setup Explained](http://www.instructables.com/id/Arduino-AND-Bluetooth-HC-05-Connecting-easily/)
* WS2801 Addressable RGB LEDs (or WS2812 or WS2811, it's possible that more will work too)

### End User Instructions ###

Commands:

| Command       | Desc       | Example |
| ------------- |:-------------| -----:|
| "c"           | **C**lear all pacers that are currently running |  |
| "r"           | **R**eset all pacers that are currently running and start them immediately      |  |
| "rd"          | (**R**eset **D**elay) reset all pacers that are currently running but wait a predetermined amount of time before starting them     |  |
| "b"           | (**B**ackwards) set all pacers to run the opposite direction that the first pacer is currently running (including the first pacer)     |  |
| "(any floating point or whole number)"          | sets a new pacer to run at x seconds per lap, which in this case is 75 seconds |    75 or 68.72 |
| "c(any whole number)"          | **C**lears a currently running pacer at a certain index off the track; this certain index is 2, so it clears pacer 2; |    "c2" |
| "r(any whole number)"          | **R**esets the pacer at a certain index to run immediately; this certain index is 1, so it resets pacer 1 |   "r1" |
| "rd(any whole number)"          | **R**esets all pacers to run following the **D**elay defined by the number on the end of the text; In this case, the number on the end of the text is 10, so all currently running pacers will be immediately removed from the track and will start following a 10 second delay |  "rd10" |
| "rdp(any whole number)"          | **R**esets following a predetermined **D**elay a **P**acer at a certain index to run; In this case, the index is 4, so pacer 4 will be immediately removed from the track and will start following a predetermined delay |    "rdp4" |
| "b(any whole number)"          | set a pacer at a specific index to run the opposite direction (**B**ackwards) that the it is currently running; this certain index is 3, so it will make pacer 3 run in the opposite direction | "b3" |
| "l(any whole number)"          | set the number of **L**ights that are on the track based on the whole number on the end of the text; In this case, the number of lights will be set to 45 | "l45" |
| "spt(any whole number)"         | Set the **S**peed of the **P**acer **T**o whatever speed that you tell it to; so the example would set the pacer at index 1 to 60 seconds per lap, whether it was currently running or not. If it was currently running, it would be as if it just sped up or slowed down to the new pace | "spt1,60" |
| "a(any whole number)"          | set clockAdjustmentFactor to the number of deci-seconds that you give it (so 10 in the example will be setting it to 1.0). This is basically not used anymore and is only necessary if you have timing issues. | "a10" |


* "c" - **C**lear all pacers that are currently running

* "r" - **R**eset all pacers that are currently running and start them immediately

* "rd" - (**R**eset **D**elay) reset all pacers that are currently running but wait a predetermined amount of time before starting them

* "b" - (**B**ackwards) set all pacers to run the opposite direction that the first pacer is currently running (including the first pacer)

* "(any floating point or whole number)"; Example: "75"; sets a new pacer to run at x seconds per lap, which in this case is 75 seconds

* "c(any whole number)": Example: "c2"; **C**lears a currently running pacer at a certain index off the track; this certain index is 2, so it clears pacer 2;*

* "r(any whole number)": Example: "r1"; **R**esets the pacer at a certain index to run immediately; this certain index is 1, so it resets pacer 1*

* "rd(any whole number)": Example: "rd10"; **R**esets all pacers to run following the **D**elay defined by the number on the end of the text; In this case, the number on the end of the text is 10, so all currently running pacers will be immediately removed from the track and will start following a 10 second delay

* "rdp(any whole number)": Example: "rdp4"; **R**esets following a predetermined **D**elay a **P**acer at a certain index to run; In this case, the index is 4, so pacer 4 will be immediately removed from the track and will start following a predetermined delay*

* "b(any whole number)": Example: "b3"; set a pacer at a specific index to run the opposite direction (**B**ackwards) that the it is currently running; this certain index is 3, so it will make pacer 3 run in the opposite direction*;

* "l(any whole number)": Example: "l45"; set the number of **L**ights that are on the track based on the whole number on the end of the text; In this case, the number of lights will be set to 45

* "spt(any whole number)": Example: "spt1,60"; Set the **S**peed of the **P**acer **T**o whatever speed that you tell it to; so the example would set the pacer at index 1 to 60 seconds per lap, whether it was currently running or not. If it was currently running, it would be as if it just sped up or slowed down to the new pace

* "a(any whole number)": Example: "a10"; set clock**A**djustmentFactor to the number of deci-seconds that you give it (so 10 in the example will be setting it to 1.0). This is basically not used anymore and is only necessary if you have timing issues.

*(keep in mind that the index of the first pacer is 0)


### How do I get set up? ###

* Summary of set up
* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact