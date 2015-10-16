#include <FastLED.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

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
	CRGB shade;		//Color shade;		//UI// This determines the color of the pacer's pacing lights
	bool isStopwatchStarted;
	bool isBackwards;
	bool isGoingToChangeSpeed;				// indicates whether the pacer is about to change its speed
	bool isVisible;							// indicates whether the pacer should be visible or not (the color black / it logically keeps running)
	long startTime, futureStartTime;
	static int numberPacers; // static
	int currentHighlightedPacingPanel;		// This determines which pacing panel is currently lit up
	int colorInt;							// Determines which color index in the color array that the pacer should be
	double trafficLightCountDownRedSeconds = 7, trafficLightCountDownYellowSeconds = 4, trafficLightCountDownDarkSeconds = 2; // Traffic light countdown variables for red, yellow, and dark/go

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
		startTime = millis() + (long)initialDelay;
		double lapTimesArray[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		shade = CRGB::White;
	};
	int getNumberPacers()
	{
		return numberPacers;
	};
	CRGB getShade()
	{
		return shade;
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
	bool getIsBackwards()
	{
		return isBackwards;
	};
	long getDelayRemaining()
	{
		return startTime - millis();
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
	int getCurrentHighlightedPacingPanel()
	{
		long tempStartTime = getStartTime();
		long temp_Millis = millis();
		if (tempStartTime > (temp_Millis-0)) // make this temp_Millis - 1000 if you want to show green when it's go time
		{
			if (isBackwards)
			{
				if ((tempStartTime - temp_Millis) > (trafficLightCountDownDarkSeconds*1000))
				{
					if ((tempStartTime - temp_Millis) > (trafficLightCountDownYellowSeconds*1000))
					{
						if ((tempStartTime - temp_Millis) > (trafficLightCountDownRedSeconds*1000))
						{
							return (getInitialHighlightedPanel()-3)%getTotalPacingPanels();
						}
						return (getInitialHighlightedPanel()-2)%getTotalPacingPanels();
					}
					return (getInitialHighlightedPanel()-1)%getTotalPacingPanels();
				}
			}
			else // frontwards
			{
				if ((tempStartTime - temp_Millis) > (trafficLightCountDownDarkSeconds*1000))
				{
					if ((tempStartTime - temp_Millis) > (trafficLightCountDownYellowSeconds*1000))
					{
						if ((tempStartTime - temp_Millis) > (trafficLightCountDownRedSeconds*1000))
						{
							return (getInitialHighlightedPanel()+2)%getTotalPacingPanels();
						}
						return (getInitialHighlightedPanel()+1)%getTotalPacingPanels();
					}
					return (getInitialHighlightedPanel()+0)%getTotalPacingPanels();
				}
			}
		}
		else
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
		if (millis() > getStartTime())
		return millis() - getStartTime();		// if the fix doesn't work, just leave this line
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
	void setShade(CRGB newShade)
	{
		shade = newShade;
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
		startTime = millis();
	};
	void setStartTimeToNowPlusDelay(long delayMillis)
	{
		startTime = millis() + delayMillis;
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

// How many leds in your strip?
#define NUM_LEDS 26

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

//***********************************************
// Declarations: Related to the pacer objects
//***********************************************
const int PACER_ARRAY_SIZE = 10;
Pacer pacer[PACER_ARRAY_SIZE] = {Pacer(2,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS) };
CRGB colorArray[8] = {CRGB::White, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Orange, CRGB::Purple, CRGB::Pink};
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
long resetDelayDefaultDelayTimeMillis = 10000;		// the default delay time when resetting pacers on a delay
int serial1AvailableIterator = 0, serial1FeedbackIterator = 0, serialFeedbackIterator = 0, trafficLightIterator = 0;
int serialCountTo = 50, trafficLightCountTo = 100, partySerialCountTo = 5;
int tempLowestDelayedPacerIndex = -1;

bool isChangePacerSpeedNeeded = false; // trigger to determine whether we need to figure out which pacer is going to change its speed
bool partyMode = false;				// This mode (now partyMode) String (now bool) has two possible values: "track" and "party" (now true and false). Each value will result in different function calls
bool isColorsNormal = true;			// temporary for testing
int partyInt = 10;					// This integer controls what party functions will be run; 0 indicates all will be run

//***********************************************
// Declarations: All variables not predefined
//***********************************************
double secondsPerLapHolder;
long tempMillis;		// holds the milliseconds time that a pacer's speed will change
double newSecondsPerLap; // holds the double (floating point) time that will become the pacer's secondsPerLap

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
YunServer server;


void setup() { 
      /* Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);*/
       FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  	  /* FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);*/

	   assignPacerColors();
	   
	   Bridge.begin();

	   // Listen for incoming connection only from localhost
	   // (no one from the external network could connect)
	   server.listenOnLocalhost();
	   server.begin();
}

void loop() { 
  
	setPixelColorBasedOnTime();
	
	// Get clients coming from server
	YunClient client = server.accept();

	// There is a new client?
	if (client)
	{
		// Process request
		process(client);

		// Close connection and free resources.
		client.stop();
	}
	// getSerialFeedback(); // This overwhelms the program if too many pacers are added
}

void assignPacerColors()
{
	for (int i=0; i < pacer[0].getNumberPacers(); i++)
	{
		pacer[i].setShade(colorArray[i%8]);
	}
}

void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "time") {
    timeCommand(client);
  }

  if (command == "clear") {
    pacerCommand(client, command);
  }

  if (command == "reset") {
    pacerCommand(client, command);
  }

  if (command == "resetdelay") {
    pacerCommand(client, command);
  }

  if (command == "visible") {
    pacerCommand(client, command);
  }

  if (command == "backwards") {
    pacerCommand(client, command);
  }

  if (command == "color") {
    pacerCommand(client, command);
  }

}

void pacerCommand(YunClient client, String receivedCommand) {

	String command2 = client.readString();
	int value = command2.toInt();

	if (receivedCommand == "clear"){
		if (isnan(command2.toInt()) != 1){
			if (value == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setSecondsPerLap(0);
				}
				return;
			}
			if (value > -1){
				pacer[value].setSecondsPerLap(0);
			}
		}
	}

	if (receivedCommand == "reset"){
		if (isnan(command2.toInt()) != 1){
			if (value == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTimeToNow();
				}
				return;
			}
			if (value > -1){
				pacer[value].setStartTimeToNow();
			}
		}
	}

	if (receivedCommand == "resetdelay"){
		if (isnan(command2.toInt()) != 1){
			if (value == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTimeToNowPlusDelay(10000);
				}
				return;
			}
			if (value > -1){
				pacer[value].setStartTimeToNowPlusDelay(10000);
			}
		}
	}
	if (receivedCommand == "visible"){
		if (isnan(command2.toInt()) != 1){
			if (value == 99){
				for (int i=0; i <= pacer[0].getNumberPacers(); i++){
					pacer[i].setIsVisible(!pacer[i].getIsVisible());
				}
				return;
			}
			if (value > -1){
				pacer[value].setIsVisible(!pacer[value].getIsVisible());
			}
		}
	}
	if (receivedCommand == "backwards"){
		if (isnan(command2.toInt()) != 1){
			if (value == 99){
				for (int i=0; i <= pacer[0].getNumberPacers(); i++){
					pacer[i].setIsBackwards(!pacer[i].getIsBackwards());
				}
				return;
			}
			if (value > -1){
				pacer[value].setIsBackwards(!pacer[value].getIsBackwards());
			}
		}
	}
	if (receivedCommand == "color"){
		if (client.read() == '/'){
			int command3 = client.parseFloat();
			if (value == 99){
				if (isColorsNormal){
					for (int i=0; i <= pacer[0].getNumberPacers(); i++){
						pacer[i].setShade(colorArray[command3%8]);
					}
					isColorsNormal = false;
					return;
				}
				else {
					assignPacerColors();
					isColorsNormal = true;
					return;
				}
			}
			else if (value > -1){
				for (int i=0; i <= pacer[0].getNumberPacers(); i++){
					pacer[value].setShade(colorArray[command3%8]);
				}
				return;
			}
		}
	}
}

