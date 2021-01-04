<?php

var_dump(Dao\Async\Process\Process::isWorker());
var_dump($ipc = Dao\Async\Process\Process::connect());

$loader = new \Dao\Loader();

$loader->registerDirs(
		array(
			__DIR__.'/mvc/controller',
		)
)->register();

function debug(string $data, $lineno = 'NULL') {
	$message = ['data' => $data, 'line' => $lineno];
	print_r($data);
}

while ($socket = Dao\Async\Network\TcpSocket::import($ipc)) {

	Dao\Async\Task::async(function () use ($socket) {
		//\Dao\Debug::enable();
		try {
			$uri = $chunk = '';
			$ret = NULL;
			$parser = new \Dao\Http\Parser();
			while($buffer = $socket->read()) {
				$ret = $parser->execute($buffer);
				if (!$ret) {
					throw new \Exception('HTTP parse failed');
				}
				if ($parser->status() == \Dao\Http\Parser::STATUS_END) {
					$uri = \Dao\Arr::get($ret, 'QUERY_STRING');
					$body = \Dao\Arr::get($ret, 'BODY');

					$app = new \Dao\Mvc\Micro();

					$app->get('/', function () {
						return "<h1>Welcome!</h1>";
					});

					$sendchunk = $app->handle($uri);
					$sendchunk = \sprintf("HTTP/1.1 200 OK\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: %s\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", ($parser->isKeepAlive() ? 'Keep-Alive' : 'Closed'), \strlen($sendchunk), $sendchunk);
					$socket->write($sendchunk);
					if (!$parser->isKeepAlive()) {
						break;
					} else {
						$parser = new \Dao\Http\Parser();
					}
				}
			}

			$socket->close();
		} catch (\Error $e) {
			$sendchunk = "<h1>Error!</h1>";
			$sendchunk = \sprintf("HTTP/1.1 400 NOT FOUND\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", \strlen($sendchunk), $sendchunk);
			$socket->write($sendchunk);
			debug($e->getMessage(), __LINE__);
		} catch (\Throwable $e) {
			$sendchunk = "<h1>Not found!</h1>";
			$sendchunk = \sprintf("HTTP/1.1 404 NOT FOUND\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", \strlen($sendchunk), $sendchunk);
			$socket->write($sendchunk);
			debug($e->getMessage(), __LINE__);
		} finally {
			//\Dao\Debug::disable();
			$socket->close();
			//debug('CLIENT DISCONNECTED', __LINE__);
		}
	});
}
