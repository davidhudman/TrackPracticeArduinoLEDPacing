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
							/*if (isStartTimeWithinXSecondsOnly(0)) {
								return (getInitialHighlightedPanel())%getTotalPacingPanels();	// green
								// setVisible(true);
							}*/
							// setVisible(false);
							return (getInitialHighlightedPanel())%getTotalPacingPanels();	// black or "off", the reason for leaving this black is so that no other pacer will come up behind it and make runners think that they should be starting
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
							/*if (isStartTimeWithinXSecondsOnly(0)) {
								return (getInitialHighlightedPanel())%getTotalPacingPanels();	// green
								// setVisible(true);
							}*/
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
		long temp_Millis = millis();
		// if it's backwards (end to beginning)
		if (getStartTime() > (temp_Millis-1000)) { // make this temp_Millis - 1000 if you want to show green when it's go time
			if (isStartTimeWithinXSecondsOnly(trafficLightCountDownRedSeconds)) {
				if (isStartTimeWithinXSecondsOnly(trafficLightCountDownYellowSeconds)) {
					if (isStartTimeWithinXSecondsOnly(trafficLightCountDownDarkSeconds)) {
						/*if (isStartTimeWithinXSecondsOnly(0)) {
							return 1;	// green
						}*/
						return 2; // yellow (now with change) - would've been: black or "off" - it doesn't really matter what the color is because we're turning visibility off
					}
					return 2; // yellow
				}
				return 3; // red
			}
			return colorInt;
		}
		else
		{
			return colorInt;
		}
	}
	bool getIsVisible() {
		return isVisible;
	}
	
	// returns the new start time for a pacer intersecting the given pacer at the current light its on given the desired seconds per lap; change the Speed of the Pacer To (command "spt")
	void setChangedPacerNewStartTime(double new_SecondsPerLap) {
		long new_startTime,
			divisionResult,
			currentTime = millis();

		// use currentHighlightedPacingPanel and solve for getRunningTime (what myMillis() will be when it hit that panel that you're on)
		new_startTime = (long)(((((getCurrentHighlightedPacingPanel() + getTotalPacingPanels()) - getInitialHighlightedPanel())*(getSecondsPerLap() / getTotalPacingPanels() * 1000))+(getSecondsPerLap()*1000)) + getStartTime());
		// temp_Millis probably needs to be verified bigger than myMillis()

		divisionResult = currentTime / (getSecondsPerLap()*1000);

		new_startTime = new_startTime + (divisionResult*(getSecondsPerLap()*1000)) + (getSecondsPerLap()*1000);
	
		while (currentTime < new_startTime) {
			new_startTime -= (getSecondsPerLap()*1000);
		}

		new_startTime -= (getCurrentHighlightedPacingPanel() - getInitialHighlightedPanel()) * ((new_SecondsPerLap*1000) / getTotalPacingPanels());	// when this line is added, also make the change below from setting the pacer's initialHighlightedPanel with currentHighlightedPanel to initialHighlightedPanel

		setStartTime(new_startTime);
		setSecondsPerLap(new_SecondsPerLap);
		setInitialHighlightedPanel(getInitialHighlightedPanel());

		// then use the new getRunningTime (actually tempMillis and the new_SecondsPerLap to solve for startTime
		// new_startTime = -(long)(((((current_Highlighted_Pacing_Panel + getTotalPacingPanels) - initialHighlightedPanel) * (new_SecondsPerLap / getTotalPacingPanels * 1000)) + (new_SecondsPerLap*1000)) - temp_Millis);
		// we need to return the new_startTime to change the pacer's start time, but we need to do this after myMillis() > tempMillis;
		// return new_startTime;
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
#define NUM_LEDS 100

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
const int PACER_ARRAY_SIZE = 10,
			COLOR_ARRAY_SIZE = 8;
Pacer pacer[PACER_ARRAY_SIZE] = {Pacer(2,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS), 
	Pacer(0,0,0,0,1,NUM_LEDS), Pacer(0,0,0,0,1,NUM_LEDS) };
CRGB colorArray[COLOR_ARRAY_SIZE] = {CRGB::White, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Orange, CRGB::Purple, CRGB::Pink};	// white, green, yellow, red, blue, lime green, aqua, grey
// new code: CRGB colorArray[COLOR_ARRAY_SIZE] = {CRGB::Black, CRGB::White, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Orange, CRGB::Purple, CRGB::Pink};
// alternate line for other WS2811: CRGB colorArray[COLOR_ARRAY_SIZE] = {CRGB::Black, CRGB::White, CRGB::Green, CRGB::Yellow, CRGB::Red, CRGB::Blue, CRGB::Lime, CRGB::Aqua, CRGB::Grey};

//***********************************************
// Declarations: Related to strings
//***********************************************
String serialStringInput,			// Holds the raw, unformatted serial input from user.
		printThis = " ", 
		stringHolder = " ";

//***********************************************
// Declarations: All variables predefined
//***********************************************
long resetDelayDefaultDelayTimeMillis = 10000,		// the default delay time when resetting pacers on a delay
		tempMillis;		// holds the milliseconds time that a pacer's speed will change
int serial1AvailableIterator = 0, serial1FeedbackIterator = 0, serialFeedbackIterator = 0, trafficLightIterator = 0,
	serialCountTo = 50, trafficLightCountTo = 100, partySerialCountTo = 5, 
	tempLowestDelayedPacerIndex = -1, totalPacingPanels = 26;

bool isChangePacerSpeedNeeded = false, // trigger to determine whether we need to figure out which pacer is going to change its speed
		partyMode = false;				// This mode (now partyMode) String (now bool) has two possible values: "track" and "party" (now true and false). Each value will result in different function calls

//***********************************************
// Declarations: All variables not predefined
//***********************************************

// Listen to the default port 5555, the Yun webserver
// will forward there all the HTTP requests you send
YunServer server;

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.      
       FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
	  // FastLED.addLeds<firstParameter, DATA_PIN, RGB>(leds, NUM_LEDS);	// firstParameter can be: TM1803, TM1804, TM1809, WS2811, WS2812, APA104, UCS1903, UCS1903B, GW6205, GW6205_400
	  // FastLED.addLeds<NEOPIXEL,   DATA_PIN>(leds, NUM_LEDS);    
      // FastLED.addLeds<firstParameter,  RGB>(leds, NUM_LEDS);				// firstParameter can be: WS2801, SM16716, LPD8806, P9813, APA102, DOTSTAR
	  // FastLED.addLeds<firstParameter, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS); // firstParameter can be: WS2801, SM16716, LPD8806, P9813, APA102, DOTSTAR
	  // Documentation on constructors can be found in FastLED examples

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
	YunClient client = server.accept();		// Get clients coming from server

	// There is a new client?
	if (client) {
		process(client);	// Process request
		client.stop();		// Close connection and free resources.
	}
	// getSerialFeedback(); // This overwhelms the program if too many pacers are added
}

