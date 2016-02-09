<?php
// updatePacers.php


$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$ipAddress = "http://172.20.10.7";			// holds the ipAddress that needs to be used on the network that Yun is connected to
// $ipAddress = "http://192.168.1.153";
// $ipAddress = "http://192.168.240.1";

$rqPacerIndex = "-1";						// holds the rqPacerIndex value from the request
$rqSscondsPerLap = 0;					// holds the rqSscondsPerLap value from the request
$rqNeedNewPacer = -1;					// holds the rqNeedNewPacer value from the request
$rqNeedYunSyncWithDB = -1;		// holds the rqNeedYunSyncWithDB value from the request

//$rqPacerIndex = "0";
//$rqSscondsPerLap = 2.0;

// Pull all your values out of the GET request
if (empty($_REQUEST["pacer"])) {
	$rqPacerIndex = $_REQUEST["pacer"];
	$rqSscondsPerLap = $_REQUEST["secondsPerLap"];
	$rqNeedNewPacer = $_REQUEST["needNewPacer"];
	$rqNeedYunSyncWithDB = $_REQUEST["needYunSyncWithDB"];
}
else {
	// do nothing - for some reason it may get recognized as "empty" even though there are clearly parameters
	$rqPacerIndex = $_REQUEST["pacer"];
	$rqSscondsPerLap = $_REQUEST["secondsPerLap"];
	$rqNeedNewPacer = $_REQUEST["needNewPacer"];
	$rqNeedYunSyncWithDB = $_REQUEST["needYunSyncWithDB"];
}

// If the flag for syncing the Yun with what is already in the database is true
if ($rqNeedYunSyncWithDB == 1) {
	// Retrieve the Pacer data that was already in the Database
	// $results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');
	$results = $db->query('SELECT * FROM Pin WHERE active = 1 ORDER BY pacerIndex ASC');

	while ($row = $results->fetchArray()) {
			$dbPacerIndex = $row['pacerIndex'];
			$dbSecondsPerLap = $row['lapTime'];
			// send requests to the Yun to update the Pacers with their data
			// *** add this: if (dbPacerData != yunPacerData)
			file_get_contents($ipAddress . "/arduino/" . "7" . "/" . $dbPacerIndex . "/" . $dbSecondsPerLap);
			echo "Command made it through to Arduino. pacerIndex = " . $dbPacerIndex . " dbSecondsPerLap=" . $dbSecondsPerLap;
			// sleep(5);	// sleep x seconds
	}
	$db->exec($query);
	$db->close();
}

// If the flag for requesting a new Pacer is true
if ($rqNeedNewPacer == 1) {
	$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');

	while ($row = $results->fetchArray()) {
			$pacerIndex = $row['pacerIndex'];
			echo $pacerIndex;
	}

	$db->exec($query);

	$results = $db->query('UPDATE Pin SET active = 1, lapTime =' . $rqSscondsPerLap . ' WHERE pacerIndex=' . $rqPacerIndex);

	$db->exec($query);
	$db->close();
}

/*$results = $db->query('UPDATE Pin set active = 1, lapTime =' . $rqSscondsPerLap . ' where pacerIndex=' . $rqPacerIndex);

$db->exec($query);
$db->close();*/
// echo $rqPacerIndex != -1 ? $rqPacerIndex : "incorrect";
echo "\nStatement Excuted Successfully". " rqPacerIndex = " . $rqPacerIndex . " rqSecondsPerLap = " . $rqSscondsPerLap;

?>