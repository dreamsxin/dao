<?php 

namespace Dao\Async\Process {

	final class ProcessBuilder {

		const STDIN = 0;

		const STDOUT = 1;

		const STDERR = 2;

		public function __construct($command, $arguments=null){ }


		public function __wakeup(){ }


		public static function fork($file){ }


		public static function shell($interactive=null){ }


		public function withCwd($directory){ }


		public function withEnv($env, $inherit=null){ }


		public function withStdinPipe(){ }


		public function withStdinInherited($fd=null){ }


		public function withoutStdin(){ }


		public function withStdoutPipe(){ }


		public function withStdoutInherited($fd=null){ }


		public function withoutStdout(){ }


		public function withStderrPipe(){ }


		public function withStderrInherited($fd=null){ }


		public function withoutStderr(){ }


		public function execute($arguments=null){ }


		public function start($arguments=null){ }


		public function daemon($arguments=null){ }

	}
}