void timeCommand(YunClient client) {
  double new_SecondsPerLap;
  int pacerIndex;

  // Read pin number
  pacerIndex = client.readStringUntil('/').toInt();

  new_SecondsPerLap = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (pacerIndex == 99) {
	pacer[getLowestUnusedPacerIndex()].setStartTime(millis());
	pacer[getLowestUnusedPacerIndex()].setSecondsPerLap (new_SecondsPerLap);
  }
  else {
    pacer[pacerIndex].setStartTime(millis());
	pacer[pacerIndex].setSecondsPerLap (new_SecondsPerLap);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pacerIndex);
  client.print(F(" set to "));
  client.println(new_SecondsPerLap);

  // Update datastore key with the current pin value
  String key = "D";
  key += pacerIndex;
  Bridge.put(key, String(new_SecondsPerLap));
}

void processo(YunClient client) {
  int value;

  // Read pin number
  value = client.parseInt();
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



  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  /*if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  }*/
}

// Set Each Pixel's color based on what the what the current highlighted pixel (formerly, pacing panel) should be
void setPixelColorBasedOnTime()
{
	// Turn every light off
	for (int i=0; i < FastLED.size(); i++)
	{
		leds[i] = CRGB::Black; // CHSV( 0, 0, 0);
	}

	// Place the pacers where they are supposed to be with the correct color
	for (int j=0; j < getHighestActivePacerIndex()+1; j++)		// This can be changed to j < inputPacer (test with actual lights to be sure)
	{
		if (pacer[j].getSecondsPerLap() > 0 && pacer[j].getIsVisible())
		{
			leds[pacer[j].getCurrentHighlightedPacingPanel()] = pacer[j].getShade(); // CHSV( 255, 255, 255);

			// strip.setPixelColor(pacer[j].getCurrentHighlightedPacingPanel(), getColorFromInt(pacer[j].getColorInt())); // set one pixel
		}
	}

	// Traffic Light Countdown for delayed pacers
	// delayedPacerTrafficLightCountdown();

	FastLED.show();              // refresh strip display

}

