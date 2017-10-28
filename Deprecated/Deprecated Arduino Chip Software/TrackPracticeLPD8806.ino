/*	**TrackPracticeLDP8806
	
	This version of the TrackPractice Arduino program is designed to work with a strip of LPD8806 lights.
	I orginally ordered a 32-light strip of these from Adafruit for $30 (https://www.adafruit.com/products/306).

	This program contains the track pacing features AND the "party" features
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
		LPD8806 strip1 = LPD8806();

		uint32_t color[7] = {strip1.Color(127,127,127), strip1.Color(127,0,0), strip1.Color(127,127,0), strip1.Color(0,127,0), strip1.Color(0,127,127), strip1.Color(0,0,127), strip1.Color(127,0,127)}; 
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
int numLEDs = 32;

Pacer pacer[10] = {Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), 
	Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs), 
	Pacer(0,0,0,0,1,numLEDs), Pacer(0,0,0,0,1,numLEDs) };

double secondsPerLapHolder;
int inputPacer = 0;			// This allows the user to control this number's pacer's secondsPerLap through the serial connection
String serialStringInput;
String mode = "track";		// This mode String has two possible values: "track" and "party". Each value will result in different function calls
int partyInt;			// This integer controls what party functions will be run; 0 indicates all will be run

// Set the first variable to the NUMBER of pixels. 32 = 32 pixels in a row
// The LED strips are 32 LEDs per meter but you can extend/cut the strip
LPD8806 strip = LPD8806(32, dataPin, clockPin);

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

	if (mode == "track")
	{
		setPixelColorBasedOnTime();

		getSerialFeedback();
	}
	else if (mode == "party")
	{
		// switch statement with party int as the value for choosing cases that calls different party functions based on its value
		switch(partyInt)
		{
			case 0:
				scannerSequence();
				break;
			case 1:
				colorChaseSequence();
				break;
			case 2:
				colorWipeSequence();
				break;
			case 3:
				ditherSequence();
				break;
			case 4:
				rainbowCycleSequence();
				break;
			case 5:
				waveSequence();
				break;
			case 6:
				partyCycle();
				break;
			default:
				//partyInt = 0;
				break;
		}
	}
}

// send input from user via the Serial Monitor Tool to send to the Arduino device
void setSerialInput()
{
	while (Serial.available()) // This could be an if statement or a while statement; an if statement will run the main loop between each serial input, but a while loop will process all the serial input and then return to the main loop
	{
		serialStringInput = Serial.readStringUntil(' ');

		if (mode == "track")
		{
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
			checkPartyModeFlags(); 
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
	if (mode == "track")
	{
		if (pacer[inputPacer].getSecondsPerLap() > 0 && inputPacer < pacer[0].getNumberPacers())
		{
			inputPacer++;
		}
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
		strip.setPixelColor(i, 0);
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

/*
*	Party Mode Functions
*
*
*/

// Cycle through the color wheel, equally spaced around the belt
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color
      // wheel (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
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
}

// Chase a dot down the strip
// good for testing purposes
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);  // turn all pixels off
  }

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c); // set one pixel
      strip.show();              // refresh strip display
      delay(wait);               // hold image for a moment
      strip.setPixelColor(i, 0); // erase pixel (but don't refresh yet)
  }
  strip.show(); // for last erased pixel
}

// An "ordered dither" fills every pixel in a sequence that looks
// sparkly and almost random, but actually follows a specific order.
void dither(uint32_t c, uint8_t wait) {

  // Determine highest bit needed to represent pixel index
  int hiBit = 0;
  int n = strip.numPixels() - 1;
  for(int bit=1; bit < 0x8000; bit <<= 1) {
    if(n & bit) hiBit = bit;
  }

  int bit, reverse;
  for(int i=0; i<(hiBit << 1); i++) {
    // Reverse the bits in i to create ordered dither:
    reverse = 0;
    for(bit=1; bit <= hiBit; bit <<= 1) {
      reverse <<= 1;
      if(i & bit) reverse |= 1;
    }
    strip.setPixelColor(reverse, c);
    strip.show();
    delay(wait);
  }
  delay(250); // Hold image for 1/4 sec
}

