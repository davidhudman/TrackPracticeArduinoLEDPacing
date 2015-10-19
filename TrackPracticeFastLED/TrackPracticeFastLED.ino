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
int totalPacingPanels = 26;
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
	   assignGetTotalPacingPanels(totalPacingPanels);
	   
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
	if (client) {
		// Process request
		process(client);

		// Close connection and free resources.
		client.stop();
	}
	// getSerialFeedback(); // This overwhelms the program if too many pacers are added
}

void assignGetTotalPacingPanels(int total_Pacing_Panels)
{
	for (int i=0; i < pacer[0].getNumberPacers(); i++) {
		pacer[i].setTotalPacingPanels(total_Pacing_Panels);
	}
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

  if (command == "color") {
	  multiIntCommand(client, command);
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
	/*if (receivedCommand == "color"){
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
	}*/
}

void multiIntCommand(YunClient client, String receivedCommand) {
  int pacerIndex;
  int thirdCommand;

  // Read pin number
  pacerIndex = client.readStringUntil('/').toInt();

  thirdCommand = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (receivedCommand == "color") {
	  if (pacerIndex == 99) {
		  if (isColorsNormal){
			  for (int i=0; i < pacer[0].getNumberPacers(); i++) {
				  pacer[i].setShade(colorArray[thirdCommand%8]);
			  }
			  isColorsNormal = false;
			  if (thirdCommand == 99) {
				  assignPacerColors();
			  }
			  return;
		  }
		  else {
			  assignPacerColors();
			  isColorsNormal = true;
			  return;
		  }
	  }
	  else {
		pacer[pacerIndex].setShade(colorArray[thirdCommand%8]);
	  }
  }

  /*if (receivedCommand == "lights") {
	  if (pacerIndex == 99) {
		  for (int i=0; i < pacer[0].getNumberPacers(); i++) {
			  assignGetTotalPacingPanels(thirdCommand);
		  }
	  }
	  else {
		  assignGetTotalPacingPanels(thirdCommand);
	  }
  }*/
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