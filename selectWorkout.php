<?php
// selectWorkout.php
// used to select everything from the "workout" table in the database

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$echoString = "";
$backslashNNeeded = -1;

// Pull all your values out of the GET request
if (empty($_REQUEST["backslashNNeeded"])) {
	$backslashNNeeded = $_REQUEST["backslashNNeeded"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$backslashNNeeded = $_REQUEST["backslashNNeeded"];
}

$results = $db->query('SELECT * FROM Workout');

if ($backslashNNeeded == 1) { 
	while ($row = $results->fetchArray()) {
				$echoString = $echoString . $row['workoutIndex'] . " | " . $row['pacerIndex'] . " | " . $row['repeat'] . " | " . $row['remaining'] . " | " . $row['active'] . " | " . $row['type'] . " | " . $row['secondsPerLap'] . " | " . $row['meters'] . "\n";
	}
}
else {
	while ($row = $results->fetchArray()) {
				$echoString = $echoString . $row['workoutIndex'] . " | " . $row['pacerIndex'] . " | " . $row['repeat'] . " | " . $row['remaining'] . " | " . $row['active'] . " | " . $row['type'] . " | " . $row['secondsPerLap'] . " | " . $row['meters'] . "<br />";
	}
}
$db->exec($query);
$db->close();

echo $echoString;

?>