<?php
// setAllWorkoutsToActive.php

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');


// set all workouts to active
$db->query('UPDATE Workout SET active=1');					


$db->close();

echo "Query Executed Successfully";

?>