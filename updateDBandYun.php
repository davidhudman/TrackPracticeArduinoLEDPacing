<?php
// updateDBandYun.php

// Example link command: http://172.20.10.7/sd/TrackPractice/updateDBandYun.php?command=7&pacerIndex=2&value=4&pin=222

/* Functional code providing some limited validation */
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$ipAddress = "http://172.20.10.7";			// holds the ipAddress that needs to be used on the network that Yun is connected to: IP address for Yun on my iPhone hotspot network
// $ipAddress = "http://192.168.1.153";	// IP address for Yun on my home network
// $ipAddress = "http://192.168.240.1";	// IP address for Yun on its own network


$pacerIndex = -1;
$value = -1;
$command = -1;
$pin = -1;
$queryResultPin = -1;

$queryPacerIndex = -1;
$backwards = -1;

//$pIndex = "0";
//$secondsPerLap = 2.0;

// Pull all your values out of the GET request
if (empty($_REQUEST["command"])) {
	$pacerIndex = $_REQUEST["pacerIndex"];
	$value = $_REQUEST["value"];
	$command = $_REQUEST["command"];
	$pin = $_REQUEST["pin"];
}
else {
	// do nothing - for some reason it always gets recognized as "empty" even though there are clearly parameters
	$pacerIndex = $_REQUEST["pacerIndex"];
	$value = $_REQUEST["value"];
	$command = $_REQUEST["command"];
	$pin = $_REQUEST["pin"];
}

// If the pacerIndex is 99, we are in "Coach Mode" so it's not necessary to do a PIN check.
if ($pacerIndex == 99) {
	// update the database, if necessary
		switch ($command) {
			case -1:	// nothing entered
				break;
			case 0:	// clear
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
				$db->query('UPDATE Pin SET active=0, lapTime=0');
				break;
			case 4:	// backwards
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
				/*$backwards = $db->query('SELECT backwards FROM Pin WHERE pacerIndex=' . $pacerIndex);
				
				// $db->exec($query);
				$db->close();
				if ($backwards == 0) {
					$db->query('UPDATE Pin SET backwards = 1 WHERE pacerIndex=' . $pacerIndex);
					// $db->exec($query);
					$db->close();
				}
				if ($backwards == 1) {
					$db->query('UPDATE Pin SET backwards = 0 WHERE pacerIndex=' . $pacerIndex);
					// $db->exec($query);
					$db->close();
				}*/
				break;
			case 5:	// color
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
				$db->query('UPDATE Pin SET color=' . $value);
				// $db->exec($query);
			case 6:	// lights
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
				break;
			case 7:	// time
				// find an available pacer
				$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');
				while ($row = $results->fetchArray()) {
						$queryPacerIndex = $row['pacerIndex'];
				}
				$db->exec($query);
				// if there's a pacer available:
				if ($queryPacerIndex != -1) {
					// do the arduino requests
					file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
					echo "Command made it through to Arduino.";
					
					// update the database
					$results = $db->query('UPDATE Pin SET active=1, lapTime=' . $value . ' WHERE pacerIndex=' . $queryPacerIndex);
					$db->exec($query);
				}
				else {
					echo "No Pacer Available";
				}
				break;
			case 8:	// change speed
			case 11:	// color array crappy
			default:	// clear, reset, reset delay, etc.
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
				break;
		}
		$db->close();
}
else {
	if ($command > -1)		// if It's a valid request, we can proceed
	{
		// query to get $queryResultPin from DB 
		$results = $db->query('SELECT * FROM Pin WHERE pacerIndex=' . $pacerIndex);
		while ($row = $results->fetchArray()) {
			$queryResultPin = $row['passcode'];
		}
		$db->exec($query);

		// make sure the $pacerIndex matches $pin
		if ($queryResultPin == $pin) {
			
			if (command != -1) {
				// then do the arduino requests
				file_get_contents($ipAddress . "/arduino/" . $command . "/" . $pacerIndex . "/" . $value);
				echo "Command made it through to Arduino.";
			}
			else {
				echo "the command was never received.";
			}
			// update the database, if necessary
			switch ($command) {
				case 0:	// clear
					$db->query('UPDATE Pin SET active=0, lapTime=0 WHERE pacerIndex=' . $pacerIndex);
					break;
				case 4:	// backwards
					/*$backwards = $db->query('SELECT backwards FROM Pin WHERE pacerIndex=' . $pacerIndex);
					
					// $db->exec($query);
					$db->close();
					if ($backwards == 0) {
						$db->query('UPDATE Pin SET backwards = 1 WHERE pacerIndex=' . $pacerIndex);
						// $db->exec($query);
						$db->close();
					}
					if ($backwards == 1) {
						$db->query('UPDATE Pin SET backwards = 0 WHERE pacerIndex=' . $pacerIndex);
						// $db->exec($query);
						$db->close();
					}*/
					break;
				case 5:	// color
					$db->query('UPDATE Pin SET color=' . $value . ' WHERE pacerIndex=' . $pacerIndex);
					// $db->exec($query);
				case 7:	// time
				case 8:	// change speed
					$results = $db->query('UPDATE Pin SET active=1, lapTime=' . $value . ' WHERE pacerIndex=' . $pacerIndex);
					while ($row = $results->fetchArray()) {
						$queryResultPin = $row['passcode'];
					}
					$db->exec($query);
					break;
				case 6:	// lights
					// **** add something here
					break;
				case 11:	// color array crappy
					// **** add something here
					break;
				default:	// clear, reset, reset delay, etc.
					break;
			}
			$db->close();
		}
		else {
			$db->close();
			echo "the PIN you're using does not match the PIN for the pacer in the Database. Query result PIN is " . $queryResultPin . " but your PIN was " . $pin;
		}
	}
	else {
		echo "invalid command or no parameters received";
	}
}
/*
$results = $db->query('UPDATE Pin set active = 1, lapTime =' . $secondsPerLap . ' where pacerIndex=' . $pIndex);

$db->exec($query);
$db->close();
// echo $pIndex != -1 ? $pIndex : "incorrect";
echo "\nStatement Excuted Successfully". " pacer = " . $pIndex . " lapTime = " . $secondsPerLap;
*/

?>