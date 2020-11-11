<?php

$monitor = new Dao\Async\Monitor('/tmp/');

$event = $monitor->awaitEvent();
var_dump($event);

