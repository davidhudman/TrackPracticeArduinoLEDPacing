/*	**TrackPractice for WS2801 or WS2811/WS2812**

	This version of the TrackPractice Arduino program is designed to work with a strip of WS2801 lights or WS2811/WS2812. You can just uncomment and comment the corresponding "strip" declaration lines.
	I orginally ordered a 50-light strip of these lights off amazon for $31 (http://www.amazon.com/gp/product/B00LYRI1KY/).
	A similar 25-light strip is also sold by Adafruit for $40 (https://www.adafruit.com/product/738).

	This program contains both the track pacing features and the "party" features

*/

// #include <Adafruit_NeoPixel.h>

#include <FastLED.h>

// #include <TimerOne.h>
// #include <Adafruit_NeoPixel.h>
//#include <Adafruit_WS2801.h>
// #include <Time.h>
//#include "LPD8806.h"
// #include <SPI.h>

/**
*	Pacer Class - public in java program
*	Holds all information about each individual pacer object (lap time, color, delay, starting postition, etc)
*/
class Pacer {
private:
	double initialDelay, 					//UI// How long each pacer should wait before starting
				secondsPerLap,				//UI// The number of seconds that it will take for the pacing panels to complete one lap
				futureSecondsPerLap;			// The number of seconds per lap that the pacer will be set to
	int initialHighlightedPanel, 			//UI// This determines where the pacer starts from
				numMeters,						// This determines how many meters the pacing lights will run for
				totalPacingPanels,				// This is passed as an argument from JpanelPractice
				lightTrainLength;			//UI// This determines how many lights wide this pacer will be
	uint32_t shade;		//Color shade;		//UI// This determines the color of the pacer's pacing lights
	bool isStopwatchStarted;
	bool isBackwards;
	bool isGoingToChangeSpeed;				// indicates whether the pacer is about to change its speed
	bool isVisible;							// indicates whether the pacer should be visible or not (the color black / it logically keeps running)
	long startTime, futureStartTime;
	static int numberPacers; // static
	int currentHighlightedPacingPanel;		// This determines which pacing panel is currently lit up
	int colorInt;							// Determines which color index in the color array that the pacer should be

public:
	// variables

