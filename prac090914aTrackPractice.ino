/*	**TrackPracticeWS2801**

	This version of the TrackPractice Arduino program is designed to work with a strip of WS2801 lights.
	I orginally ordered a 50-light strip of these lights off amazon for $31 (http://www.amazon.com/gp/product/B00LYRI1KY/).
	A similar 25-light strip is also sold by Adafruit for $40 (https://www.adafruit.com/product/738).

	This program contains both the track pacing features and the "party" features

*/

#include <SoftwareSerial.h>
#include <Adafruit_WS2801.h>
#include <Time.h>
#include "LPD8806.h"
#include "SPI.h"

class Stopwatch{
private:
	long startTime;

public:
	Stopwatch() //(int constant_Delay_Seconds)
    {
    	//constantDelaySeconds = constant_Delay_Seconds;
    }
    
    void start()
    {
    	startTime = millis(); // + (constantDelaySeconds * 1000);
    }
    
	long getStartTime()
    {
    	return startTime;
    }
    
	long getEstimatedTime()
    {
    	return millis() - startTime;
    }
    
	short getMilliSeconds()
    {
    	return (short)((millis() - startTime) % 1000);
    }
    
	byte getSeconds()
    {
    	return (byte)(((millis() - startTime) / 1000) % 60);
    }
    
	byte getMinutes()
    {
    	return (byte)(((millis() - startTime) / 60000) % 60);
    }
	long getHours()
    {
    	return (millis() - startTime) / 3600000;
    }
};

/**
*	Pacer Class - public in java program
*	Holds all information about each individual pacer object (lap time, color, delay, starting postition, etc)
*/
class Pacer {
private:
	double initialDelay, 					//UI// How long each pacer should wait before starting
				secondsPerLap;				//UI// The number of seconds that it will take for the pacing panels to complete one lap
	int initialHighlightedPanel, 			//UI// This determines where the pacer starts from
				numMeters,						// This determines how many meters the pacing lights will run for
				totalPacingPanels,				// This is passed as an argument from JpanelPractice
				lightTrainLength;			//UI// This determines how many lights wide this pacer will be
	uint32_t shade;		//Color shade;		//UI// This determines the color of the pacer's pacing lights
	bool isStopwatchStarted;
	long startTime;
	static int numberPacers; // static
	int currentHighlightedPacingPanel;	// This determines which pacing panel is currently lit up

public:
	// variables
	
