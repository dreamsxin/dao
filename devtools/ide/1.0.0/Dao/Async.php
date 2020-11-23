<?php 

namespace Dao {

	/**
	 * Dao\Async
	 */
	
	class Async {

		const NOWAIT = 1;

		const MSG_NOERROR = 2;

		const MSG_EXCEPT = 4;

		protected static $_num;

		protected static $_filename;

		protected static $_proj;

		/**
		 * Called asynchronous
		 *
		 *<code>
		 *	$async = \Dao\Async::call(function () {
		 *		return 'one';
		 *	 });
		 *</code>
		 *
		 * @param closure $callable
		 * @return int
		 */
		public static function call(\Closure $closure, $arguments=null){ }


		/**
		 * Gets asynchronous result
		 *
		 *<code>
		 *	$id = \Dao\Async::call(function () {
		 *		return 'one';
		 *	});
		 *	$data = \Dao\Async::recv($id);
		 *</code>
		 *
		 * @param int $pid
		 * @param int $flag
		 * @return mixed
		 */
		public static function recv($pid, $flag=null){ }


		/**
		 * Gets all asynchronous result
		 *
		 *<code>
		 *	$id = \Dao\Async::call(function () {
		 *		return 'one';
		 *	});
		 *	$data = \Dao\Async::recvAll();
		 *</code>
		 *
		 * @return array
		 */
		public static function recvAll($flag=null){ }


		/**
		 * Gets result count
		 *
		 *<code>
		 *	Dao\Async::count();
		 *</code>
		 *
		 * @return int
		 */
		public static function count(){ }


		/**
		 * Destroy asynchronous
		 *
		 *<code>
		 *	Dao\Async::clear();
		 *</code>
		 *
		 * @return bool
		 */
		public static function clear(){ }


		/**
		 * Sets the filename to a System V IPC key
		 *
		 *<code>
		 * \Dao\Async::setFilename('/tmp');
		 *</code>
		 *
		 * @param string $filename
		 */
		public static function setFilename($filename){ }


		/**
		 * Gets the filename
		 *
		 *<code>
		 * \Dao\Async::getFilename();
		 *</code>
		 *
		 * @return string
		 */
		public static function getFilename(){ }

	}
}
