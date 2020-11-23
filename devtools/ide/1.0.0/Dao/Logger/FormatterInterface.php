<?php 

namespace Dao\Logger {

	interface FormatterInterface {

		public function format($message, $type, $timestamp, $context);

	}
}
