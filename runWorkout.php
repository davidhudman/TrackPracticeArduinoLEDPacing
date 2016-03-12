<?php
// runWorkout.php
// used to find the workouts in the workout table of the database that need to be run - where "active=0"

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$ipAddress = "http://172.20.10.7";			// holds the ipAddress that needs to be used on the network that Yun is connected to: IP address for Yun on my iPhone hotspot network
// $ipAddress = "http://192.168.1.153";	// IP address for Yun on my home network
// $ipAddress = "http://192.168.240.1";	// IP address for Yun on its own network

$i = 0;		// used to increment array in the while loop
$arrayLength = 0;
$arduinoGetRequestDelay = 22;	// the time that it takes for the Yun to make a GET request.

$workoutIndex = -1;
$pacerIndex = -1;
$repeat = -1;
$remaining = -1;
$active = -1;
$typer = -1;
$secondsPerLap = -1;
$meters = -1;
$next = -1;
$previous = -1;
$webAppGeneratedRequest = -1;

// Pull all your values out of the GET request
if (empty($_REQUEST["pacerIndex"])) {
	$pacerIndex = $_REQUEST["pacerIndex"];
	$webAppGeneratedRequest = $_REQUEST["webAppGeneratedRequest"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pacerIndex = $_REQUEST["pacerIndex"];
	$webAppGeneratedRequest = $_REQUEST["webAppGeneratedRequest"];
}

if ($pacerIndex != -1) {
	$results = $db->query('SELECT * FROM workout WHERE pacerIndex=' . $pacerIndex . ' AND active=1 ORDER BY pacerIndex ASC LIMIT 1');
}
else {
	$results = $db->query('SELECT * FROM workout WHERE pacerIndex=0 AND active=1 ORDER BY pacerIndex ASC LIMIT 1');		// OLD CODE - TEST BEFORE REMOVING THIS ENTIRE ELSE STATEMENT- This will only work with the 0 index pacer.
}

// Read the results of the database query - it should just be one row
while ($row = $results->fetchArray()) {
			
			$workoutIndex = $row['workoutIndex'];
			$pacerIndex = $row['pacerIndex'];
			$repeat = $row['repeat'];
			$remaining = $row['remaining'];
			$active = $row['active'];
			$typer = $row['type'];
			$secondsPerLap = $row['secondsPerLap'];
			$meters = $row['meters'];
			// $next = $row['next'];
			// $previous = $row['previous'];
			
			// $lapTimes[$i] = $secondsPerLap;
			// $metersToRun[$i] = $meters;
			// $type[$i] = $typer;
			// $i = $i + 1;
}

// First, make sure that we got something from the query
if ($workoutIndex > -1) {
	// The workout type is "Rest" - we're going to have to subtract 20-30 seconds from their rest because that's how long the processing takes
	if ($typer == 0) {
		$delayTime = $secondsPerLap;
		if ($delayTime <= $arduinoGetRequestDelay) {
			$delayTime = 1;
		}
		else {
			$delayTime = $delayTime - $arduinoGetRequestDelay;
		}
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/-" . $delayTime . "/" . $meters);		// send the delayTime with a negative number to indicate a rest period
	}
	// The workout type is "Rep"
	else if ($typer == 1) {
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/" . $secondsPerLap . "/" . $meters);		// set the time on that pacer
	}

	// Change that row in the database to inactive (because it's already running and we don't want it to run again)
	$db->query('UPDATE Workout SET active=0 WHERE workoutIndex=' . $workoutIndex);
	echo "<br />Your interval that you created will begin shortly.";

}
// No workout available, so let's end the pacer's workout on the Yun
else {
	// if this request did not come from the app
	if ($webAppGeneratedRequest != 1) {
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/0" . "/" . $meters);		// send the command to end the pacer's workout
		echo "<br />Pacer cleared successfully";
	}
	else {	// if the request came from the app
		echo "No workout available.";
	}
}

$db->close();

?>