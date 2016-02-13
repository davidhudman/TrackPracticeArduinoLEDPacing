<?php
// selectPin.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$echoString = "";

$results = $db->query('SELECT * FROM Pin');

while ($row = $results->fetchArray()) {
			$echoString = $echoString . $row['pacerIndex'] . " | " . $row['passcode'] . " | " . $row['active'] . " | " . $row['color'] . " | " . $row['lapTime'] . " | " . $row['name'] . "<br />";
}
$db->exec($query);
$db->close();

echo $echoString;

?>