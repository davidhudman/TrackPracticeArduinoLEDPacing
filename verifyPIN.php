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

// Pull all your values out of the GET request
if (empty($_REQUEST["pin"])) {
	// do nothing
}
else {
	$pin = $_REQUEST["pin"];

	// parse the PIN to be a character array
	$length = strlen($pin);
	$pinAsArray = array();
	for ($i=0; $i<$length; $i++) {
		$pinAsArray[$i] = $pin[$i];
	}

	$pIndex = $_REQUEST["pacerIndex"];
	$needNewPacer = $_REQUEST["needNewPacer"];
}

// If the flag for requesting a new Pacer is true - MAY BE UNNECESSARY AS WELL AS ITS VARIABLE ABOVE
if ($needNewPacer != "1") {
	$results = $db->query('Select * from Pin WHERE passcode=' . $pinAsArray[0] . $pinAsArray[1] . $pinAsArray[2] . ' AND pacerIndex=' . $pIndex);
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
	$results = $db->query('SELECT * FROM Pin WHERE active = 0 ORDER BY pacerIndex ASC LIMIT 1');

	while ($row = $results->fetchArray()) {
			$pacerIndex = $row['pacerIndex'];
			echo $pacerIndex;
	}

	$db->exec($query);

	$results = $db->query('UPDATE Pin SET active = 1, passcode=' . $pinAsArray[0] . $pinAsArray[1] . $pinAsArray[2] . ' WHERE pacerIndex=' . $pacerIndex);

	$db->exec($query);
	$db->close();
}

?>