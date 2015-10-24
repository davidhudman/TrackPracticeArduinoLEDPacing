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
	Pacer(double lapSecs, double initDelay, int firstHighlightedPanel, int meters, int light_Train_Length, int total_Pacing_Panels) { // Constructor
		secondsPerLap = lapSecs;
		initialDelay = initDelay;
		currentHighlightedPacingPanel = firstHighlightedPanel;
		initialHighlightedPanel = firstHighlightedPanel;
		numMeters = meters;
		colorInt = numberPacers;
		lightTrainLength = light_Train_Length;
		isStopwatchStarted = false;
		isBackwards = false;
		isGoingToChangeSpeed = false;
		isVisible = true;
		totalPacingPanels = total_Pacing_Panels;
		numberPacers++;
		startTime = millis() + (long)initialDelay;
		double lapTimesArray[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	};
	int getNumberPacers() {
		return numberPacers;
	};
	bool getIsStopwatchStarted() {
		return isStopwatchStarted;
	};
	bool getIsGoingToChangeSpeed() {
		return isGoingToChangeSpeed;
	};
	bool isCurrentlyDelayed() {
		if ((startTime - millis()) > 0) {
			return true;
		}
		else return false;
	};
	bool isStartTimeWithinXSecondsAndGreaterThanZero(int seconds) {
		if (((startTime - millis()) > 0) && ((startTime - millis()) < (seconds*1000))) {
			return true;
		}
		else return false;
	};
	// is the time remaining until the start less than the parameter number of seconds
	bool isStartTimeWithinXSecondsOnly(int seconds) {
		if ((startTime - millis()) < (seconds*1000)) {
			return true;
		}
		else return false;
	};
	bool getIsBackwards() {
		return isBackwards;
	};
	long getDelayRemaining() {
		return startTime - millis();
	}
	double getSecondsPerLap() {
		return secondsPerLap;
	};
	double getFutureSecondsPerLap() {
		return futureSecondsPerLap;
	};
	int getTotalPacingPanels() {
		return totalPacingPanels;
	};
	int getLightTrainLength() {
		return lightTrainLength;
	};
	double getInitialDelay() {
		return initialDelay;
	};
	int getInitialHighlightedPanel() {
		if (isBackwards) {
			return getTotalPacingPanels()-initialHighlightedPanel;
		}
		else {
			return initialHighlightedPanel;
		}
	};
	int getNumMeters() {
		return numMeters;
	};
	double getNextLightDelay() {		// The delay between pacing panels lighting up. This is derived from the number of seconds that it takes for the pacing panels to complete a lap and the total number of pacing panels
		return ((getSecondsPerLap() / (double)getTotalPacingPanels()*1000));
	};
	int getCurrentHighlightedPacingPanel() {
		long tempStartTime = getStartTime();
		long temp_Millis = millis();
		// if it's backwards (end to beginning)
		if (tempStartTime > (temp_Millis-0)) { // make this temp_Millis - 1000 if you want to show green when it's go time
			if (getIsBackwards()) {
				if (isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds)) {
					if (isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds)) {
						if (isStartTimeWithinXSecondsOnly(trafficLightCountDownDarkSeconds)) {
							if (isStartTimeWithinXSecondsOnly(0)) {
								return (getInitialHighlightedPanel())%getTotalPacingPanels();	// green
								// setVisible(true);
							}
							// setVisible(false);
							return (getInitialHighlightedPanel()+1)%getTotalPacingPanels();	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
						}
						return (getInitialHighlightedPanel()-1)%getTotalPacingPanels(); // yellow
					}
					return (getInitialHighlightedPanel()-2)%getTotalPacingPanels(); // red
				}
				return (getInitialHighlightedPanel()-2)%getTotalPacingPanels(); // doesn't really matter what number this is
			}
			// If it's frontwards (beginning to end)
			else {
				if (isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds)) {
					if (isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds)) {
						if (isStartTimeWithinXSecondsOnly(trafficLightCountDownDarkSeconds)) {
							if (isStartTimeWithinXSecondsOnly(0)) {
								return (getInitialHighlightedPanel())%getTotalPacingPanels();	// green
								// setVisible(true);
							}
							// setVisible(false);
							return (getInitialHighlightedPanel()-1)%getTotalPacingPanels();	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
						}
						return (getInitialHighlightedPanel()+1)%getTotalPacingPanels(); // yellow
					}
					return (getInitialHighlightedPanel()+2)%getTotalPacingPanels(); // red
				}
				return (getInitialHighlightedPanel()+2)%getTotalPacingPanels(); // doesn't really matter what number this is
			}
		}
		else {
			if (!isBackwards) {	// if isBackwards is not true
				currentHighlightedPacingPanel = (int)(((getRunningTime()%(long)(getSecondsPerLap()*(double)1000))/getNextLightDelay())+getInitialHighlightedPanel())%getTotalPacingPanels();
				return currentHighlightedPacingPanel;
			}
			else {
				currentHighlightedPacingPanel = (getTotalPacingPanels()-1) - (int)(((getRunningTime()%(long)(getSecondsPerLap()*(double)1000))/getNextLightDelay())+getInitialHighlightedPanel())%getTotalPacingPanels();
				return currentHighlightedPacingPanel;
			}
		}
	};
	long getStartTime() {
		return startTime;
	};
	long getFutureStartTime() {
		return futureStartTime;
	};
	long getRunningTime() {
		// This if statement is meant to solve the problem of pacing panels running before their delay
		if (millis() > getStartTime())
		return millis() - getStartTime();		// if the fix doesn't work, just leave this line
		else
		return 0;
	};
	int getColorInt() {
		/*long tempStartTime = getStartTime();
		long temp_Millis = millis();
		// if it's backwards (end to beginning)
		if (tempStartTime > (temp_Millis-1000)) { // make this temp_Millis - 1000 if you want to show green when it's go time
			if (isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds)) {
				if (isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds)) {
					if (isStartTimeWithinXSecondsOnly(trafficLightCountDownDarkSeconds)) {
						if (isStartTimeWithinXSecondsOnly(0)) {
							return 1;	// green
						}
						return 2; // yellow (now with change) - would've been: black or "off" - it doesn't really matter what the color is because we're turning visibility off
					}
					return 2; // yellow
				}
				return 3; // red
			}
			return colorInt;
		}
		else */
		{
			return colorInt;
		}
	}
	bool getIsVisible() {
		return isVisible;
	}
	void setIsVisible(bool is_Visible) {
		isVisible = is_Visible;
	}
	void setColorInt(int i) {
		colorInt = i;
	}
	void setTotalPacingPanels(int total_Pacing_Panels_) {
		totalPacingPanels = total_Pacing_Panels_;
	};
	void setStartTimeToNow() {
		startTime = millis();
	};
	void setStartTimeToNowPlusDelay(long delayMillis) {
		startTime = millis() + delayMillis;
	};
	void setStartTime(long start_Time) {
		startTime = start_Time;
	};
	void setFutureStartTime(long future_Start_Time) {
		futureStartTime = future_Start_Time;
	}
	void setCurrentHighlightedPacingPanel (int current_Highlighted_Pacing_Panel) {
		currentHighlightedPacingPanel = current_Highlighted_Pacing_Panel;
	};
	void setIsStopwatchStarted(boolean is_Stopwatch_Started) {
		isStopwatchStarted = is_Stopwatch_Started;
	};
	void setIsBackwards(boolean is_Backwards) {
		isBackwards = is_Backwards;
	};
	void setIsGoingToChangeSpeed(boolean is_Going_To_Change_Speed) {
		isGoingToChangeSpeed = is_Going_To_Change_Speed;
	};
	void setNumberPacers(int number_Pacers) {
		numberPacers = number_Pacers;
	};
	void setSecondsPerLap(double seconds_Per_Lap) {
		secondsPerLap = seconds_Per_Lap;
	};
	void setFutureSecondsPerLap(double future_Seconds_Per_Lap) {
		futureSecondsPerLap = future_Seconds_Per_Lap;
	};
	void setInitialDelay(int initDelay) {
		initialDelay = initDelay;
	};
	void setInitialHighlightedPanel(int firstHighlightedPanel) {
		initialHighlightedPanel = firstHighlightedPanel;
	};
	void setNumMeters(int meters) {
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
const int COLOR_ARRAY_SIZE = 8;
CRGB colorArray[COLOR_ARRAY_SIZE] = {CRGB::White, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Orange, CRGB::Purple, CRGB::Pink};
int totalPacingPanels = 26;
// const int COLOR_ARRAY_SIZE = 7;
// const String colorName[COLOR_ARRAY_SIZE] = {"white", "green", "yellow", "red", "magenta", "blue", "cyan"};
		// white, red, yellow, green, cyan, blue, magenta

//***********************************************
// Declarations: Related to strings
//***********************************************
// const int TRACK_FLAG_SIZE = 15, PARTY_FLAG_SIZE = 11;
// const String trackFlags[TRACK_FLAG_SIZE] = {"c", "r", "l", "b", "rd", "rdp", "party", "track", "spt", "strip", "a", "pct", "v", "apb", "spb"};	// This array is used to make a hashmap so that I can associate the index of the array with an integer for a switch statement
// const String partyFlags[PARTY_FLAG_SIZE] = {"red wipe", "green wipe", "blue wipe", "rainbow", "rainbow cycle", "red wipe", "red wipe", "scanner", "multi-color dither", "multi-color colorchase", "multi-color wipe"};	// This array is used to make a hasmap so I can associate the index of the array with its party function
// String stringSepFlag = ",";	// holds the string that separates the values in the speed change function
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

// Listen to the default port 5555, the YÃºn webserver
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

// Use an integer parameter to determine what RGB color value it is associated with
CRGB getColorFromInt(int i)
{
	return colorArray[i%COLOR_ARRAY_SIZE];
}

void assignGetTotalPacingPanels(int total_Pacing_Panels) {
	for (int i=0; i < pacer[0].getNumberPacers(); i++) {
		pacer[i].setTotalPacingPanels(total_Pacing_Panels);
	}
}

void assignPacerColors() {
	for (int i=0; i < pacer[0].getNumberPacers(); i++) {
		pacer[i].setColorInt(i);
	}
}

void process(YunClient client) {
  // read the command
  int command = client.readStringUntil('/').toInt();

  // is "digital" command?
  switch(command)
  {
	  case 0:	// clear
		  pacerCommand(client, command);
		  break;
	  case 1:	// reset
		  pacerCommand(client, command);
		  break;
	  case 2:	// resetdelay
		  pacerCommand(client, command);
		  break;
	  case 3:	// visible
		  pacerCommand(client, command);
		  break;
	  case 4:	// backwards
		  pacerCommand(client, command);
		  break;
	  case 5:	// color
		  multiIntCommand(client, command);
		  break;
	  case 6:	// lights
		  break;
	  case 7:	// time
		  timeCommand(client);
		  break;
	  case 8:	// change speed by
		  multiIntCommand(client, command);
		  break;
	  default:
		  break;
  }
  /*if (command == 0) { // clear
    pacerCommand(client, command);
  }

  if (command == 1) { // reset
    pacerCommand(client, command);
  }

  if (command == 2) { // reset delay
    pacerCommand(client, command);
  }

  if (command == 3) { // visible
    pacerCommand(client, command);
  }

  if (command == 4) { // backwards
    pacerCommand(client, command);
  }

  if (command == 5) { // color
	  multiIntCommand(client, command);
  }

  if (command == 7) { // time
	  timeCommand(client);
  }*/
}

void pacerCommand(YunClient client, int receivedCommand) {

	String command2 = client.readString();
	int value = command2.toInt();

	if (receivedCommand == 0){ // clear
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

	if (receivedCommand == 1){ // reset
		if (isnan(command2.toInt()) != 1){
			tempMillis = millis();
			if (value == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTime(tempMillis);
				}
				return;
			}
			if (value > -1){
				pacer[value].setStartTime(tempMillis);
			}
		}
	}

	if (receivedCommand == 2){ // resetdelay
		if (isnan(command2.toInt()) != 1){
			tempMillis = millis();
			if (value == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTime(tempMillis+resetDelayDefaultDelayTimeMillis);
				}
				return;
			}
			if (value > -1){
				pacer[value].setStartTime(tempMillis+resetDelayDefaultDelayTimeMillis);
			}
		}
	}
	if (receivedCommand == 3){ //visible
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
	if (receivedCommand == 4){ // backwards
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

void multiIntCommand(YunClient client, int receivedCommand) {
  int pacerIndex;
  int thirdCommand;

  // Read pin number
  pacerIndex = client.readStringUntil('/').toInt();

  thirdCommand = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (receivedCommand == 5) { // color
	  if (pacerIndex == 99) {
		  if (isColorsNormal){
			  for (int i=0; i < pacer[0].getNumberPacers(); i++) {
				  pacer[i].setColorInt(thirdCommand);
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
		  pacer[pacerIndex].setColorInt(thirdCommand);
	  }
  }
  if (receivedCommand == 8) {  // change speed in flight
	  if (pacerIndex == 99) {
			for (int i=0; i < pacer[0].getNumberPacers(); i++) {
				if (pacer[i].getSecondsPerLap() > 0) {
					setChangedPacerNewStartTime(i, pacer[i].getSecondsPerLap()-thirdCommand);
				}
			}
	  }
	  else {
		  if (pacer[pacerIndex].getSecondsPerLap() > 0) {
			  setChangedPacerNewStartTime(pacerIndex, pacer[pacerIndex].getSecondsPerLap()-thirdCommand);
		  }
	  }
  }

  /*if (receivedCommand == 6) { // lights
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
void setPixelColorBasedOnTime() {
	// Turn every light off
	for (int i=0; i < FastLED.size(); i++) {
		leds[i] = CRGB::Black; // CHSV( 0, 0, 0);
	}

	// Place the pacers where they are supposed to be with the correct color
	for (int j=0; j < getHighestActivePacerIndex()+1; j++) {		// This can be changed to j < inputPacer (test with actual lights to be sure)
		if (pacer[j].getSecondsPerLap() > 0 && pacer[j].getIsVisible()) {
			leds[pacer[j].getCurrentHighlightedPacingPanel()] = getColorFromInt(pacer[j].getColorInt());
			// strip.setPixelColor(pacer[j].getCurrentHighlightedPacingPanel(), getColorFromInt(pacer[j].getColorInt())); // set one pixel
		}
	}
	FastLED.show();              // refresh strip display
}

// returns the new start time for a pacer intersecting the given pacer at the current light its on given the desired seconds per lap; change the Speed of the Pacer To (command "spt")
void setChangedPacerNewStartTime(int index, double new_SecondsPerLap)
{
	long temp_Millis, startTime = pacer[index].getStartTime(), new_startTime, currentTime = millis(), divisionResult;
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
	pacer[index].setInitialHighlightedPanel(initialHighlightedPanel);
	// tempMillis = temp_Millis;

	// then use the new getRunningTime (actually tempMillis and the new_SecondsPerLap to solve for startTime
	// new_startTime = -(long)(((((current_Highlighted_Pacing_Panel + getTotalPacingPanels) - initialHighlightedPanel) * (new_SecondsPerLap / getTotalPacingPanels * 1000)) + (new_SecondsPerLap*1000)) - temp_Millis);

	// we need to return the new_startTime to change the pacer's start time, but we need to do this after myMillis() > tempMillis;
	// return new_startTime;
}

// Returns the lowest available (empty) pacer or return the highest index; returns the index of the lowest pacer instance with getSecondsPerLap() == 0 unless all are greater than 0, in which case it will return the int associated with the instance of the highest pacer
int getLowestUnusedPacerIndex() {
	for (int i = 0; i < pacer[0].getNumberPacers(); i++) {
		if (pacer[i].getSecondsPerLap() == 0) {
			return i;
		}
	}

	// Return the index of the highest pacer if a lower empty one was not found
	return pacer[0].getNumberPacers()-1;
}

// Returns the index of the highest pacer instance with getSecondsPerLap() > 0 or returns -1 if no pacers have getSecondsPerLap > 0
int getHighestActivePacerIndex() {
	for (int i = pacer[0].getNumberPacers()-1; i > -1; i--) {
		if (pacer[i].getSecondsPerLap() > 0) {
			return i;
		}
	}
	return -1;
}