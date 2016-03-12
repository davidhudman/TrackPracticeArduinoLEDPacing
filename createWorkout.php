<?php
// createWorkout.php
// used to add the workout that the user creates in the app to the workout table in the database

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pacerIndex = -1;
$sets = 1;
$intervals = -1;
$metersArray = [];
$paceArray = [];
$restArray = [];
$queryArray = [];
$queryArrayCounter = 0;
$concatenatedString = "";

// echo $_REQUEST["pace9"];
// echo $_REQUEST["numMetersArray"];


// Pull all your values out of the GET request
$pacerIndex = $_REQUEST["pacerIndex"];
$sets = $_REQUEST["sets"];
$intervals = $_REQUEST["intervals"];


// loop to get all array elements out of the request
for ($x = 0; $x < $intervals; $x++) {
	$tempString = "m" . $x;
	$metersArray[$x] = $_REQUEST[$tempString];
	$tempString = "p" . $x;
	$paceArray[$x] = $_REQUEST[$tempString];
	$tempString = "r" . $x;
	$restArray[$x] = $_REQUEST[$tempString];
}

// Delete whatever workout that pacer already had in the database
$db->query('DELETE FROM Workout WHERE pacerIndex=' . $pacerIndex);

// Loop to add the same workout sequence for all the sets - maybe we could change this to pass an array of values in the GET request so we can add unlimited number of intervals in each set
for ($x = 0; $x < $sets; $x++) {
	// loop through the intervals in each set
	for ($y = 0; $y < $intervals; $y++) {
		// Query to add the rep portion
		$queryArray[$queryArrayCounter] = 'INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . $paceArray[$y] . ', ' . $metersArray[$y] . ');';
		$queryArrayCounter = $queryArrayCounter + 1;			
		
		// Query to add the rest portion
		$queryArray[$queryArrayCounter] = 'INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '0' . ', ' . $restArray[$y] . ', ' . $metersArray[$y] . ');';
		$queryArrayCounter = $queryArrayCounter + 1;
	}
} 

for ($x = 0; $x <= count($queryArray); $x++) {
	$concatenatedString .= $queryArray[$x];
}
$db->query($concatenatedString);

// set all the workout entries in the workout table for this pacer to "active=1" to indicate that they need to be run
$db->query('UPDATE Workout SET active=1 WHERE pacerIndex=' . $pacerIndex);

$db->close();

echo "Your workout was successfully added." . " sets=" . $sets . ", intervals=" . $intervals . ", last rest entered=" . $restArray[$intervals-1];

?>