	// classes
	Pacer(double lapSecs, double initDelay, int firstHighlightedPanel, int meters, int light_Train_Length, int total_Pacing_Panels) // Constructor
	{
		Adafruit_WS2801 strip1 = Adafruit_WS2801();

		uint32_t color[7] = {Color(127,127,127), Color(127,0,0), Color(127,127,0), Color(0,127,0), Color(0,127,127), Color(0,0,127), Color(127,0,127)}; 
		// white, red, yellow, green, cyan, blue, magenta

		secondsPerLap = lapSecs;
		initialDelay = initDelay;
		currentHighlightedPacingPanel = firstHighlightedPanel;
		initialHighlightedPanel = firstHighlightedPanel;
		numMeters = meters;
		shade = color[numberPacers%7];
		lightTrainLength = light_Train_Length;
		isStopwatchStarted = false;
		totalPacingPanels = total_Pacing_Panels;
		numberPacers++;
		startTime = millis() + (long)initialDelay;
	};
	int getNumberPacers()
	{
		return numberPacers;
	};
	bool getIsStopwatchStarted()
	{
		return isStopwatchStarted;
	};
	bool isCurrentlyDelayed()
	{
		if ((startTime - millis()) > 0)
		{
			return true;
		}
		else return false;
	};
	bool isStartTimeWithinXSecondsAndGreaterThanZero(int seconds)
	{
		if (((startTime - millis()) > 0) && ((startTime - millis()) < (seconds*1000)))
		{
			return true;
		}
		else return false;
	};
	bool isStartTimeWithinXSecondsOnly(int seconds)
	{
		if ((startTime - millis()) < (seconds*1000))
		{
			return true;
		}
		else return false;
	};
	long getDelayRemaining()
	{
		return startTime - millis();
	}
	double getSecondsPerLap()
	{
		return secondsPerLap;
	};
	int getTotalPacingPanels()
	{
		return totalPacingPanels;
	};
	int getLightTrainLength()
	{
		return lightTrainLength;
	};
	double getInitialDelay()
	{
		return initialDelay;
	};
	int getInitialHighlightedPanel()
	{
		return initialHighlightedPanel;
	};
	int getNumMeters()
	{
		return numMeters;
	};
	double getNextLightDelay()			// The delay between pacing panels lighting up. This is derived from the number of seconds that it takes for the pacing panels to complete a lap and the total number of pacing panels
	{
		return ((getSecondsPerLap() / (double)getTotalPacingPanels()*1000));
	};
	int getCurrentHighlightedPacingPanel ()
	{
		currentHighlightedPacingPanel = (int)(((getRunningTime()%(long)((int)getSecondsPerLap()*(double)1000))/getNextLightDelay())+initialHighlightedPanel)%getTotalPacingPanels();
		return currentHighlightedPacingPanel;
	};
	long getStartTime()
	{
		return startTime;
	};
	long getRunningTime()
	{
		// This if statement is meant to solve the problem of pacing panels running before their delay
		if (millis() > getStartTime())
		return millis() - getStartTime();		// if the fix doesn't work, just leave this line
		else
		return 0;
	};
	uint32_t getShade()
	{
		return shade;
	};
	void setShade(uint32_t new_Shade)
	{
		shade = new_Shade;
	}
	void setTotalPacingPanels(int total_Pacing_Panels_)
	{
		totalPacingPanels = total_Pacing_Panels_;
	};
	void setStartTimeToNow()
	{
		startTime = millis();
	};
	void setStartTimeToNowPlusDelay(long delayMillis)
	{
		startTime = millis() + delayMillis;
	};
	void setStartTime(long start_Time)
	{
		startTime = start_Time;
	}
	void setCurrentHighlightedPacingPanel (int current_Highlighted_Pacing_Panel)
	{
		currentHighlightedPacingPanel = current_Highlighted_Pacing_Panel;
	};
	void setIsStopwatchStarted(boolean is_Stopwatch_Started)
	{
		isStopwatchStarted = is_Stopwatch_Started;
	};
	void setNumberPacers(int number_Pacers)
	{
		numberPacers = number_Pacers;
	};
	void setSecondsPerLap(double seconds_Per_Lap)
	{
		secondsPerLap = seconds_Per_Lap;
	};
	void setInitialDelay(int initDelay)
	{
		initialDelay = initDelay;
	};
	void setInitialHighlightedPanel(int firstHighlightedPanel)
	{
		initialHighlightedPanel = firstHighlightedPanel;
	};
	void setNumMeters(int meters)
	{
		numMeters = meters;
	};
	void setC(uint32_t c)
	{
		shade = c;
	};
};

int Pacer::numberPacers = 0;

int dataPin = 2;
int clockPin = 3;
int numLEDS = 20;

Pacer pacer[10] = {Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), 
	Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), 
	Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS) };
double secondsPerLapHolder;
//int inputPacer = 0;				//This allows the user to control this number's pacer's secondsPerLap through the serial connection
//int highestPacer = 0;
String serialStringInput;		// Holds the raw, unformatted serial input from user
String mode = "track";			// This mode String has two possible values: "track" and "party". Each value will result in different function calls
int partyInt = 10;				// This integer controls what party functions will be run; 0 indicates all will be run

// Set the first variable to the NUMBER of pixels. 32 = 32 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(numLEDS, dataPin, clockPin);

SoftwareSerial swerial(8,9);	// RX, TX
int serial1AvailableIterator = 0, serial1FeedbackIterator = 0, serialFeedbackIterator = 0, trafficLightInterator = 0;
int serialCountTo = 2000, trafficLightCountTo = 100;
int tempLowestDelayedPacerIndex = -1;

void setup()
{
	Serial.begin(9600);			// Allows Arduino to communicate with computer through Serial monitor tool window
	Serial1.begin(9600);		// Allows Arduino to communicate with mobile devices through bluetooth connection

	// Start up the LED strip
	strip.begin();

	// Update the strip, to start they are all 'off'
	strip.show();

	Serial.flush();
	Serial1.flush();
}

// If necessary, function declarations should be here - Declaring functions whose functions will be defined later

void loop()
{
	//getArdudroidInputFromUser();

	//setArdudroidTextCommandToArduinoSerialInput();

	setSerialInput();

	if (mode == "track")
	{
		setPixelColorBasedOnTime();

		getSerialFeedback();
	}
	else if (mode == "party")
	{
		partyFunctions();
	}
}

