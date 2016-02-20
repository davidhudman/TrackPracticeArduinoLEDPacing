<?php
// createWorkout.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pacerIndex = -1;
$sets = 1;
$meters = -1;
$secondsPerLap = -1;
$secondsRest = -1;
$meters2 = 0;
$secondsPerLap2 = 0;
$secondsRest2 = 0;

// Pull all your values out of the GET request
if (empty($_REQUEST["pacerIndex"])) {
	$pacerIndex = $_REQUEST["pacerIndex"];
	$sets = $_REQUEST["sets"];
	$meters = $_REQUEST["meters"];
	$secondsPerLap = $_REQUEST["secondsPerLap"];
	$secondsRest = $_REQUEST["secondsRest"];
	$meters2 = $_REQUEST["meters2"];
	$secondsPerLap2 = $_REQUEST["secondsPerLap2"];
	$secondsRest2 = $_REQUEST["secondsRest2"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pacerIndex = $_REQUEST["pacerIndex"];
	$sets = $_REQUEST["sets"];
	$meters = $_REQUEST["meters"];
	$secondsPerLap = $_REQUEST["secondsPerLap"];
	$secondsRest = $_REQUEST["secondsRest"];
	$meters2 = $_REQUEST["meters2"];
	$secondsPerLap2 = $_REQUEST["secondsPerLap2"];
	$secondsRest2 = $_REQUEST["secondsRest2"];
}

$db->query('DELETE FROM Workout WHERE pacerIndex=' . $pacerIndex);

// for loop to add the same workout sequence for all the sets
for ($x = 0; $x < $sets; $x++) {
	// Query to add the rep portion
	$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . $secondsPerLap . ', ' . $meters . ')');
				
	// Query to add the rest portion
	$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '0' . ', ' . $secondsRest . ', ' . $meters . ')');
	
	if ($meters2 > 0 && $secondsPerLap2 > 0 && $secondsRest2 > 0) {
		// Query to add the rep portion
		$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . $secondsPerLap2 . ', ' . $meters2 . ')');
				
		// Query to add the rest portion
		$db->query('INSERT INTO Workout (pacerIndex, repeat, remaining, active, type, secondsPerLap, meters) VALUES (' . $pacerIndex . ', ' . '1' . ', ' . '1' . ', ' . '1' . ', ' . '0' . ', ' . $secondsRest2 . ', ' . $meters2 . ')');
	}
} 


$db->query('UPDATE Workout SET active=1 WHERE pacerIndex=' . $pacerIndex);					


$db->close();

echo "Query Executed Successfully";

?>