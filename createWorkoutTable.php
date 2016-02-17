<?php
// createWorkoutTable.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');


// Creating database
$db->query('CREATE TABLE Workout(
						workoutIndex INT PRIMARY KEY,
						pacerIndex INT,
						repeat INT,
						remaining INT,
						active INT,
						type TEXT,
						secondsPerLap REAL,
						meters INT,
						next INT,
						previous INT
	)');

		// "type" should actually be INT, not TEXT
	
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (0, 0, 1, 1, 0, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (1, 0, 1, 1, 0, 0, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (2, 0, 1, 1, 0, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (3, 0, 1, 1, 0, 0, 10, 400)');						

/*
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(0, 0, 0, 1, 1, 0, 1, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(1, 0, 0, 1, 1, 0, 0, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(2, 0, 0, 1, 1, 0, 1, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(3, 0, 0, 1, 1, 0, 0, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(4, 0, 0, 1, 1, 0, 1, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(5, 0, 0, 1, 1, 0, 0, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(6, 0, 0, 1, 1, 0, 1, 10, 400\)');
$db->query('INSERT INTO Workout \(workoutIndex, pacerIndex, group, repeat, remaining, active, type, secondsPerLap, meters\)
						VALUES \(7, 0, 0, 1, 1, 0, 0, 10, 400\)');	
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

$db->close();

echo "Query Executed Successfully";

?>