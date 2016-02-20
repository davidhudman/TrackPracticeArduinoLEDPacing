<?php
// runWorkout.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$ipAddress = "http://172.20.10.7";			// holds the ipAddress that needs to be used on the network that Yun is connected to: IP address for Yun on my iPhone hotspot network
// $ipAddress = "http://192.168.1.153";	// IP address for Yun on my home network
// $ipAddress = "http://192.168.240.1";	// IP address for Yun on its own network

$i = 0;		// used to increment array in the while loop
$arrayLength = 0;

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

// Pull all your values out of the GET request
if (empty($_REQUEST["pacerIndex"])) {
	$pacerIndex = $_REQUEST["pacerIndex"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pacerIndex = $_REQUEST["pacerIndex"];
}

if ($pacerIndex != -1) {
	$results = $db->query('SELECT * FROM workout WHERE pacerIndex=' . $pacerIndex . ' AND active=1 ORDER BY pacerIndex ASC LIMIT 1');
}
else {
	$results = $db->query('SELECT * FROM workout WHERE pacerIndex=0 AND active=1 ORDER BY pacerIndex ASC LIMIT 1');		// This will only work with the 0 index pacer.
}

// echo "<br />SELECT query Executed Successfully";

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

// first make sure that we got something
if ($workoutIndex != -1) {
	if ($typer == 0) {		// Rest - we're going to have to subtract 25-30 seconds from their rest because that's how long the processing takes
		$delayTime = $secondsPerLap;
		if ($delayTime <= 22) {
			$delayTime = 1;
		}
		else {
			$delayTime = $delayTime - 22;
		}
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/-" . $delayTime . "/" . $meters);		// send the delayTime with a negative number to indicate a rest period
	}
	else if ($typer == 1) {		// rep
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/" . $secondsPerLap . "/" . $meters);		// set the time on that pacer
	}

	// Change that row in the database to inactive
	$db->query('UPDATE Workout SET active=0 WHERE workoutIndex=' . $workoutIndex);
	// $db->exec($query);
	echo "<br />UPDATE query Executed Successfully";

}
else {		// no workout available, so let's end the pacer's workout on the Yun
	file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/0" . "/" . $meters);		// send the command to end the pacer's workout
	echo "<br />Pacer cleared successfully";
}

$db->close();

?>