// send input from user via the Serial Monitor Tool to send to the Arduino device
void setSerialInput()
{
	// This could be an if statement or a while statement; an if statement will run the main loop between each serial input, but a while loop will process all the serial input and then return to the main loop

	while (Serial.available()) // Serial1.available() || Serial.available()
	{
		tempMillis = millis();
		/*if (Serial1.available())
		{
			serialStringInput = Serial1.readStringUntil(' ');	// Serial1 processes serial input data from a mobile bluetooth connection
			//serialStringInput.trim();
		}*/
		// else
		// {
			serialStringInput = Serial.readStringUntil(' ');	// Serial processes serial data input from a USB connection
			//serialStringInput.trim();
		// }

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
			// setChangedPacerNewStartTime(serialInputInt, serialInputDouble);
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
			// clockAdjustmentFactor = serialInputDouble;
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
			// setChangedPacerNewStartTime(serialInputInt, pacer[serialInputInt].getSecondsPerLap() + serialInputDouble);
			break;
		case 14:	// "spb" slow pacer by
			// setChangedPacerNewStartTime(serialInputInt, pacer[serialInputInt].getSecondsPerLap() + serialInputDouble);
			break;
		default:
			break;
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
		printThis.concat(millis());
		printThis.concat("\", \"hr\": \"");
		printThis.concat((millis()/3600000));
		printThis.concat("\", \"min\": \"");
		printThis.concat((millis()/60000)%(60));
		printThis.concat("\", \"sec\": \"");
		printThis.concat((millis()/1000)%(60));
		printThis.concat("\", \"millis\": \"");
		printThis.concat(millis()%1000);
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
				// printThis.concat(getColorWord(pacer[i].getColorInt()));
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

	/*if (serial1FeedbackIterator >= serialCountTo)				// bluetooth mobile feedback
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

		// Serial1.println(printThis);

		serial1FeedbackIterator = 0;
	}
	else
	{
		serial1FeedbackIterator++;
	}*/
}

// Checks for "track" as input sent from the user to change the mode from something else (like "party" mode)
void checkTrackModeFlags()
{
	if (serialStringInput == trackFlags[7])
	{
		partyMode = false;
	}
}

// Returns an integer that represents the array index of a string so that it can be used in a switch statement
int getDesiredFlagIndex(String s)
{
	for (int i=0; i < TRACK_FLAG_SIZE; i++)	// For each index of the flags array
	{
		if (s.equals(trackFlags[i]))					// If the string sent by the user equals a particular string in the array
			return i;							// Return that string's index
	}
	return -1;									// If no index matches, return -1;
}

// Returns the lowest available (empty) pacer or return the highest index; returns the index of the lowest pacer instance with getSecondsPerLap() == 0 unless all are greater than 0, in which case it will return the int associated with the instance of the highest pacer
int getLowestUnusedPacerIndex()
{
	for (int i = 0; i < pacer[0].getNumberPacers(); i++)
	{
		if (pacer[i].getSecondsPerLap() == 0)
		{
			return i;
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
		}
	}
	return -1;
}