<?php
// runWorkout.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$ipAddress = "http://172.20.10.7";			// holds the ipAddress that needs to be used on the network that Yun is connected to: IP address for Yun on my iPhone hotspot network
// $ipAddress = "http://192.168.1.153";	// IP address for Yun on my home network
// $ipAddress = "http://192.168.240.1";	// IP address for Yun on its own network

/*
// Creating database
$db->query('CREATE TABLE Workout(
						workoutIndex INT PRIMARY KEY,
						pacerIndex INT,
						repeat INT,
						remaining INT,
						active INT,
						type TEXT,		// This should actually be INT, not TEXT
						secondsPerLap REAL,
						meters INT,
						next INT,
						previous INT
	)');
	
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (0, 0, 1, 1, 0, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (1, 0, 1, 1, 0, 0, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (2, 0, 1, 1, 0, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (3, 0, 1, 1, 0, 0, 10, 400)');						
*/

/*
// Resetting data in database to original
$db->query('UPDATE Pin SET passcode="123", active=0, color=1, lapTime=0 WHERE pacerIndex=0');
$db->query('UPDATE Pin SET passcode="111", active=0, color=2, lapTime=0 WHERE pacerIndex=1');
$db->query('UPDATE Pin SET passcode="222", active=0, color=3, lapTime=0 WHERE pacerIndex=2');
$db->query('UPDATE Pin SET passcode="333", active=0, color=4, lapTime=0 WHERE pacerIndex=3');
$db->query('UPDATE Pin SET passcode="444", active=0, color=5, lapTime=0 WHERE pacerIndex=4');
$db->query('UPDATE Pin SET passcode="555", active=0, color=6, lapTime=0 WHERE pacerIndex=5');
$db->query('UPDATE Pin SET passcode="666", active=0, color=7, lapTime=0 WHERE pacerIndex=6');
$db->query('UPDATE Pin SET passcode="777", active=0, color=1, lapTime=0 WHERE pacerIndex=7');
$db->query('UPDATE Pin SET passcode="888", active=0, color=2, lapTime=0 WHERE pacerIndex=8');
$db->query('UPDATE Pin SET passcode="999", active=0, color=3, lapTime=0 WHERE pacerIndex=9');
*/

$i = 0;		// used to increment array in the while loop
$arrayLength = 0;

$workoutIndex = -1;
$pacerIndex = -1;
$repeat = -1;
$remaining = -1;
$active = -1;
$typer = -1;
$secondsPerLap = -1;
$meters = -1;
$next = -1;
$previous = -1;

$results = $db->query('SELECT * FROM workout WHERE pacerIndex=0 AND active=1 ORDER BY pacerIndex ASC LIMIT 1');		// This will only work with the 0 index pacer.
// echo "<br />SELECT query Executed Successfully";

while ($row = $results->fetchArray()) {
			
			$workoutIndex = $row['workoutIndex'];
			$pacerIndex = $row['pacerIndex'];
			$repeat = $row['repeat'];
			$remaining = $row['remaining'];
			$active = $row['active'];
			$typer = $row['type'];
			$secondsPerLap = $row['secondsPerLap'];
			$meters = $row['meters'];
			// $next = $row['next'];
			// $previous = $row['previous'];
			
			// $lapTimes[$i] = $secondsPerLap;
			// $metersToRun[$i] = $meters;
			// $type[$i] = $typer;
			// $i = $i + 1;
}

// first make sure that we got something
if ($workoutIndex != -1) {
	if ($typer == 0) {		// Rest
		$delayTime = $secondsPerLap;
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/-" . $delayTime);		// send the delayTime with a negative number to indicate a rest period
		// echo "Status: 204 No Content\r\n\r\n";
		// echo "<br />Rest command made it through to Arduino. type=" . $typer . " delay=" . $delayTime;
	}
	else if ($typer == 1) {		// rep
		$delayTime = $secondsPerLap * ($meters / 400);
		file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/" . $delayTime);		// set the time on that pacer
		
		// echo "Status: 204 No Content\r\n\r\n";
		// echo "<br />Rest command made it through to Arduino. type=" . $typer . " delay=" . $delayTime;
	}

	// Change that row in the database to inactive
	$db->query('UPDATE Workout SET active=0 WHERE workoutIndex=' . $workoutIndex);
	// echo "<br />UPDATE query Executed Successfully";

}
else {		// no workout available, so let's end the pacer's workout on the Yun
	file_get_contents($ipAddress . "/arduino/13/" . $pacerIndex . "/0");		// send the command to end the pacer's workout
}



/*
$arrayLength = count($lapTimes);

for($x = 0; $x < $arrayLength; $x++) {
    $delayTime = 0;
	
	set_time_limit(900);
	if ($type[$x] == 0) {		// Rest
		$delayTime = $lapTimes[$x];
		file_get_contents($ipAddress . "/arduino/0/" . $pacerIndex . "/" . $lapTimes[$x]);		// clear that pacer
		$delayTime = $delayTime;
		sleep($delayTime);
		echo "Status: 204 No Content\r\n\r\n";
		echo "<br />Rest command made it through to Arduino." . $x . " type=" . $type[$x] . " delay=" . $delayTime;
	}
	else if ($type[$x] == 1) {		// rep
		$delayTime = $lapTimes[$x] * ($metersToRun[$x] / 400);
		file_get_contents($ipAddress . "/arduino/7/" . $pacerIndex . "/" . $lapTimes[$x]);		// set the time on that pacer
		file_get_contents($ipAddress . "/arduino/2/" . $pacerIndex . "/" . $lapTimes[$x]);		// reset delay that pacer
		$delayTime = $delayTime + 10;		// adding ten seconds accounts for the countdown delay.
		sleep($delayTime);
		echo "Status: 204 No Content\r\n\r\n";
		echo "<br />Rep command made it through to Arduino." . $x . " type=" . $type[$x] . " delay=" . $delayTime;
	}
}

// now that we're done, clear the pacer
file_get_contents($ipAddress . "/arduino/0/" . $pacerIndex . "/0");		// clear that pacer
echo "<br />Pacer cleared.";
*/

$db->close();

// echo "<br />PHP Executed Successfully";

?>