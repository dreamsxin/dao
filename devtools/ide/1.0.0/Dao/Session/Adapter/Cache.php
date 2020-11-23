<?php 

namespace Dao\Session\Adapter {

	/**
	 * Dao\Session\Adapter\Cache
	 *
	 * This adapter store sessions in cache
	 *
	 *<code>
	 * $frontCache = new Dao\Cache\Frontend\Data(array(
	 *    "lifetime" => 3600
	 * ));
	 *
	 * $cache = new Dao\Cache\Backend\Redis($frontCache, array(
	 *		'host' => 'localhost',
	 *		'port' => 6379,
	 *		'auth' => 'foobared',
	 *  	'persistent' => false
	 * ));
	 *
	 * $session = new Dao\Session\Adapter\Cache(array(
	 *     'service' => $cache // or service name
	 * ));
	 *
	 * $session->start();
	 *
	 * $session->set('var', 'some-value');
	 *
	 * echo $session->get('var');
	 *</code>
	 */
	
	class Cache extends \Dao\Session\Adapter implements \ArrayAccess, \Traversable, \IteratorAggregate, \Countable, \Dao\Session\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_lifetime;

		protected $_cache;

		/**
		 * Starts the session (if headers are already sent the session will not be started)
		 *
		 * @return boolean
		 */
		public function start(){ }


		/**
		 *
		 * @return boolean
		 */
		public function open($savePath, $sessionName){ }


		/**
		 *
		 * @return boolean
		 */
		public function close(){ }


		/**
		 *
		 * @param string $sessionId
		 * @return mixed
		 */
		public function read($sessionId){ }


		/**
		 *
		 * @param string $sessionId
		 * @param string $data
		 */
		public function write($sessionId, $sessionData){ }


		/**
		 *
		 * @param string $session_id optional, session id
		 *
		 * @return boolean
		 */
		public function destroy($regenerate=null){ }


		/**
		 *
		 * @return boolean
		 */
		public function gc($maxlifetime=null){ }

	}
}
