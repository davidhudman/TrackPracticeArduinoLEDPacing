/*	**TrackPracticeWS2801**

	This version of the TrackPractice Arduino program is designed to work with a strip of WS2801 lights.
	I orginally ordered a 50-light strip of these lights off amazon for $31 (http://www.amazon.com/gp/product/B00LYRI1KY/).
	A similar 25-light strip is also sold by Adafruit for $40 (https://www.adafruit.com/product/738).

	This program contains only the track pacing features, not the "party" features

*/

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
*	Holds all information about each individual pacer object (lap time, color, delay, starting postition, etc).
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
	void setStartTimeToNow()
	{
		startTime = millis();
	};
	void setCurrentHighlightedPacingPanel (int current_Highlighted_Pacing_Panel)
	{
		currentHighlightedPacingPanel = current_Highlighted_Pacing_Panel;
	};
	void setIsStopwatchStarted(boolean is_Stopwatch_Started)
	{
		isStopwatchStarted = is_Stopwatch_Started;
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
int numLEDS = 45;

Pacer pacer[10] = {Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), 
	Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS), 
	Pacer(0,0,0,0,1,numLEDS), Pacer(0,0,0,0,1,numLEDS) };
double secondsPerLapHolder;
int inputPacer = 0;			// This allows the user to control this number's pacer's secondsPerLap through the serial connection
String serialStringInput;

// Set the first variable to the NUMBER of pixels. 32 = 32 pixels in a row
// The LED strips are 32 LEDs per meter but you can extend/cut the strip
//LPD8806 strip = LPD8806(32, dataPin, clockPin);
Adafruit_WS2801 strip = Adafruit_WS2801(numLEDS, dataPin, clockPin);

void setup()
{
	Serial.begin(9600);			// Allows Arduino to communicate with computer through Serial monitor tool window
	
	// Start up the LED strip
	strip.begin();

	// Update the strip, to start they are all 'off'
	strip.show();

	Serial.flush();
}

// If necessary, function declarations should be here - Declaring functions whose functions will be defined later

void loop()
{
	//getArdudroidInputFromUser();

	//setArdudroidTextCommandToArduinoSerialInput();

	setSerialInput();

	setPixelColorBasedOnTime();

	getSerialFeedback();
}

// send input from user via the Serial Monitor Tool to send to the Arduino device
void setSerialInput()
{
	while (Serial.available()) // This could be an if statement or a while statement; an if statement will run the main loop between each serial input, but a while loop will process all the serial input and then return to the main loop
	{
		serialStringInput = Serial.readStringUntil(' ');
		secondsPerLapHolder = atof(serialStringInput.c_str());
		if (secondsPerLapHolder > 0)
		{
			pacer[inputPacer].setStartTimeToNow();
			pacer[inputPacer].setSecondsPerLap (secondsPerLapHolder);
			inputPacer++;
		}
		secondsPerLapHolder = 0;

		checkClearFlags();
		checkResetFlags();

		serialStringInput = NULL;
	}
	if (pacer[inputPacer].getSecondsPerLap() > 0 && inputPacer < pacer[0].getNumberPacers())
	{
		inputPacer++;
	}
}

// Print the input that Arduino has in the Serial Monitor Tool
void getSerialFeedback()
{
	Serial.print("\ninputPacer = ");
	Serial.print(inputPacer);
	for (int i=0; i < pacer[0].getNumberPacers(); i++)
	{
		Serial.print(" Lap[");
		Serial.print(i);
		Serial.print("] = ");
		Serial.print(pacer[i].getSecondsPerLap());
	}
}

// Set Each Pixel's color based on what the what the current highlighted pixel (formerly, pacing panel) should be
void setPixelColorBasedOnTime()
{
	for (int i=0; i < strip.numPixels(); i++) 
	{
		strip.setPixelColor(i, Color(0,0,0));
	}

	for (int j=0; j < pacer[0].getNumberPacers(); j++)
	{
		if (pacer[j].getSecondsPerLap() > 0)
		{
			strip.setPixelColor(pacer[j].getCurrentHighlightedPacingPanel(), pacer[j].getShade()); // set one pixel
		}
	}
	strip.show();              // refresh strip display

}

void checkClearFlags()
{
	//serialStringInput = Serial.readString();
	if (serialStringInput == "c")	// If the user sends the clear all text string
	{
		// set all pacers' secondsPerLap variable to 0
		for (int i = 0; i < pacer[0].getNumberPacers(); i++)
		{
			pacer[i].setSecondsPerLap(0);
		}

		// set inputPacer to 0
		inputPacer = 0;
		return;
	}
	if (serialStringInput == "c1")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[0].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c2")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[1].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c3")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[2].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c4")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[3].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c5")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[4].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c6")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[5].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c7")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[6].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c8")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[7].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c9")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[8].setSecondsPerLap(0);
		return;
	}
	if (serialStringInput == "c10")	// If the user sends the clear pacer 1 text string, clear pacer 1 and reset it to 0
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[9].setSecondsPerLap(0);
		return;
	}
}

void checkResetFlags()
{
	if (serialStringInput == "r")	// If the user sends the restart all text string
	{
		// call all pacers' setStartTimeToNow() function
		for (int i = 0; i < pacer[0].getNumberPacers(); i++)
		{
			pacer[i].setStartTimeToNow();
		}
		return;
	}
	if (serialStringInput == "r1")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[0].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r2")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[1].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r3")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[2].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r4")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[3].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r5")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[4].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r6")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[5].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r7")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[6].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r8")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[7].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r9")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[8].setStartTimeToNow();
		return;
	}
	if (serialStringInput == "r10")	// If the user sends the restart pacer 1 text string, restart pacer 1 by calling setStartTimeToNow
	{
		// set pacer[0] secondsPerLap variable to 0
		pacer[9].setStartTimeToNow();
		return;
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