// send input from user via the Serial Monitor Tool to send to the Arduino device
void setSerialInput()
{
	// This could be an if statement or a while statement; an if statement will run the main loop between each serial input, but a while loop will process all the serial input and then return to the main loop

	while (Serial1.available() || Serial.available())
	{
		if (Serial1.available())
		{
			serialStringInput = Serial1.readStringUntil(' ');	// Serial1 processes serial input data from a mobile bluetooth connection
		}
		else
		{
			serialStringInput = Serial.readStringUntil(' ');	// Serial processes serial data input from a USB connection
		}

		if (mode == "track")
		{
			secondsPerLapHolder = atof(serialStringInput.c_str());
			if (secondsPerLapHolder > 0)
			{
				pacer[getLowestUnusedPacerIndex()].setStartTimeToNow();
				pacer[getLowestUnusedPacerIndex()].setSecondsPerLap (secondsPerLapHolder);
			}
			secondsPerLapHolder = 0;

			checkClearFlags();
			checkResetFlags();
			checkLightFlags();
			checkPartyModeFlags();

			serial1FeedbackIterator = serialCountTo;
			serialFeedbackIterator = serialCountTo;
			getSerialFeedback();

		}
		else // if (mode == "party")
		{
			checkTrackModeFlags();
			if (mode != "track")
			{
				partyInt = atoi(serialStringInput.c_str());
			}
		}
		serialStringInput = NULL;
	}
}

void checkPartyModeFlags()
{
	if (serialStringInput == "party")
	{
		mode = "party";
	}
}

void checkTrackModeFlags()
{
	if (serialStringInput == "track")
	{
		mode = "track";
	}
}

// Print the input that Arduino has in the Serial Monitor Tool
void getSerialFeedback()
{
	if (serialFeedbackIterator >= serialCountTo)
	{
		Serial.print("\ninputPacer = ");
		Serial.print(getLowestUnusedPacerIndex());
		for (int i=0; i < (getHighestActivePacerIndex()+1); i++) // changed from i < pacer[0].getNumberPacers();
		{
			if (pacer[i].getSecondsPerLap() > 0)
			{
				Serial.print(" Lap[");
				Serial.print(i);
				Serial.print("] = ");
				Serial.print(pacer[i].getSecondsPerLap());
				Serial.print(" c = ");
				Serial.print(pacer[i].getShade());
			}
		}
		Serial.print(" LEDs ");
		Serial.print(pacer[0].getTotalPacingPanels());
		
		serialFeedbackIterator = 0;
	}
	else
	{
		serialFeedbackIterator++;
	}

	if (serial1FeedbackIterator == serialCountTo)
	{
		Serial1.print("\ninputPacer = ");
		Serial1.print(getLowestUnusedPacerIndex());
		for (int i=0; i < (getHighestActivePacerIndex()+1); i++) // changed from i < pacer[0].getNumberPacers();
		{
			if (pacer[i].getSecondsPerLap() > 0)
			{
				Serial1.print(" Lap[");
				Serial1.print(i);
				Serial1.print("] = ");
				Serial1.print(pacer[i].getSecondsPerLap());
			}
		}
		Serial1.print(" LEDs ");
		Serial1.print(pacer[0].getTotalPacingPanels());

		serial1FeedbackIterator = 0;
	}
	else
	{
		serial1FeedbackIterator++;
	}
}

// Set Each Pixel's color based on what the what the current highlighted pixel (formerly, pacing panel) should be
void setPixelColorBasedOnTime()
{
	// *** Need to change the count to variables in these for loops to variables so it's not calling that function on every loop

	// Turn every light off
	for (int i=0; i < strip.numPixels(); i++) 
	{
		strip.setPixelColor(i, Color(0,0,0));
	}

	// Place the pacers where they are supposed to be with the correct color
	for (int j=0; j < getHighestActivePacerIndex()+1; j++)		// This can be changed to j < inputPacer (test with actual lights to be sure)
	{
		if (pacer[j].getSecondsPerLap() > 0)
		{
			strip.setPixelColor(pacer[j].getCurrentHighlightedPacingPanel(), pacer[j].getShade()); // set one pixel
		}
	}

	// Traffic Light Countdown for delayed pacers
	delayedPacerTrafficLightCountdown();

	strip.show();              // refresh strip display

}

