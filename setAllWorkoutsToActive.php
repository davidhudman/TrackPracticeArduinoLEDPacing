<?php
// setAllWorkoutsToActive.php
// used to set all workouts in the database for all pacers to "active=1" to indicate that they need to be run

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');


// set all workouts to active for all pacers
$db->query('UPDATE Workout SET active=1');					


$db->close();

echo "All workouts successfully set to active.";

?>