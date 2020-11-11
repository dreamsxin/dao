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
					break;
				}
			}

			if (empty($ret)) {
				$socket->close();
				return;
			}

			/*
			$sendchunk = 'hello world';
			$sendchunk = \sprintf("HTTP/1.1 200 OK\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", \strlen($sendchunk), $sendchunk);
			$socket->write($sendchunk);
			return;
			*/

			$app = new \Dao\Mvc\Micro();

			// 超始路由
			$app->get('/', function () use ($socket) {
				
				$sendchunk = "<h1>Welcome!</h1>";

				//debug($sendchunk, __LINE__);
				$sendchunk = \sprintf("HTTP/1.1 200 OK\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", \strlen($sendchunk), $sendchunk);
				$socket->write($sendchunk);
			});

			$app->handle($uri);
			return;

			$di = new \Dao\DI;
			$di->set('dispatcher', function () {
				$dispatcher = new \Dao\Mvc\Dispatcher();
				return $dispatcher;
			}, TRUE);
			$di->set('request', function () {
				$request = new \Dao\Http\Request();
				return $request;
			}, TRUE);
			$di->set('response', function () {
				$response = new \Dao\Http\Response();
				return $response;
			}, TRUE);
			$di->set('router', function () {
				$router = new \Dao\Mvc\Router();
				return $router;
			}, TRUE);
			$di->set('view', function () {
				$view = new \Dao\Mvc\View();
				$view->setBasePath(__DIR__.DIRECTORY_SEPARATOR.'mvc/views');
				return $view;
			}, TRUE);

			$application = new \Dao\Mvc\Application;
			$application->useImplicitView(false);
			$sendchunk = $application->handle($uri)->getContent();

			//debug($sendchunk, __LINE__);
			$sendchunk = \sprintf("HTTP/1.1 200 OK\r\nServer: webserver\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", \strlen($sendchunk), $sendchunk);
			$socket->write($sendchunk);
		} catch (\Throwable $e) {
			debug($e->getMessage(), __LINE__);
		} finally {
			//\Dao\Debug::disable();
			$socket->close();
			//debug('CLIENT DISCONNECTED', __LINE__);
		}
	});
}
