<?php

$signal = new Dao\Async\Signal(Dao\Async\Signal::SIGINT);
var_dump(getenv('PATH'));
echo "START: \"", getenv('MY_TITLE'), "\"\n";

$signal->awaitSignal();

echo "END!";

exit(7);