	// classes
	Pacer(double lapSecs, double initDelay, int firstHighlightedPanel, int meters, int light_Train_Length, int total_Pacing_Panels) // Constructor
	{
		//Adafruit_WS2801 strip1 = Adafruit_WS2801();

		secondsPerLap = lapSecs;
		initialDelay = initDelay;
		currentHighlightedPacingPanel = firstHighlightedPanel;
		initialHighlightedPanel = firstHighlightedPanel;
		numMeters = meters;
		colorInt = numberPacers%7;
		lightTrainLength = light_Train_Length;
		isStopwatchStarted = false;
		isBackwards = false;
		isGoingToChangeSpeed = false;
		isVisible = true;
		totalPacingPanels = total_Pacing_Panels;
		numberPacers++;
		startTime = myMillis() + (long)initialDelay;
		double lapTimesArray[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	};
	int getNumberPacers()
	{
		return numberPacers;
	};
	bool getIsStopwatchStarted()
	{
		return isStopwatchStarted;
	};
	bool getIsGoingToChangeSpeed()
	{
		return isGoingToChangeSpeed;
	};
	bool isCurrentlyDelayed()
	{
		if ((startTime - myMillis()) > 0)
		{
			return true;
		}
		else return false;
	};
	bool isStartTimeWithinXSecondsAndGreaterThanZero(int seconds)
	{
		if (((startTime - myMillis()) > 0) && ((startTime - myMillis()) < (seconds*1000)))
		{
			return true;
		}
		else return false;
	};
	bool isStartTimeWithinXSecondsOnly(int seconds)
	{
		if ((startTime - myMillis()) < (seconds*1000))
		{
			return true;
		}
		else return false;
	};
	bool getIsBackwards()
	{
		return isBackwards;
	};
	long getDelayRemaining()
	{
		return startTime - myMillis();
	}
	double getSecondsPerLap()
	{
		return secondsPerLap;
	};
	double getFutureSecondsPerLap()
	{
		return futureSecondsPerLap;
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
		if (isBackwards)
		{
			return getTotalPacingPanels()-initialHighlightedPanel;
		}
		else
		{
			return initialHighlightedPanel;
		}
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
		if (!isBackwards)	// if isBackwards is not true
		{
			currentHighlightedPacingPanel = (int)(((getRunningTime()%(long)(getSecondsPerLap()*(double)1000))/getNextLightDelay())+getInitialHighlightedPanel())%getTotalPacingPanels();
			return currentHighlightedPacingPanel;
		}
		else
		{
			currentHighlightedPacingPanel = (getTotalPacingPanels()-1) - (int)(((getRunningTime()%(long)(getSecondsPerLap()*(double)1000))/getNextLightDelay())+getInitialHighlightedPanel())%getTotalPacingPanels();
			return currentHighlightedPacingPanel;
		}
	};
	long getStartTime()
	{
		return startTime;
	};
	long getFutureStartTime()
	{
		return futureStartTime;
	};
	long getRunningTime()
	{
		// This if statement is meant to solve the problem of pacing panels running before their delay
		if (myMillis() > getStartTime())
		return myMillis() - getStartTime();		// if the fix doesn't work, just leave this line
		else
		return 0;
	};
	int getColorInt()
	{
		return colorInt;
	}
	bool getIsVisible()
	{
		return isVisible;
	}
	void setIsVisible(bool is_Visible)
	{
		isVisible = is_Visible;
	}
	void setColorInt(int i)
	{
		if (i>=0 && i<8)
		{
			colorInt = i;
		}
	}
	void setTotalPacingPanels(int total_Pacing_Panels_)
	{
		totalPacingPanels = total_Pacing_Panels_;
	};
	void setStartTimeToNow()
	{
		startTime = myMillis();
	};
	void setStartTimeToNowPlusDelay(long delayMillis)
	{
		startTime = myMillis() + delayMillis;
	};
	void setStartTime(long start_Time)
	{
		startTime = start_Time;
	};
	void setFutureStartTime(long future_Start_Time)
	{
		futureStartTime = future_Start_Time;
	}
	void setCurrentHighlightedPacingPanel (int current_Highlighted_Pacing_Panel)
	{
		currentHighlightedPacingPanel = current_Highlighted_Pacing_Panel;
	};
	void setIsStopwatchStarted(boolean is_Stopwatch_Started)
	{
		isStopwatchStarted = is_Stopwatch_Started;
	};
	void setIsBackwards(boolean is_Backwards)
	{
		isBackwards = is_Backwards;
	};
	void setIsGoingToChangeSpeed(boolean is_Going_To_Change_Speed)
	{
		isGoingToChangeSpeed = is_Going_To_Change_Speed;
	};
	void setNumberPacers(int number_Pacers)
	{
		numberPacers = number_Pacers;
	};
	void setSecondsPerLap(double seconds_Per_Lap)
	{
		secondsPerLap = seconds_Per_Lap;
	};
	void setFutureSecondsPerLap(double future_Seconds_Per_Lap)
	{
		futureSecondsPerLap = future_Seconds_Per_Lap;
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
};

int Pacer::numberPacers = 0;

#define DATA_PIN 2
#define CLOCK_PIN 3
#define NUM_LEDS 30

//***********************************************
// Declarations: Related to the pacer objects
//***********************************************
const int PACER_ARRAY_SIZE = 10;
Pacer pacer[PACER_ARRAY_SIZE] = {Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS) };
uint32_t color[7] = {Color(127,127,127), Color(127,0,0), Color(127,127,0), Color(0,127,0), Color(0,127,127), Color(0,0,127), Color(127,0,127)};
// const int COLOR_ARRAY_SIZE = 7;
// const String colorName[COLOR_ARRAY_SIZE] = {"white", "green", "yellow", "red", "magenta", "blue", "cyan"};
		// white, red, yellow, green, cyan, blue, magenta

//***********************************************
// Declarations: Related to strings
//***********************************************
const int TRACK_FLAG_SIZE = 15, PARTY_FLAG_SIZE = 11;
const String trackFlags[TRACK_FLAG_SIZE] = {"c", "r", "l", "b", "rd", "rdp", "party", "track", "spt", "strip", "a", "pct", "v", "apb", "spb"};	// This array is used to make a hashmap so that I can associate the index of the array with an integer for a switch statement
const String partyFlags[PARTY_FLAG_SIZE] = {"red wipe", "green wipe", "blue wipe", "rainbow", "rainbow cycle", "red wipe", "red wipe", "scanner", "multi-color dither", "multi-color colorchase", "multi-color wipe"};	// This array is used to make a hasmap so I can associate the index of the array with its party function
String stringSepFlag = ",";	// holds the string that separates the values in the speed change function
String serialStringInput;			// Holds the raw, unformatted serial input from user.
String printThis = " ";
String stringHolder = " ";

//***********************************************
// Declarations: All variables predefined
//***********************************************
double trafficLightCountDownRedSeconds = 7, trafficLightCountDownYellowSeconds = 4, trafficLightCountDownDarkSeconds = 2; // Traffic light countdown variables for red, yellow, and dark/go
long resetDelayDefaultDelayTimeMillis = 10000;		// the default delay time when resetting pacers on a delay
int serial1AvailableIterator = 0, serial1FeedbackIterator = 0, serialFeedbackIterator = 0, trafficLightIterator = 0;
int serialCountTo = 50, trafficLightCountTo = 100, partySerialCountTo = 5;
int tempLowestDelayedPacerIndex = -1;

bool isChangePacerSpeedNeeded = false; // trigger to determine whether we need to figure out which pacer is going to change its speed
bool partyMode = false;				// This mode (now partyMode) String (now bool) has two possible values: "track" and "party" (now true and false). Each value will result in different function calls
int partyInt = 10;					// This integer controls what party functions will be run; 0 indicates all will be run

//***********************************************
// Declarations: All variables not predefined
//***********************************************
double secondsPerLapHolder;
long tempMillis;		// holds the milliseconds time that a pacer's speed will change
double newSecondsPerLap; // holds the double (floating point) time that will become the pacer's secondsPerLap

//***********************************************
// Declarations: Everything related to the strip and its timing
//***********************************************
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDS, dataPin, NEO_RGB + NEO_KHZ800); // Initialization of the Adafruit NeoPixel strip
CRGB leds[NUM_LEDS];
double clockAdjustmentFactor = 1.0;		// (originally set at .5) Use with caution. Different numLEDs require different adjustments. Since the Adafruit_NeoPixel library creates timing problems for Arduino, this factor will be used to adjust all user input and output, so the user can continue to use numbers they would want. This is a temporary workaround until the timing issues created by the library are resolved.
long interruptMillis = 0;
long interruptFreqMicro = 10000;		// The number of microseconds that will pass before the interrupt fires (example: 10,000 microseconds = .01 seconds).
//Adafruit_WS2801 strip = Adafruit_WS2801(numLEDS, dataPin, clockPin);