// Check to see if the user sent a clear flag that signifies that one or all pacers needs to be cleared
void checkClearFlags()
{
	int serialInputInt;	// Holds the integer on the end of the string that the user input, such as "c1" or "r2"

	// If the user sends a string that starts with "c"
	if (serialStringInput.startsWith("c") == true)
	{
		// If the user sends a string that is longer than 1
		if (serialStringInput.length() > 1)
		{
			serialStringInput = serialStringInput.substring(1);
			serialInputInt = serialStringInput.toInt();
			//int initialSerialInputInstance = serialInputInt;
			
			if (serialInputInt <= getHighestActivePacerIndex())		// Are we sure that we've cleared a pacer that is smaller than the inputPacer?
			{
				pacer[serialInputInt].setSecondsPerLap(0);		// If the user sends the clear pacer i text string, clear pacer i and reset it to 0
			}
			return;
		}
		else // If the user sends the clear all text string
		{
			// set all pacers' secondsPerLap variable to 0
			for (int i = 0; i < pacer[0].getNumberPacers(); i++)
			{
				pacer[i].setSecondsPerLap(0);
			}

			return;
		}
	}
}

// Check to see if the user sent a reset flag that signifies that one or all pacers needs to be reset
void checkResetFlags()
{
	int serialInputInt;	// Holds the integer on the end of the string that the user input, such as "c1", "r2", "r", "rd", or "c"

	// If the user sends a string that starts with "r"
	if (serialStringInput.startsWith("r") == true)
	{
		if (serialStringInput.startsWith("rd") == 1)
		{
			if (serialStringInput.startsWith("rdp") == 1)
			{
				if (serialStringInput.length() > 3)
				{
					serialStringInput = serialStringInput.substring(3);
					serialInputInt = serialStringInput.toInt();	
					// call this pacer's setStartTimeToNowPlusDelay() function
					pacer[serialInputInt].setStartTimeToNowPlusDelay(5000); // This will always be 5 seconds unless I want to change it
					return;
				}
			}
			else if (serialStringInput.length() > 2)
			{
				serialStringInput = serialStringInput.substring(2);
				serialInputInt = serialStringInput.toInt();	
				// call all pacers' setStartTimeToNowPlusDelay() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setStartTimeToNowPlusDelay(serialInputInt*1000);
				}
				return;
			}
			else
			{
				// call all pacers' setStartTimeToNowPlusDelay() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setStartTimeToNowPlusDelay(5000);
				}
				return;
			}
		}
		// If the user sends a string that is longer than 1 character
		else if (serialStringInput.length() > 1)
		{
			serialStringInput = serialStringInput.substring(1);
			serialInputInt = serialStringInput.toInt();
			pacer[serialInputInt].setStartTimeToNow();
			return;
		}
		else
		{
			// call all pacers' setStartTimeToNow() function
			for (int i = 0; i < pacer[0].getNumberPacers(); i++)
			{
				pacer[i].setStartTimeToNow();
			}
			return;
		}
	}
}

// Check to see if the user sent a light flag that signifies that the totalPacingPanels value needs to be changed for each pacer, thus changing the number of lights on the track
void checkLightFlags()
{
	int newPacingPanels;

	// If the user sends a string that starts with "l" (lowercase L) and the user sends a string that is longer than 1 character
	if (serialStringInput.startsWith("l") == true && serialStringInput.length() > 1)
	{

		serialStringInput = serialStringInput.substring(1);
		newPacingPanels = serialStringInput.toInt();

		// call all pacers' setTotalPacingPanels function and pass it the value received from the user to set the number of lights on the track
		for (int i = 0; i < pacer[0].getNumberPacers(); i++)
		{
			pacer[i].setTotalPacingPanels(newPacingPanels);
		}
		strip.updateLength(newPacingPanels);

		return;
	}
}

// Returns the lowest available (empty) pacer or return the highest index; returns the index of the lowest pacer instance with getSecondsPerLap() == 0 unless all are greater than 0, in which case it will return the int associated with the instance of the highest pacer
int getLowestUnusedPacerIndex()
{
	for (int i = 0; i < pacer[0].getNumberPacers(); i++)
	{
		if (pacer[i].getSecondsPerLap() == 0)
		{
			return i;
			break;
		}
	}

	// Return the index of the highest pacer if a lower empty one was not found
	return pacer[0].getNumberPacers()-1;
}

