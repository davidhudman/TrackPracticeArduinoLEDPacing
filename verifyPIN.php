<?php

// get the q parameter from URL
$q = $_REQUEST["q"];

$len=strlen($q);

// Output "no suggestion" if no hint was found or output correct values 
echo $q == "123" ? $q : "incorrect";

?>