void setup()
{
	// Uncomment/edit one of the following lines for your leds arrangement.
	// FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
	// FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
	FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
	// FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	// FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
	// FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

	Serial.begin(9600);			// Allows Arduino to communicate with computer through Serial monitor tool window
	Serial1.begin(9600);		// Allows Arduino to communicate with mobile devices through bluetooth connection

	// strip.begin();				// Start up the LED strip
	FastLED.show();				// Update the strip, to start they are all 'off'

	Serial.flush();
	Serial1.flush();

	//Timer1.initialize(10000);         // initialize timer1, and set a .01 second (10,000 micro second) period
	//Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

// If necessary, function declarations should be here - Declaring functions whose functions will be defined later
void loop()
{
	setSerialInput();

	if (partyMode == false)
	{
		setPixelColorBasedOnTime();

		getSerialFeedback();
	}
	else
	{
		partyFunctions();
		getPartySerialFeedback();
	}
}

void callback()
{
	interruptMillis+=(interruptFreqMicro/1000);
}

long myMillis()
{
	return interruptMillis;
}

// Use an integer parameter to determine what RGB color value it is associated with
uint32_t getColorFromInt(int i)
{
	return color[i];
}

// Use an integer parameter to determine what color word it is associated with
String getColorWord(int color_Int)
{
	switch (color_Int)
	{
	case 0:
		return "white";
		break;
	case 1:
		return "green";		// actually red on better strip
		break;
	case 2:
		return "yellow";
		break;
	case 3:
		return "red";		// actually green on better strip
		break;
	case 4:
		return "magenta";		// actually cyan on better strip
		break;
	case 5:
		return "blue";
		break;
	case 6:
		return "cyan";	// actually magenta on better strip
		break;
	case 7:
		return "black";
		break;
	default:
		return "?";
		break;
	}
}

// returns the new start time for a pacer intersecting the given pacer at the current light its on given the desired seconds per lap; change the Speed of the Pacer To (command "spt")
void setChangedPacerNewStartTime(int index, double new_SecondsPerLap)
{
	/*long temp_Millis, startTime = pacer[index].getStartTime(), new_startTime, currentTime = myMillis(), divisionResult;
	int getTotalPacingPanels = pacer[index].getTotalPacingPanels(), initialHighlightedPanel = pacer[index].getInitialHighlightedPanel();
	double getSecondsPerLap = pacer[index].getSecondsPerLap();
	int current_Highlighted_Pacing_Panel = pacer[index].getCurrentHighlightedPacingPanel();

	// use currentHighlightedPacingPanel and solve for getRunningTime (what myMillis() will be when it hit that panel that you're on)
	new_startTime = (long)(((((current_Highlighted_Pacing_Panel + getTotalPacingPanels) - initialHighlightedPanel)*(getSecondsPerLap / getTotalPacingPanels * 1000))+(getSecondsPerLap*1000)) + startTime);
	// temp_Millis probably needs to be verified bigger than myMillis()

	divisionResult = currentTime / (getSecondsPerLap*1000);

	new_startTime = new_startTime + (divisionResult*(getSecondsPerLap*1000)) + (getSecondsPerLap*1000);
	
	while (currentTime < new_startTime)
	{
		new_startTime -= (getSecondsPerLap*1000);
	}

	new_startTime -= (current_Highlighted_Pacing_Panel - initialHighlightedPanel) * ((new_SecondsPerLap*1000) / getTotalPacingPanels);	// when this line is added, also make the change below from setting the pacer's initialHighlightedPanel with currentHighlightedPanel to initialHighlightedPanel

	pacer[index].setStartTime(new_startTime);
	pacer[index].setSecondsPerLap(new_SecondsPerLap);
	pacer[index].setInitialHighlightedPanel(initialHighlightedPanel);		// change to initialHighlightedPanel eventually
	// tempMillis = temp_Millis;

	// then use the new getRunningTime (actually tempMillis and the new_SecondsPerLap to solve for startTime
	// new_startTime = -(long)(((((current_Highlighted_Pacing_Panel + getTotalPacingPanels) - initialHighlightedPanel) * (new_SecondsPerLap / getTotalPacingPanels * 1000)) + (new_SecondsPerLap*1000)) - temp_Millis);

	// we need to return the new_startTime to change the pacer's start time, but we need to do this after myMillis() > tempMillis;
	// return new_startTime;*/
}

// send input from user via the Serial Monitor Tool to send to the Arduino device
void setSerialInput()
{
	// This could be an if statement or a while statement; an if statement will run the main loop between each serial input, but a while loop will process all the serial input and then return to the main loop

	while (Serial1.available() || Serial.available())
	{
		tempMillis = myMillis();
		if (Serial1.available())
		{
			serialStringInput = Serial1.readStringUntil(' ');	// Serial1 processes serial input data from a mobile bluetooth connection
			//serialStringInput.trim();
		}
		else
		{
			serialStringInput = Serial.readStringUntil(' ');	// Serial processes serial data input from a USB connection
			//serialStringInput.trim();
		}

		if (partyMode == false)
		{
			checkAllUserInput();

			serial1FeedbackIterator = serialCountTo;
			serialFeedbackIterator = serialCountTo;
			getSerialFeedback();

		}
		else // if (partyMode == true)
		{
			checkTrackModeFlags();
			partyInt = serialStringInput.toInt();
		}
		serialStringInput = " ";
	}
}

// Checks for "track" as input sent from the user to change the mode from something else (like "party" mode)
void checkTrackModeFlags()
{
	if (serialStringInput == trackFlags[7])
	{
		partyMode = false;
	}
}

// Print user output that pertains to party functions
void getPartySerialFeedback()
{
	if (serialFeedbackIterator >= partySerialCountTo)				// desktop direct wired connection feedback
	{
		printThis = " ";

		for (int i=0; i < PARTY_FLAG_SIZE; i++)			// Bug: potential bug because this .length may just return the length of the string, not the array; If it does, you can use something like sizeOf(partyFlags) / sizeOf(partyFlags[0]
		{
			if (i == partyInt)
			{
				printThis.concat("x");				// "x" marks the party function that is currently running
			}

			printThis.concat(i);
			printThis.concat(": ");
			printThis.concat(partyFlags[i]);
			printThis.concat("... ");
		}

		Serial.println(printThis);
		
		serialFeedbackIterator = 0;
	}
	else
	{
		serialFeedbackIterator++;
	}

	if (serial1FeedbackIterator >= partySerialCountTo)				// bluetooth mobile feedback.
	{
		printThis = " ";

		for (int i=0; i < PARTY_FLAG_SIZE; i++)			// Bug: potential bug because this .length may just return the length of the string, not the array; If it does, you can use something like sizeOf(partyFlags) / sizeOf(partyFlags[0]
		{	
			if (i == partyInt)
			{
				printThis.concat("x");				// "x" marks the party function that is currently running
			}

			printThis.concat(i);
			printThis.concat(": ");
			printThis.concat(partyFlags[i]);
			printThis.concat("... ");
		}

		Serial.println(printThis);

		serial1FeedbackIterator = 0;
	}
	else
	{
		serial1FeedbackIterator++;
	}
}

// Print the output that Arduino has in the Serial Monitor Tool
void getSerialFeedback()
{
	if (serialFeedbackIterator >= serialCountTo)				// desktop direct wired connection feedback
	{
		// This is going to need to be JSON output like "glossary": { "title": "example glossary", "GlossDiv": {
		printThis = " ";
		printThis.concat("{ ");
		printThis.concat("\"inputPcr\": \"");
		printThis.concat(getLowestUnusedPacerIndex());
		printThis.concat("\", ");
		printThis.concat("\"Time\": {\"totalMillis\": ");
		printThis.concat(myMillis());
		printThis.concat("\", \"hr\": \"");
		printThis.concat((myMillis()/3600000));
		printThis.concat("\", \"min\": \"");
		printThis.concat((myMillis()/60000)%(60));
		printThis.concat("\", \"sec\": \"");
		printThis.concat((myMillis()/1000)%(60));
		printThis.concat("\", \"millis\": \"");
		printThis.concat(myMillis()%1000);
		printThis.concat("\"}, ");
		printThis.concat("\"LEDs\": \"");
		printThis.concat(pacer[0].getTotalPacingPanels());
		printThis.concat("\", ");

		for (int i=0; i < (getHighestActivePacerIndex()+1); i++)
		{
			if (pacer[i].getSecondsPerLap() > 0)
			{
				printThis.concat("\"Pcr\": {\"i\": \"");
				printThis.concat(i);
				printThis.concat("\", \"LapSecs\": \"");

				// round the float
				float b = (pacer[i].getSecondsPerLap()) * 100;
				while (b >= 100)
				{
					b -= 100;
				}
				printThis.concat((int)(pacer[i].getSecondsPerLap()));
				printThis.concat(".");
				printThis.concat((int)(b));

				printThis.concat("\", \"colorInt\": \"");
				printThis.concat(pacer[i].getColorInt());
				printThis.concat("\", \"color\": \"");
				printThis.concat(getColorWord(pacer[i].getColorInt()));
				printThis.concat("\"}, ");
			}
		}

		printThis.concat("}");

		Serial.println(printThis);
		
		serialFeedbackIterator = 0;
	}
	else
	{
		serialFeedbackIterator++;
	}

	if (serial1FeedbackIterator >= serialCountTo)				// bluetooth mobile feedback
	{
		printThis = " ";
		printThis.concat("\ninputPacer = ");
		printThis.concat(getLowestUnusedPacerIndex());
		for (int i=0; i < (getHighestActivePacerIndex()+1); i++)
		{
			if (pacer[i].getSecondsPerLap() > 0)
			{
				printThis.concat(" Lap[");
				printThis.concat(i);
				printThis.concat("] = ");
				printThis.concat((int)(pacer[i].getSecondsPerLap()));
			}
		}
		printThis.concat(" LEDs ");
		printThis.concat(pacer[0].getTotalPacingPanels());

		Serial1.println(printThis);

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
	for (int i=0; i < FastLED.size(); i++)
	{
		leds[i] = CHSV( 0, 0, 0);
	}

	// Place the pacers where they are supposed to be with the correct color
	for (int j=0; j < getHighestActivePacerIndex()+1; j++)		// This can be changed to j < inputPacer (test with actual lights to be sure)
	{
		if (pacer[j].getSecondsPerLap() > 0)
		{
			leds[pacer[j].getCurrentHighlightedPacingPanel()] = CHSV( 255, 255, 255);

			// strip.setPixelColor(pacer[j].getCurrentHighlightedPacingPanel(), getColorFromInt(pacer[j].getColorInt())); // set one pixel
		}
	}

	// Traffic Light Countdown for delayed pacers
	delayedPacerTrafficLightCountdown();

	FastLED.show();              // refresh strip display

}

// checks for all flags in the user's input with a switch statement
void checkAllUserInput()
{
	// parse the letters from the numbers and put the letters into parsedString
	int inChar;					// a temporary variable for holding a character to decide if it equals a digit or not
	String parsedLetterString;	// a temporary string for holding the user's non-digit string input
	String parsedNumericString;	// a temporary string for holding the user's digit string input
	int serialInputInt = -1;	// Holds the integer on the end of the string that the user input, such as "c1", "r2", "r", "rd", or "c"; It is equal to negative one because the user will never enter that value and can be used to determine bad input
	double serialInputDouble = -1;	// Holds lap time in the case of "spt1,75" for set pacer 1 to 75 second lap

	// Check if the user sent a lap time
	secondsPerLapHolder = atof(serialStringInput.c_str());		// change: try using String.toFloat() here
	if (secondsPerLapHolder > 0)
	{
		pacer[getLowestUnusedPacerIndex()].setStartTime(tempMillis);
		pacer[getLowestUnusedPacerIndex()].setSecondsPerLap (secondsPerLapHolder);
		secondsPerLapHolder = 0;
		return;												// This allows me to skip the checks for all the other flags if the user sent this input
	}
	else
	{
		secondsPerLapHolder = 0;
	}

	// specifically made to deal with spt#,#.##, but it seems to work for "pct" and maybe "apb"
	if (serialStringInput.lastIndexOf(stringSepFlag) > -1)
	{
		for (int i=0; i < TRACK_FLAG_SIZE; i++)
		{
			if (serialStringInput.substring(0,3).equals(trackFlags[i]))
			{
				parsedLetterString = trackFlags[i];

				serialInputInt = serialStringInput.substring(3,serialStringInput.indexOf(stringSepFlag)).toInt();

				serialInputDouble = atof(serialStringInput.substring(serialStringInput.indexOf(stringSepFlag)+1).c_str());
			}

		}
	}
	else
	{
		// Parse the string that the user sent into characters and numbers (optional)
		for (int i = 0; i < serialStringInput.length(); i++)	// Bug: I might need to reconsider the starting point because of the space that the user will usually enter
		{
			inChar = serialStringInput.charAt(i);
			// if it's a letter
			if (isAlpha(inChar))
			{
				// if it's the last character of the string
				if (i == serialStringInput.length()-1)
				{
					parsedLetterString = serialStringInput.substring(0);		// take the whole string
					break;														// break out of the loop
				}
				continue;
			}
			// if it's anything besides a letter
			else
			{
				// if it's the last character of the string
				if (i == serialStringInput.length()-1)
				{
					parsedLetterString = serialStringInput.substring(0,i);		// Bug: there might need to just be one parameter - 0
					serialInputInt = serialStringInput.substring(i).toInt();	// Make a string out of everything past the nth character (string starts at 0th) onward, then Convert that string to an integer
					break;
				}
				parsedLetterString = serialStringInput.substring(0,i);	// Bug: if "i+1" is greater than the string length, I suspect that this could cause problems, but I don't know

				// not totally sure that this if statement is necessary
				if (isDigit(serialStringInput.charAt(i)))
				{
					serialInputInt = serialStringInput.substring(i).toInt();	// Make a string out of everything past the nth character (string starts at 0th) onward, then Convert that string to an integer
				}
				break;
			}
		}
	}

	/*Serial.println("serialInputInt: " + serialInputInt);
	Serial1.println("serialInputInt: " + serialInputInt);
	Serial.println("LetterString: " + parsedLetterString);
	Serial1.println("LetterString: " + parsedLetterString);*/

	// See if the parsedLetterString flag the user sent matches any of the flags above. If it does, do the tasks associated with that flag: flags[8] = {"c", "r", "l", "b", "rd", "rdp", "party", "track"};
	switch(getDesiredFlagIndex(parsedLetterString))
	{
		case -1:	// there isn't any input from the user
			break;
		case 0:		// "c"
			// If the user sends a string that is longer than 1
			if (serialInputInt > getHighestActivePacerIndex());
				// do nothing
			// If the user didn't enter anything after the string, serialInputInt will still be -1
			else
			{
				if (serialInputInt >= 0)
				{
					if (serialInputInt <= getHighestActivePacerIndex())	// Are we sure that we've cleared a pacer that is smaller than the inputPacer?
					{
						pacer[serialInputInt].setSecondsPerLap(0);		// If the user sends the clear pacer i text string, clear pacer i and reset it to 0
						pacer[serialInputInt].setInitialHighlightedPanel(0);
					}
				}
				else // If the user sends the clear all text string (just "c")
				{
					// set all pacers' secondsPerLap variable to 0
					for (int i = 0; i < pacer[0].getNumberPacers(); i++)
					{
						pacer[i].setSecondsPerLap(0);
						pacer[i].setInitialHighlightedPanel(0);
					}
				}
			}
			break;
		case 1: // "r"
			if (serialInputInt > 0 && serialInputInt <= getHighestActivePacerIndex())
			{
				serialInputInt = serialStringInput.substring(1).toInt();	// Make a string out of everything past the 1th character (string starts at 0th) onward, then Convert that string to an integer
				pacer[serialInputInt].setStartTimeToNow();
			}
			// user sent just "r" to immediately reset all pacers
			else
			{
				// call all pacers' setStartTime() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setStartTime(tempMillis);		// This makes sure that all pacers have the exact same start time. Using a call to myMillis() to calculate the pacer start time would result in slightly different start times for each pacer called
				}
			}
			break; 
		case 2: // "l"
			// If the user sends a string that starts with "l" (lowercase L) and the user sends a digit that is greater than 0
			if (serialStringInput.length() > 1 && serialInputInt > 0)
			{
				// call all pacers' setTotalPacingPanels function and pass it the value received from the user to set the number of lights on the track
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setTotalPacingPanels(serialInputInt);
				}
				// strip.updateLength(serialInputInt);
			}
			break; 
		case 3: // "b"
			// If the user sends an int that is in the range of the indexes for the pacer array
			if (serialInputInt >= 0 && serialInputInt < pacer[0].getNumberPacers())
			{
				pacer[serialInputInt].setIsBackwards(!pacer[serialInputInt].getIsBackwards());		// Make pacer i the opposite of its current backwards-ness
			}
			else // If the user sends the set all backwards text string. When switching the backwards-ness of all pacers, this function looks at the first pacer and flips it and then flips all the others the same way.
			{
				bool tempIsBackwards = pacer[0].getIsBackwards();
				// set all pacers is backwards to the opposite of whatever pacer at index 0 currently is
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setIsBackwards(!tempIsBackwards);
				}
			}
			break; 
		case 4: // "rd"
			// If the user sends "rd" with a digit on the end to set the delay to a specific number of characters
			if (serialStringInput.length() > 2)
			{
				// call all pacers' setStartTime() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setStartTime(tempMillis + ((serialInputInt*1000)));	// sets each pacers' milliseconds startTime to the current time + (the number of seconds the user sent * 1000 milliseconds * the clockAdjustmentFactor); tempMillis makes sure that all pacers have the exact same start time. Using a call to myMillis() to calculate the pacer start time would result in slightly different start times for each pacer called
				}
			}
			// If the user just sends "rd" to reset all pacers on a delay
			else
			{
				// call all pacers' setStartTimeToNowPlusDelay() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setStartTime(tempMillis + resetDelayDefaultDelayTimeMillis);		// This makes sure that all pacers have the exact same start time. Using a call to myMillis() to calculate the pacer start time would result in slightly different start times for each pacer called
				}
			}
			break; 
		case 5: // "rdp"
			if (serialInputInt >= 0 && serialInputInt < pacer[0].getNumberPacers())
			{
				// call this pacer's setStartTimeToNowPlusDelay() function
				pacer[serialInputInt].setStartTimeToNowPlusDelay(resetDelayDefaultDelayTimeMillis);
			}
			break; 
		case 6: // "party"
			partyMode = true;
			break; 
		case 7: // "track"
			partyMode = false;
			break;
		case 8: // "spt"
			setChangedPacerNewStartTime(serialInputInt, serialInputDouble);
			break;
		case 9: // "strip"
			/*if (stripNum == 0)
			{
				Adafruit_WS2801 strip = Adafruit_WS2801(numLEDS, dataPin, clockPin);
				stripNum = 1;
				break;
			}
			if (stripNum == 1)
			{
				Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDS, dataPin, NEO_GRB + NEO_KHZ800);
				stripNum = 0;
				break;
			}*/
			break;
		case 10: // "a" clockAdjustmentFactor - this is weird because it expects "a5" to set clockAdjustmentFactor to .5 and "a10" to set clockAdjustmentFactor to 1.0
			serialInputDouble = (double) serialInputInt / 10;
			clockAdjustmentFactor = serialInputDouble;
			break;
		case 11: // "pct" pacerColorTo - example "pct2,0" changes pacer with index 2 to the color index 0 which is white
			if (serialStringInput.length() > 1 && serialInputInt > -1 && serialInputInt < 7)
			{
				int intHolder = (int) serialInputDouble;
				pacer[serialInputInt].setColorInt(intHolder);
			}
			break;
		case 12: // "v" hidden - example "v2" changes pacer with index 2 to visible so that it is still logically running while not visible to the athlete
			// If the user sends an int that is in the range of the indexes for the pacer array
			if (serialInputInt >= 0 && serialInputInt < pacer[0].getNumberPacers())
			{
				pacer[serialInputInt].setIsVisible(!pacer[serialInputInt].getIsVisible());
			}
			else if (serialInputInt == -1)
			{
				bool tempVisible = pacer[0].getIsVisible();
				// call all pacers' setIsVisible() function
				for (int i = 0; i < pacer[0].getNumberPacers(); i++)
				{
					pacer[i].setIsVisible(!tempVisible);
				}
			}
			break;
		case 13:	// "apb" accelerate pacer by
			setChangedPacerNewStartTime(serialInputInt, pacer[serialInputInt].getSecondsPerLap() + serialInputDouble);
			break;
		case 14:	// "spb" slow pacer by
			setChangedPacerNewStartTime(serialInputInt, pacer[serialInputInt].getSecondsPerLap() + serialInputDouble);
			break;
		default:
			break;
	}
}

