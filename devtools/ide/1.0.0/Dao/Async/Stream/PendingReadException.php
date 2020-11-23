<?php 

namespace Dao\Async\Stream {

	class PendingReadException extends \Dao\Async\Stream\StreamException implements \Throwable, \Stringable {
	}
}
