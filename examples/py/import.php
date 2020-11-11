<?php

function test() {
	echo 'test';
}
$phalcon = Dao\Py::import('phalcon');
Dao\Py::callFunction($phalcon, 'call', 'test');