// Returns an integer that represents the array index of a string so that it can be used in a switch statement
int getDesiredFlagIndex(String s)
{
	/*for (int i=0; i < TRACK_FLAG_SIZE; i++)	// For each index of the flags array
	{
		if (s.equals(trackFlags[i]))					// If the string sent by the user equals a particular string in the array
			return i;							// Return that string's index
	}
	return -1;									// If no index matches, return -1;
	*/
}

// Returns the lowest available (empty) pacer or return the highest index; returns the index of the lowest pacer instance with getSecondsPerLap() == 0 unless all are greater than 0, in which case it will return the int associated with the instance of the highest pacer
int getLowestUnusedPacerIndex()
{
	/*for (int i = 0; i < pacer[0].getNumberPacers(); i++)
	{
		if (pacer[i].getSecondsPerLap() == 0)
		{
			return i;
		}
	}

	// Return the index of the highest pacer if a lower empty one was not found
	return pacer[0].getNumberPacers()-1;*/
}

// Returns the index of the highest pacer instance with getSecondsPerLap() > 0 or returns -1 if no pacers have getSecondsPerLap > 0
int getHighestActivePacerIndex()
{
	/*for (int i = pacer[0].getNumberPacers()-1; i > -1; i--)
	{
		if (pacer[i].getSecondsPerLap() > 0)
		{
			return i;
		}
	}
	return -1;*/
}

