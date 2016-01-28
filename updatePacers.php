<?php
// updatePacers.php

/* Functional code providing some limited validation */
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pIndex = "-1";
$secondsPerLap = 0;

$pIndex = "0";
$secondsPerLap = 2.0;

/*if (empty($_REQUEST["pacer"])) {
	$pIndex = $_REQUEST["pacer"];
	$secondsPerLap = $_REQUEST["secondsPerLap"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pIndex = $_REQUEST["pacer"];
	$secondsPerLap = $_REQUEST["secondsPerLap"];
}*/

$results = $db->query('UPDATE Pin set active = 1, lapTime =' . $secondsPerLap . ' where pacerIndex=' . $pIndex);

$db->exec($query);
$db->close();
// echo $pIndex != -1 ? $pIndex : "incorrect";
echo "\nStatement Excuted Successfully". " pacer = " . $pIndex . " lapTime = " . $secondsPerLap;

?>