// Use an integer parameter to determine what RGB color value it is associated with
CRGB getColorFromInt(int i) {
	return colorArray[i%COLOR_ARRAY_SIZE];
}

// Sets the number of pacing pixels for each pacer to the number sent in the parameter
void assignGetTotalPacingPanels(int total_Pacing_Panels) {
	for (int i=0; i < pacer[0].getNumberPacers(); i++) {
		pacer[i].setTotalPacingPanels(total_Pacing_Panels);
	}
}

// Sets each pacer's color according to the its corresponding element in the colorArray
void assignPacerColors() {
	for (int i=0; i < pacer[0].getNumberPacers(); i++) {
		pacer[i].setColorInt(i);
		// pacer[i].setColorInt((i%8)+1);
	}
}

// Process the cURL command
void process(YunClient client) {
	int command = client.readStringUntil('/').toInt(),		// read the command
		pacerIndex = client.readStringUntil('/').toInt();	// read pin number
	double thirdCommand = client.parseFloat();

	// If the next character is a '/' it means we have an URL
	// with a value like: "/1/99/0"

	switch(command) {
		case 0:	// clear
			if (pacerIndex == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setSecondsPerLap(0);
				}
				return;
			}
			if (pacerIndex > -1){
				pacer[pacerIndex].setSecondsPerLap(0);
			}
			break;
		case 1:	// reset
			tempMillis = millis();
			if (pacerIndex == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTime(tempMillis);
				}
				return;
			}
			if (pacerIndex > -1){
				pacer[pacerIndex].setStartTime(tempMillis);
			}
			break;
		case 2:	// resetdelay
			tempMillis = millis();
			if (pacerIndex == 99){
				for (int i=0; i <= getHighestActivePacerIndex(); i++){
					pacer[i].setStartTime(tempMillis+resetDelayDefaultDelayTimeMillis);
				}
				return;
			}
			if (pacerIndex > -1){
				pacer[pacerIndex].setStartTime(tempMillis+resetDelayDefaultDelayTimeMillis);
			}
			break;
		case 3:	// visible
			if (pacerIndex == 99){
				for (int i=0; i <= pacer[0].getNumberPacers(); i++){
					pacer[i].setIsVisible(!pacer[i].getIsVisible());
				}
				return;
			}
			if (pacerIndex > -1){
				pacer[pacerIndex].setIsVisible(!pacer[pacerIndex].getIsVisible());
			}
			break;
		case 4:	// backwards
			if (pacerIndex == 99){
				for (int i=0; i <= pacer[0].getNumberPacers(); i++){
					pacer[i].setIsBackwards(!pacer[i].getIsBackwards());
				}
				return;
			}
			if (pacerIndex > -1){
				pacer[pacerIndex].setIsBackwards(!pacer[pacerIndex].getIsBackwards());
			}
			break;
		case 5:	// color
			// if "all pacers" is selected
			if (pacerIndex == 99) {
				// if "normal colors" is selected
				if ((int)thirdCommand == 99) {
					assignPacerColors();
				}
				else {
					for (int i=0; i < pacer[0].getNumberPacers(); i++) {
						pacer[i].setColorInt((int)thirdCommand);
					}
				}
			}
			// if a specific pacer is selected
			else {
				if ((int)thirdCommand == 99){
					assignPacerColors();
				}
				else {
					pacer[pacerIndex].setColorInt((int)thirdCommand);
				}
			}
			break;
		case 6:	// lights
			/*if (pacerIndex == 99) {
				for (int i=0; i < pacer[0].getNumberPacers(); i++) {
					assignGetTotalPacingPanels(thirdCommand);
				}
			}
			else {
				assignGetTotalPacingPanels(thirdCommand);
			}*/
			for (int i=0; i < pacer[0].getNumberPacers(); i++) {
					assignGetTotalPacingPanels((int)thirdCommand);
			}
			break;
		case 7:	// time
			if (pacerIndex == 99) {
				pacer[getLowestUnusedPacerIndex()].setStartTime(millis()+7000);
				pacer[getLowestUnusedPacerIndex()].setSecondsPerLap (thirdCommand);
			}
			else {
				pacer[pacerIndex].setStartTime(millis()+7000);
				pacer[pacerIndex].setSecondsPerLap (thirdCommand);
			}

			/*// Send feedback to client
			client.print(F("Pin D"));
			client.print(pacerIndex);
			client.print(F(" set to "));
			client.println(thirdCommand);

			// Update datastore key with the current pin value
			String key = "D";
			key += pacerIndex;
			Bridge.put(key, String(thirdCommand));*/
			break;
		case 8:	// change speed by
			if (pacerIndex == 99) {
				for (int i=0; i < pacer[0].getNumberPacers(); i++) {
					if (pacer[i].getSecondsPerLap() > 0) {
						pacer[i].setChangedPacerNewStartTime(pacer[i].getSecondsPerLap()+thirdCommand);
					}
				}
			}
			else {
				if (pacer[pacerIndex].getSecondsPerLap() > 0) {
					pacer[pacerIndex].setChangedPacerNewStartTime(pacer[pacerIndex].getSecondsPerLap()+thirdCommand);
				}
			}
			break;
		default:
			break;
	}
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
		}
	}
	FastLED.show();              // refresh strip display
}

// Returns the lowest available (empty) pacer or return the highest index; returns the index of the lowest pacer instance with getSecondsPerLap() == 0 unless all are greater than 0, in which case it will return the int associated with the instance of the highest pacer
int getLowestUnusedPacerIndex() {
	for (int i = 0; i < pacer[0].getNumberPacers(); i++) {
		if (pacer[i].getSecondsPerLap() == 0) {
			return i;
		}
	}
	return pacer[0].getNumberPacers()-1;	// Return the index of the highest pacer if a lower empty one was not found
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