// Computes the Lowest Delayed Pacer only one time per trafficLightCountTo times that the main loop runs
void computeLowestDelayedPacer()
{
	/*if (trafficLightIterator >= trafficLightCountTo)
	{
		int highest_Active_Pacer = getHighestActivePacerIndex();
		for (int i=0; i <= highest_Active_Pacer; i++)	// used with the if statement to get the lowest delayed pacer index
		{
			if (pacer[i].isCurrentlyDelayed())
			{
				tempLowestDelayedPacerIndex = i;
				return;
			}
		}
	}
	else
	{
		trafficLightIterator++;
	}*/
}

// Give delayed pacers a traffic light countdown
void delayedPacerTrafficLightCountdown()
{
	/*if (tempLowestDelayedPacerIndex <= -1)
	{
		computeLowestDelayedPacer();
	}
	else if (tempLowestDelayedPacerIndex > -1)
	{
		pacerCountdown();
	}*/
}

// Gives delayed pacers a countdown similar to a traffic light at wherever they are starting in whatever direction they are running
void pacerCountdown()
{
	// if it's backwards (end to beginning)
	/*if (pacer[tempLowestDelayedPacerIndex].getIsBackwards())
	{
		if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds))
		{
			if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds))
			{
				if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(-1))
				{
					if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(-4))
					{
						leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel()-1)%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (255,0,0);	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
						return;
					}
					else
					{
						trafficLightIterator = 0;
						tempLowestDelayedPacerIndex = -1;
						return;
					}
				}
				leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel()-2)%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (255,255,0); // yellow
				return;
			}
			leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel()-3)%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (255,0,0);	// red
			return;
		}
	}
	// If it's frontwards (beginning to end)
	else
	{
		if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds))
		{
			if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds))
			{
				if (pacer[tempLowestDelayedPacerIndex].isStartTimeWithinXSecondsOnly(trafficLightCountDownDarkSeconds))
				{
					if (pacer[tempLowestDelayedPacerIndex].isCurrentlyDelayed())
					{
						leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel())%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (0,0,0);	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
						return;
					}
					else
					{
						trafficLightIterator = 0;
						tempLowestDelayedPacerIndex = -1;
						return;
					}
				}
				leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel()+1)%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (255,255,0); // yellow
				return;
			}
			leds[(pacer[tempLowestDelayedPacerIndex].getInitialHighlightedPanel()+2)%pacer[tempLowestDelayedPacerIndex].getTotalPacingPanels()] = CHSV (255,0,0);	// red
			return;
		}
	}*/
}

