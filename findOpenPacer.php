<?php
// findOpenPacer.php - may no longer need this file.

$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pacerIndex = -1;		// holds the index of the pacer returned from the query

// queries the DB to find the free pacer with the lowest index
$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');

// assigns the "pacerIndex" query result from the DB to this file's $pacerIndex variable
while ($row = $results->fetchArray()) {
        $pacerIndex = $row['pacerIndex'];
		echo $pacerIndex;
}

$db->exec($query);		// executes the query - may be unnecessary

$results = $db->query('UPDATE Pin SET active = 1 WHERE pacerIndex=' . $pacerIndex);	// updates the DB by setting the available DB pacerIndex to active = 1

$db->exec($query);		// executes the query - may be unnecessary
$db->close();					// closes the database connection
// echo $pIndex != -1 ? $pIndex : "incorrect";
// echo "\nStatement Excuted Successfully". " pacer = " . $pIndex . " lapTime = " . $secondsPerLap;

?>