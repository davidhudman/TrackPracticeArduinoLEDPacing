<?php
// findOpenPacer.php

/* Functional code providing some limited validation. */
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

// select * from pin where active = 0 order by pacerindex asc;
$pacerIndex = -1;

$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');

while ($row = $results->fetchArray()) {
        $pacerIndex = $row['pacerIndex'];
		echo $pacerIndex;
}

$db->exec($query);

$results = $db->query('UPDATE Pin SET active = 1 WHERE pacerIndex=' . $pacerIndex);

$db->exec($query);
$db->close();
// echo $pIndex != -1 ? $pIndex : "incorrect";
// echo "\nStatement Excuted Successfully". " pacer = " . $pIndex . " lapTime = " . $secondsPerLap;

?>