//***************************
//****Party Functions
//***************************

// Switch statement that controls which "party" function will run
void partyFunctions()
{
	// switch statement with party int as the value for choosing cases that calls different party functions based on its value
	/*switch(partyInt)
	{
		// {"red wipe", "green wipe", "blue wipe", "rainbow", "rainbow cycle", "red wipe", "red wipe", "scanner", "multi-color dither", "multi-color colorchase", "multi-color wipe"}
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
		case 7:
			// scanner
			scanner(127,0,0, 30);        // red, slow
			scanner(0,0,127, 15);        // blue, fast
			break;
		case 8:
			// dither
			dither(Color(0,127,127), 50);       // cyan, slow
			dither(Color(0,0,0), 15);           // black, fast
			dither(Color(127,0,127), 50);       // magenta, slow
			dither(Color(0,0,0), 15);           // black, fast
			dither(Color(127,127,0), 50);       // yellow, slow
			dither(Color(0,0,0), 15);           // black, fast
			break;
		case 9:
			// color chase
			colorChase(Color(127,127,127), 20); // white
			colorChase(Color(127,0,0), 20);     // red
			colorChase(Color(127,127,0), 20);   // yellow
			colorChase(Color(0,127,0), 20);     // green
			colorChase(Color(0,127,127), 20);   // cyan
			colorChase(Color(0,0,127), 20);     // blue
			colorChase(Color(127,0,127), 20);   // magenta
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
	}*/
}

