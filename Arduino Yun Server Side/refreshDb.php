<?php
// refreshDb.php
// used to bring the Pin table back to its original state

// $sqlite3 DatabaseName.db
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

/*
// Creating database
$db->query('CREATE TABLE Pin(
						pacerIndex INT PRIMARY KEY,
						passcode TEXT,
						active INT,
						color INT,
						lapTime REAL,
						name TEXT
	)');
	
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (0, "123", 0, 1, 0, "Pacer0")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (1, "111", 0, 2, 0, "Pacer1")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (2, "222", 0, 3, 0, "Pacer2")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (3, "333", 0, 4, 0, "Pacer3")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (4, "444", 0, 5, 0, "Pacer4")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (5, "555", 0, 6, 0, "Pacer5")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (6, "666", 0, 7, 0, "Pacer6")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (7, "777", 0, 1, 0, "Pacer7")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (8, "888", 0, 2, 0, "Pacer8")');
$db->query('INSERT INTO Pin (pacerIndex, passcode, active, color, lapTime, name)
						VALUES (9, "999", 0, 3, 0, "Pacer9")');							
*/

// Resetting data in database to original
$db->query('UPDATE Pin SET passcode="123", active=0, color=1, lapTime=0, name="Pacer0" WHERE pacerIndex=0');
$db->query('UPDATE Pin SET passcode="111", active=0, color=2, lapTime=0, name="Pacer1" WHERE pacerIndex=1');
$db->query('UPDATE Pin SET passcode="222", active=0, color=3, lapTime=0, name="Pacer2" WHERE pacerIndex=2');
$db->query('UPDATE Pin SET passcode="333", active=0, color=4, lapTime=0, name="Pacer3" WHERE pacerIndex=3');
$db->query('UPDATE Pin SET passcode="444", active=0, color=5, lapTime=0, name="Pacer4" WHERE pacerIndex=4');
$db->query('UPDATE Pin SET passcode="555", active=0, color=6, lapTime=0, name="Pacer5" WHERE pacerIndex=5');
$db->query('UPDATE Pin SET passcode="666", active=0, color=7, lapTime=0, name="Pacer6" WHERE pacerIndex=6');
$db->query('UPDATE Pin SET passcode="777", active=0, color=1, lapTime=0, name="Pacer7" WHERE pacerIndex=7');
$db->query('UPDATE Pin SET passcode="888", active=0, color=2, lapTime=0, name="Pacer8" WHERE pacerIndex=8');
$db->query('UPDATE Pin SET passcode="999", active=0, color=3, lapTime=0, name="Pacer9" WHERE pacerIndex=9');

echo "Executed successfully";

$db->close();

?>