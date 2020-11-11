<?php

error_reporting(-1);

$cond = new Dao\Async\Sync\Condition();

$available = 0;
$consumed = 0;

Dao\Async\Task::async(function () use ($cond, & $available) {
    $timer = new Dao\Async\Timer(500);

    while (true) {
        $timer->awaitTimeout();

        $available += random_int(0, 2);
        $cond->signal();
    }
});

try {
    while ($consumed < 5) {
        while ($available == 0) {
            var_dump('WAIT');
            $cond->wait();
            var_dump('SIGNALED');
        }

        var_dump($available);

        $consumed += $available;
        $available = 0;
    }
} finally {
    var_dump('DONE');

    $cond->close();
}
