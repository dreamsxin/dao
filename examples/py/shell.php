<?php

echo "Python in PHP (" . Dao\Py::version() . ")\n";

$stdin = fopen('php://stdin', 'r');

while (!feof($stdin)) {
    echo ">>> ";
    $line = rtrim(fgets($stdin));
    Dao\Py::exec($line);
}
