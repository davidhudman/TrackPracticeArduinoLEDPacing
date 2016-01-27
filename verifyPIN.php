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

$pIndex = "-1";
$q = -1;

if (empty($_REQUEST["q"])) {
// do nothing
}
else {
$q = $_REQUEST["q"];
}

$results = $db->query('Select * from Pin WHERE passcode=' . $q);
while ($row = $results->fetchArray()) {
// echo var_dump($row);
$pIndex = $row['pacerIndex'];
}
$db->exec($query);
$db->close();
echo $pIndex != "-1" ? $pIndex : "incorrect";
// echo "\nStatement Excuted Successfully";

?>