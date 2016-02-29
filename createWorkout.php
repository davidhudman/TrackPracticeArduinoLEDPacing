<?php
// createWorkout.php
// used to add the workout that the user creates in the app to the workout table in the database

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pacerIndex = -1;
$sets = 1;
$intervals = -1;

echo $_REQUEST;
// echo $_REQUEST["numMetersArray"];

// Pull all your values out of the GET request
if (empty($_REQUEST["pacerIndex"])) {
	$pacerIndex = $_REQUEST["pacerIndex"];
	$sets = $_REQUEST["sets"];
	$intervals = $_REQUEST["intervals"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pacerIndex = $_REQUEST["pacerIndex"];
	$sets = $_REQUEST["sets"];
	$intervals = $_REQUEST["intervals"];
}

$echoString = "";
// loop to get all array elements out of the request
for ($x = 0; $x < $intervals; $x++) {
	$echoString .= $_REQUEST["numMetersArray[intervals]"];
	$echoString .= $_REQUEST["secondsLapPaceArray[intervals]"];
	$echoString .= $_REQUEST["secondsRestArray[intervals]"];
}

/*
// Delete whatever workout that pacer already had in the database
$db->query('DELETE FROM Workout WHERE pacerIndex=' . $pacerIndex);

// Loop to add the same workout sequence for all the sets - maybe we could change this to pass an array of values in the GET request so we can add unlimited number of intervals in each set
for ($x = 0; $x < $sets; $x++) {
	// Query to add the rep portion
	$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . $secondsPerLap . ', ' . $meters . ')');
				
	// Query to add the rest portion
	$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '0' . ', ' . $secondsRest . ', ' . $meters . ')');
	
	// If there is a second interval to be done in each set, this is where it is set up
	if ($meters2 > 0 && $secondsPerLap2 > 0 && $secondsRest2 > 0) {
		// Query to add the rep portion
		$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . $secondsPerLap2 . ', ' . $meters2 . ')');
				
		// Query to add the rest portion
		$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '0' . ', ' . $secondsRest2 . ', ' . $meters2 . ')');
	}
} 

// set all the workout entries in the workout table for this pacer to "active=1" to indicate that they need to be run
$db->query('UPDATE Workout SET active=1 WHERE pacerIndex=' . $pacerIndex);					
*/

$db->close();

/*
echo "Your workout was successfully added.";
*/

?>