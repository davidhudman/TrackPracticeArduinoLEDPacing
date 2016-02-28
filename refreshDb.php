<?php
// refreshDb.php
// used to bring the Pin table back to its original state

// $sqlite3 DatabaseName.db
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

/*
// Creating database
$db->query('CREATE TABLE pacer.Pin(
						pacerIndex INT PRIMARY KEY,
						passcode TEXT,
						active INT,
						color INT,
						lapTime REAL
	)');
	
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (0, "123", 0, 1, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (1, "111", 0, 2, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (2, "222", 0, 3, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (3, "333", 0, 4, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (4, "444", 0, 5, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (5, "555", 0, 6, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (6, "666", 0, 7, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (7, "777", 0, 1, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (8, "888", 0, 2, 0)');
$db->query('INSET INTO Pin (pacerIndex, passcode, active, color, lapTime)
						VALUES (9, "999", 0, 3, 0)');						
*/

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

echo "Executed successfully";

$db->close();

?>