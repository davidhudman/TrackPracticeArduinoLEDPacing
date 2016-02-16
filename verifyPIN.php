<?php
/*
    $q = $_REQUEST["q"];
    $pIndex = "-1";

    class MyDB extends SQLite3
    {
        function __construct()
        {
            $this->open('/root/pacer.db');
        }
    }
    $db = new MyDB();
    if(!$db) {
        // echo $db->lastErrorMsg();
    } else {
        // echo "Opened database successfully\n";
    }

    $sql =<<<EOF
        // --- Error?: I don't know if you can write "= $q" like that or not
        SELECT * FROM Pin WHERE passcode='123';
EOF;

    $ret = $db->query($sql);
    while($row = $ret->fetchArray(SQLITE3_ASSOC) ){
        $pIndex = $row['pacerIndex'];
        // echo $row['pacerIndex'];
        // echo "SALARY =  ".$row['SALARY'] ."\n\n";
    }
    // echo "Operation done successfully\n";
    $db->close();

    // --- Error?: This '!= -1' may just need to be '!- "-1"'
    echo $pIndex != "-1" ? $pIndex : "incorrect";
?>
*/

/*
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');
// $query = "Select * from Pin";
// "CREATE TABLE Pin (pacerIndex INT, passcode TEXT, active INT, color INT, lapTime REAL)";
// "INSERT INTO Pin ( pacerIndex, passcode, active, color, lapTime) VALUES ( 0, '123', 0, 1, 0), ( 1, '111', 0, 2, 0),  ( 2, '222', 0, 3, 0), ( 3, '333', 0, 4, 0)";

$myNum = 111;

$results = $db->query('Select * from Pin WHERE passcode=' . $myNum);
while ($row = $results->fetchArray()) {
// echo var_dump($row);
echo $row['passcode'];
}
$db->exec($query);
$db->close();
echo "select executed";
*/

/* Functional code providing some limited validation */
$db = new SQLite3('/mnt/sda1/arduino/www/TrackPractice/pacer.db');

$pIndex = "-1";					// the pacerIndex from the request that needs to be verified PIN
$pin = "-1";							// the PIN from the request that needs to be verified with the pIndex
$DbPin = "-1";						// the correct PIN
$needNewPacer = "-1";		// the flag that indicates whether a new pacer is needed
$lapTime = -1;					// the pace sent from a coach requesting a new pacer at a specific lap time

// Pull all your values out of the GET request
if (empty($_REQUEST["pin"])) {
	// do nothing
}
else {
	// do nothing - for some reason it may get recognized as "empty" even though there are clearly parameters
	$pin = $_REQUEST["pin"];
	$pIndex = $_REQUEST["pacerIndex"];
	$needNewPacer = $_REQUEST["needNewPacer"];
	$lapTime = $_REQUEST["lapTime"];
}

// If the flag for requesting a new Pacer is true - MAY BE UNNECESSARY AS WELL AS ITS VARIABLE ABOVE
if ($needNewPacer != "1") {
	$results = $db->query('Select * from Pin WHERE passcode=' . $pin . ' AND pacerIndex=' . $pIndex);
	while ($row = $results->fetchArray()) {
	// echo var_dump($row);
	$DbPin = $row['passcode'];
	}
	$db->exec($query);
	$db->close();
	
	if ($DbPin == $pin && $DbPin != "-1") {
		echo $pIndex;
		// echo "\nStatement Excuted Successfully";
	}
	else {
		echo "incorrect";
	}
}
else {		// the user is requesting a new pacer
	$pacerIndex = -1;
	$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');

	while ($row = $results->fetchArray()) {
			$pacerIndex = $row['pacerIndex'];
	}

	$db->exec($query);

	if ($pacerIndex != -1) {		// There is a pacer available
		if ($lapTime == -1) {		// We're in the createNewPacerSequence
			$results = $db->query('UPDATE Pin SET active = 1, passcode=' . $pin . ' WHERE pacerIndex=' . $pacerIndex);
			$db->exec($query);
			echo $pacerIndex;
			// echo "pacerIndex did not equal -1";
		}
		else {							// This is when we're in the "Coach Mode" and the coach is creating a pacer with its new time simultaneously.
			$results = $db->query('UPDATE Pin SET active = 1, passcode=' . $pin . ', lapTime=' . $lapTime . ' WHERE pacerIndex=' . $pacerIndex);
			$db->exec($query);
			echo $pacerIndex;
		}
	}
	else {	// There is no pacer available
		// Do nothing (you already sent the "-1" flag)
		echo $pacerIndex;
	}
	
	$db->close();
}

?>