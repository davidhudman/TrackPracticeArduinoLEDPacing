<?php
// selectPin.php

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

$results = $db->query('SELECT * FROM Pin');

if ($backslashNNeeded == 1) { 
	while ($row = $results->fetchArray()) {
				$echoString = $echoString . $row['pacerIndex'] . " | " . $row['passcode'] . " | " . $row['active'] . " | " . $row['color'] . " | " . $row['lapTime'] . " | " . $row['name'] . "\n";
	}
}
else {
	while ($row = $results->fetchArray()) {
				$echoString = $echoString . $row['pacerIndex'] . " | " . $row['passcode'] . " | " . $row['active'] . " | " . $row['color'] . " | " . $row['lapTime'] . " | " . $row['name'] . "<br />";
	}
}
$db->exec($query);
$db->close();

echo $echoString;

?>