// Returns the index of the highest pacer instance with getSecondsPerLap() > 0 or returns -1 if no pacers have getSecondsPerLap > 0
int getHighestActivePacerIndex()
{
	for (int i = pacer[0].getNumberPacers()-1; i > -1; i--)
	{
		if (pacer[i].getSecondsPerLap() > 0)
		{
			return i;
			break;
		}
	}
	return -1;
}

// Computes the Lowest Delayed Pacer only one time per trafficLightCountTo times that the main loop runs
void computeLowestDelayedPacer()
{
	if (trafficLightInterator >= trafficLightCountTo)
	{
		int highest_Active_Pacer_Panel = getHighestActivePacerIndex();
		for (int i=0; i <= highest_Active_Pacer_Panel; i++)	// used with the if statement to get the lowest delayed pacer index
		{
			if (pacer[i].isCurrentlyDelayed() == true)
			{
				tempLowestDelayedPacerIndex = i;
				return;
			}
		}
	}
	else
	{
		trafficLightInterator++;
	}
}

// Give delayed pacers a traffic light countdown
void delayedPacerTrafficLightCountdown()
{
	if (tempLowestDelayedPacerIndex <= -1)
	{
		computeLowestDelayedPacer();
		return;
	}
	else if (tempLowestDelayedPacerIndex > -1)
	{
		if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(7) == true)
		{
			if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(4) == true)
			{
				if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(2) == true)
				{
					if (pacer[tempLowestDelayedPacerIndex].isCurrentlyDelayed() == false)
					{
						trafficLightInterator = 0;
						tempLowestDelayedPacerIndex = -1;
						return;
					}
					strip.setPixelColor(0, Color(0,0,0));	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
					return;
				}
				strip.setPixelColor(1, Color(255,255,0)); // yellow
				return;
			}
			strip.setPixelColor(2, Color(255,0,0));	// red
			return;
		}
		return;
	}
}

//***************************
//****Party Functions
//***************************

void partyFunctions()
{
	// switch statement with party int as the value for choosing cases that calls different party functions based on its value
	switch(partyInt)
	{
		case 0:
			//scannerSequence();
			colorWipe(Color(255, 0, 0), 20);	// red
			colorWipe(Color(0, 0, 0), 20);
			break;
		case 1:
			//colorChaseSequence();
			colorWipe(Color(0, 255, 0), 20);	// green
			colorWipe(Color(0, 0, 0), 20);
			break;
		case 2:
			//colorWipeSequence();
			colorWipe(Color(0, 0, 255), 20);	// blue
			colorWipe(Color(0, 0, 0), 20);
			break;
		case 3:
			//ditherSequence
			rainbow(10);
			break;
		case 4:
			//rainbowCycleSequence();
			rainbowCycle(10);
			break;
		case 5:
			//waveSequence();
			colorWipe(Color(255, 0, 0), 20);	// red
			colorWipe(Color(0, 0, 0), 20);
			break;
		case 6:
			//partyCycle();
			colorWipe(Color(255, 0, 0), 20);	// red
			colorWipe(Color(0, 0, 0), 20);
			break;
		default:
			//partyInt = 0;
			colorWipe(Color(255, 0, 0), 20);	// red
			colorWipe(Color(0, 0, 0), 20);
			colorWipe(Color(0, 255, 0), 20);	// green
			colorWipe(Color(0, 0, 0), 20);
			colorWipe(Color(0, 0, 255), 20);	// blue
			colorWipe(Color(0, 0, 0), 20);
			colorWipe(Color(255, 255, 0), 20);	// orange
			colorWipe(Color(0, 0, 0), 20);
			colorWipe(Color(255, 0, 255), 20);	// purple
			colorWipe(Color(0, 0, 0), 20);
			colorWipe(Color(0, 255, 255), 20);	// aqua
			colorWipe(Color(0, 0, 0), 20);
			break;
	}
}
void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) 
	{
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
  setSerialInput();
}

/* Helper functions */

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
	if (WheelPos < 85) 
	{
		return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} 
	else if (WheelPos < 170) 
	{
		WheelPos -= 85;
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} 
	else 
	{
		WheelPos -= 170; 
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
	uint32_t c;
	c = r;
	c <<= 8;
	c |= g;
	c <<= 8;
	c |= b;
	return c;
}