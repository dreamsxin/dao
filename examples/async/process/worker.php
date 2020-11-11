<?php

var_dump(Dao\Async\Process\Process::isWorker());
var_dump($ipc = Dao\Async\Process\Process::connect());

var_dump($tcp = Dao\Async\Network\TcpSocket::import($ipc));

try {
    while (null !== ($chunk = $tcp->read())) {
        echo $chunk;
    }
} finally {
    $tcp->close();
}
