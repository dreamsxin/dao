<?php

class Pool
{
	private $channel;
	private $concurrency;
	private $count = 0;
	private $context;

	public function __construct(int $concurrency = 1, int $capacity = 0)
	{
		$this->concurrency = max(1, $concurrency);
		$this->channel = new \Dao\Async\Channel($capacity);
		$this->context = \Dao\Async\Context::background();
	}

	public function close(?\Throwable $e = null): void
	{
		$this->count = \PHP_INT_MAX;
		$this->channel->close($e);
	}

	public function submit(callable $work, $socket, ...$args): \Dao\Async\Awaitable
	{
		if ($this->count < $this->concurrency) {
			$this->count++;
			LmdbServer::info('Pool count '.$this->count);
			\Dao\Async\Task::asyncWithContext($this->context, static function (iterable $it) {
				try {
					foreach ($it as list ($defer, $context, $work, $socket, $args)) {
						try {
							$defer->resolve($context->run($work, $socket, ...$args));
						} catch (\Throwable $e) {
							LmdbServer::err($e->getMessage());
							$defer->fail($e);
						} finally {
						}
					}
				} catch (\Throwable $e) {
					LmdbServer::err($e->getMessage());
				} finally {
					--$this->count;
				}
			}, $this->channel->getIterator());
		}

		$this->channel->send([
			$defer = new \Dao\Async\Deferred(),
			\Dao\Async\Context::current(),
			$work,
			$socket,
			$args
		]);

		return $defer->awaitable();
	}
}

class LmdbServer
{
	static public $debug = false;
	private $server;

	private $host;
	private $port;

	private $clients;

	public $pattern='#^[\w\x{4e00}-\x{9fa5}]+$#u';
	public $filenamepattern='#^[\w]+.[a-zA-Z0-9]+$#';

	public function __construct($host, $port, callable $callback = NULL, int $concurrency = 1, int $capacity = 0)
	{
		$this->host = $host;
		$this->port = $port;
		$this->callback = $callback;
		$this->pool = new Pool($concurrency, $capacity);
	}

	public function SendHttp($socket, $sendchunk, $header = 'text/html; charset=utf-8', $isKeepAlive = FALSE) {
		$sendchunk = \sprintf("HTTP/1.1 200 OK\r\nServer: webserver\r\nContent-Type: %s\r\nTransfer-Encoding: chunked\r\nConnection: %s\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", ($header ?: 'text/html; charset=utf-8'), ($isKeepAlive ? 'Keep-Alive' : 'Closed'), \strlen($sendchunk), $sendchunk);
		$socket->write($sendchunk);
	}

	public function start()
	{
		$callback = $this->callback;
		$ws = $this;
		$worker = static function ($socket) use ($ws, $callback) {
			
			$parser = new \Dao\Http\Parser();

			try {
				$buffer = '';
				while (null !== ($chunk = $socket->read())) {
					$ret = $parser->execute($chunk);
					if (!$ret) {
						throw new \Exception('HTTP parse failed');
					}
					if ($parser->status() == \Dao\Http\Parser::STATUS_END) {
						$uri = \Dao\Arr::get($ret, 'QUERY_STRING');
						$body = \Dao\Arr::get($ret, 'BODY');
						$data = parse_url($uri);
						$path = \Dao\Arr::get($data, 'path');
						$query = \Dao\Arr::get($data, 'query');
						parse_str($query, $data);
						$filename = \Dao\Arr::get($data, 'filename');

						if (!in_array($path, ['/download'])) {
							$ws->SendHttp($socket, '非法路径');
						} elseif (empty($filename) || !is_string($filename)) {
							$ws->SendHttp($socket, '非法文件名');
						} else {
							$db = new \Dao\Storage\Lmdb(__DIR__.'/lmdb/files', NULL, NULL, NULL, \Dao\Storage\Lmdb::NOSYNC|\Dao\Storage\Lmdb::NOLOCK);
							$db->begin(\Dao\Storage\Lmdb::RDONLY);
							$blob = $db->get($filename);
							$db->commit();
							unset($db);

							$ext = pathinfo($filename, PATHINFO_EXTENSION);
							
							switch ($ext) {
								case 'jpg':
								case 'jpeg':
								case 'png':
									$head = 'image/jpg;';
									break;
								default:
									$head = 'application/octet-stream';
									break;
							}
							if ($blob) {
								$ws->SendHttp($socket, $blob, $head, $parser->isKeepAlive());
							} else {
								$ws->SendHttp($socket, '没有找到文件', NULL, $parser->isKeepAlive());
							}
						}

						if (!$parser->isKeepAlive()) {
							break;
						} else {
							$parser = new \Dao\Http\Parser();
						}
					}
				}
			} catch (\Throwable $e) {
				self::err($e->getMessage());
			} finally {
				$socket->close();
			}
		};

		try {
			$this->server = \Dao\Async\Network\TcpServer::listen($this->host, $this->port);
			echo Dao\Cli\Color::info('start server listen:'.$this->host.':'.$this->port).PHP_EOL;
			
			file_put_contents(__DIR__. '/'.basename(__FILE__, '.php').'.pid', getmypid());
			while (true) {
				$socket = $this->server->accept();
				if ($socket === false) {
					continue;
				}
				$this->pool->submit($worker, $socket);
			}
		} catch (\Throwable $e) {
			self::err($e->getMessage());
		} finally {
			if ($this->server) {
				$this->server->close();
			}
		}

	}

	static public function info($message)
	{
		if (self::$debug) {
			echo Dao\Cli\Color::info($message).PHP_EOL;
		}
	}

	static public function err($message)
	{
		echo Dao\Cli\Color::error($message).PHP_EOL;
	}
}

$opts = new \Dao\Cli\Options('Websocket CLI');
$opts->add([
    'type' => \Dao\Cli\Options::TYPE_INT,
    'name' => 'port',
    'shortName' => 'p',
    'required' => false,
	'help' => "port"
]);
$opts->add([
    'type' => \Dao\Cli\Options::TYPE_BOOLEAN,
    'name' => 'concurrency',
    'shortName' => 'c',
    'required' => false
]);
$opts->add([
    'type' => \Dao\Cli\Options::TYPE_BOOLEAN,
    'name' => 'capacity',
    'shortName' => 'C',
    'required' => false
]);
$opts->add([
    'type' => \Dao\Cli\Options::TYPE_BOOLEAN,
    'name' => 'debug',
    'shortName' => 'v',
    'required' => false,
	'help' => "enable debug"
]);
$vals = $opts->parse();
if ($vals === false ) {
	exit;
}
/**
 * 运行 php LmdbServer.php
 */

$server = new LmdbServer('0.0.0.0', \Dao\Arr::get($vals, 'port', 8080), function($socket, $status, $data) {
	// TODO
}, \Dao\Arr::get($vals, 'concurrency', 500), \Dao\Arr::get($vals, 'capacity', 1));
$server->start();