// Chase a dot down the strip
// good for testing purposes
void colorChase(uint32_t c, uint8_t wait) 
{
	//int i;

	//for (i=0; i < FastLED.size(); i++) //strip.numPixels();
	//{
	//	leds[i] = CHSV (0,0,0);  // turn all pixels off
	//}

	//for (i=0; i < FastLED.size(); i++) // strip.numPixels();
	//{
	//	strip.setPixelColor(i, c); // set one pixel
	//	strip.show();              // refresh strip display
	//	delay(wait);               // hold image for a moment
	//	strip.setPixelColor(i, 0); // erase pixel (but don't refresh yet)
	//}
	//strip.show(); // for last erased pixel
}

// An "ordered dither" fills every pixel in a sequence that looks
// sparkly and almost random, but actually follows a specific order.
void dither(uint32_t c, uint8_t wait) 
{
	//// Determine highest bit needed to represent pixel index
	//int hiBit = 0;
	//int n = FastLED.size() - 1; // strip.numPixels()
	//for(int bit=1; bit < 0x8000; bit <<= 1) 
	//{
	//	if(n & bit) hiBit = bit;
	//}

	//int bit, reverse;
	//for(int i=0; i<(hiBit << 1); i++) 
	//{
	//	// Reverse the bits in i to create ordered dither:
	//	reverse = 0;
	//	for(bit=1; bit <= hiBit; bit <<= 1) 
	//	{
	//		reverse <<= 1;
	//		if(i & bit) reverse |= 1;
	//	}
	//	strip.setPixelColor(reverse, c);
	//	strip.show();
	//	
	//	delay(wait);
	//}
	//delay(250); // Hold image for 1/4 sec
}

