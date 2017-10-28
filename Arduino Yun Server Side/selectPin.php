<?php
// selectPin.php
// used to run a query to select everything in the Pin table - to allow us to see all of the pacer data

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

// run the query
$results = $db->query('SELECT * FROM Pin');

// parse the query and return it to the user and use the user's input to determine whether they need linebreaks in the form of "\n" or <br />
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