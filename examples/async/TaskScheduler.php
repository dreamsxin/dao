<?php

$result = Dao\Async\TaskScheduler::run(function () {
    $t = Dao\Async\Task::async(function (): int {
        return max(123, Dao\Async\Task::await(Dao\Async\Deferred::value()));
    });

    printf("LAUNCHED TASK: [%s]\nin %s:%u\n\n", $t->status, $t->file, $t->line);
    
    print_r($t);
    
    try {
        Dao\Async\Task::await(Dao\Async\Deferred::error(new \Error('Fail!')));
    } catch (\Throwable $e) {
        var_dump($e->getMessage());
    }
    
    var_dump(2 * Dao\Async\Task::await($t));
    
    return 777;
}, function (array $tasks) {
    print_r($tasks);
});

$timer = new Dao\Async\Timer(500);
$timer->awaitTimeout();
var_dump($result);
