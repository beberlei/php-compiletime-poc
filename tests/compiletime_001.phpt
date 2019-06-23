--TEST--
Compiletime: Calculate how long complication takes
--FILE--
<?php

$start = compiletime_get_current();

$s = microtime(true);
require_once __DIR__ . "/foo.php";

$afterRequire = compiletime_get_current();

if ($start > $afterRequire) {
    echo "FAIL!";
}
var_dump($afterRequire);
--EXPECTF--
int(%d)
