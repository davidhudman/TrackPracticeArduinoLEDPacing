<?php
// createWorkoutTable.php
// used to create the workout table in the pacer database - this is a backup of the original table - this won't work if the table already exists

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');


// Creating database
$db->query('CREATE TABLE Workout(
						workoutIndex INTEGER PRIMARY KEY AUTOINCREMENT,
						pacerIndex INT NOT NULL,
						repeat INT,
						remaining INT,
						active INT,
						type INT,
						secondsPerLap REAL,
						meters INT,
						next INT,
						previous INT
	)');
	

$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (0, 0, 1, 1, 1, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (1, 0, 1, 1, 1, 0, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (2, 0, 1, 1, 1, 1, 10, 400)');
$db->query('INSERT INTO Workout (workoutIndex, pacerIndex, repeat, remaining, active, type, secondsPerLap, meters)
						VALUES (3, 0, 1, 1, 1, 0, 10, 400)');

$db->close();

echo "Query Executed Successfully";

?>