// "Larson scanner" = Cylon/KITT bouncing light effect
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) 
{
	//int i, j, pos, dir;

	//pos = 0;
	//dir = 1;

	//for(i=0; i<((FastLED.size()-1) * 8); i++) // strip.numPixels()
	//{
	//	// Draw 5 pixels centered on pos.  setPixelColor() will clip
	//	// any pixels off the ends of the strip, no worries there.
	//	// we'll make the colors dimmer at the edges for a nice pulse
	//	// look
	//	strip.setPixelColor(pos - 2, Color(r/4, g/4, b/4));
	//	strip.setPixelColor(pos - 1, Color(r/2, g/2, b/2));
	//	strip.setPixelColor(pos, Color(r, g, b));
	//	strip.setPixelColor(pos + 1, Color(r/2, g/2, b/2));
	//	strip.setPixelColor(pos + 2, Color(r/4, g/4, b/4));

	//	strip.show();
	//	
	//	delay(wait);
	//	// If we wanted to be sneaky we could erase just the tail end
	//	// pixel, but it's much easier just to erase the whole thing
	//	// and draw a new one next time.
	//	for(j=-2; j<= 2; j++)
	//	{
	//		strip.setPixelColor(pos+j, Color(0,0,0));
	//	}
	//	// Bounce off ends of strip
	//	pos += dir;
	//	if(pos < 0) 
	//	{
	//		pos = 1;
	//		dir = -dir;
	//	} 
	//	else if(pos >= strip.numPixels()) 
	//	{
	//		pos = strip.numPixels() - 2;
	//		dir = -dir;
	//	}
	//}
}
void rainbow(uint8_t wait) 
{
	//int i, j;
 //  
	//for (j=0; j < 256; j++)     // 3 cycles of all 256 colors in the wheel
	//{
	//	for (i=0; i < strip.numPixels(); i++) 
	//	{
	//		strip.setPixelColor(i, Wheel( (i + j) % 255));
	//	}  
	//	strip.show();   // write all the pixels out
	//	
	//	delay(wait);
	//}
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait)
{
	//int i, j;
 // 
	//for (j=0; j < 256 * 5; j++)     // 5 cycles of all 25 colors in the wheel
	//{
	//	for (i=0; i < FastLED.size(); i++) // strip.numPixels()
	//	{
	//		// tricky math! we use each pixel as a fraction of the full 96-color wheel
	//		// (thats the i / strip.numPixels() part)
	//		// Then add in j which makes the colors go around per pixel
	//		// the % 96 is to make the wheel cycle around
	//		strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
	//	}  
	//	strip.show();   // write all the pixels out
	//	
	//	delay(wait);
	//}
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) 
{
	//int i;
 // 
	//for (i=0; i < FastLED.size(); i++) // strip.numPixels()
	//{
	//	strip.setPixelColor(i, c);
	//	strip.show();
	//	
	//	delay(wait);
	//}
	//setSerialInput();
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