// "Larson scanner" = Cylon/KITT bouncing light effect
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  int i, j, pos, dir;

  pos = 0;
  dir = 1;

  for(i=0; i<((strip.numPixels()-1) * 8); i++) {
    // Draw 5 pixels centered on pos.  setPixelColor() will clip
    // any pixels off the ends of the strip, no worries there.
    // we'll make the colors dimmer at the edges for a nice pulse
    // look
    strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
    strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos, strip.Color(r, g, b));
    strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

    strip.show();
    delay(wait);
    // If we wanted to be sneaky we could erase just the tail end
    // pixel, but it's much easier just to erase the whole thing
    // and draw a new one next time.
    for(j=-2; j<= 2; j++) 
        strip.setPixelColor(pos+j, strip.Color(0,0,0));
    // Bounce off ends of strip
    pos += dir;
    if(pos < 0) {
      pos = 1;
      dir = -dir;
    } else if(pos >= strip.numPixels()) {
      pos = strip.numPixels() - 2;
      dir = -dir;
    }
  }
}

// Sine wave effect
#define PI 3.14159265
void wave(uint32_t c, int cycles, uint8_t wait) {
  float y;
  byte  r, g, b, r2, g2, b2;

  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f; 

  for(int x=0; x<(strip.numPixels()*5); x++)
  {
    for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 127 - (byte)((float)(127 - r) * y);
        g2 = 127 - (byte)((float)(127 - g) * y);
        b2 = 127 - (byte)((float)(127 - b) * y);
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
    }
    strip.show();
    delay(wait);
  }
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g - b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128; // red down
      g = WheelPos % 128;       // green up
      b = 0;                    // blue off
      break;
    case 1:
      g = 127 - WheelPos % 128; // green down
      b = WheelPos % 128;       // blue up
      r = 0;                    // red off
      break;
    case 2:
      b = 127 - WheelPos % 128; // blue down
      r = WheelPos % 128;       // red up
      g = 0;                    // green off
      break;
  }
  return(strip.Color(r,g,b));
}

void partyCycle()
{
	colorChaseSequence();
	colorWipeSequence();
	ditherSequence();
	scannerSequence();
	waveSequence();
	rainbowCycleSequence();
	resetStrip();
}

void colorChaseSequence()
{
	// Send a simple pixel chase in...
	colorChase(strip.Color(127,127,127), 20); // white
	colorChase(strip.Color(127,0,0), 20);     // red
	colorChase(strip.Color(127,127,0), 20);   // yellow
	colorChase(strip.Color(0,127,0), 20);     // green
	colorChase(strip.Color(0,127,127), 20);   // cyan
	colorChase(strip.Color(0,0,127), 20);     // blue
	colorChase(strip.Color(127,0,127), 20);   // magenta
	resetStrip();
}

void colorWipeSequence()
{
	// Fill the entire strip with...
	colorWipe(strip.Color(127,0,0), 20);      // red
	colorWipe(strip.Color(0, 127,0), 20);     // green
	colorWipe(strip.Color(0,0,127), 20);      // blue
	colorWipe(strip.Color(0,0,0), 20);        // black
	resetStrip();
}

void ditherSequence()
{
	// Color sparkles
	dither(strip.Color(0,127,127), 50);       // cyan, slow
	dither(strip.Color(0,0,0), 15);           // black, fast
	dither(strip.Color(127,0,127), 50);       // magenta, slow
	dither(strip.Color(0,0,0), 15);           // black, fast
	dither(strip.Color(127,127,0), 50);       // yellow, slow
	dither(strip.Color(0,0,0), 15);           // black, fast
	resetStrip();
}

void scannerSequence()
{
	// Back-and-forth lights
	scanner(127,0,0, 30);        // red, slow
	scanner(0,0,127, 15);        // blue, fast
	resetStrip();
}

void waveSequence()
{
	// Wavy ripple effects
	wave(strip.Color(127,0,0), 4, 20);        // candy cane
	wave(strip.Color(0,0,100), 2, 40);        // icy
	resetStrip();
}

void rainbowCycleSequence()
{
	// make a pretty rainbow cycle!
	rainbowCycle(0);  // make it go through the cycle fairly fast
	resetStrip();
}

void resetStrip()
{
	// Clear strip data before start of next effect
	for (int i=0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0);
	}
	setSerialInput();
}