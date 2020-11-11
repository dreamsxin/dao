<?php

namespace Dao\Async\Network;

list ($a, $b) = TcpSocket::pair();

\Dao\Async\Task::async(function () use ($a) {
    try {
        $a->write('Hello World :)');
    } finally {
        $a->close();
    }
});

var_dump($b->read());
