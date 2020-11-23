<?php 

namespace Dao\Session\Adapter {

	/**
	 * Dao\Session\Adapter\Memcached
	 *
	 * This adapter store sessions in memcached
	 *
	 *<code>
	 * $session = new Dao\Session\Adapter\Memcached(array(
	 *     'servers' => array(
	 *         array('host' => 'localhost', 'port' => 11211, 'weight' => 1),
	 *     ),
	 *     'client' => array(
	 *         Memcached::OPT_HASH => Memcached::HASH_MD5,
	 *         Memcached::OPT_PREFIX_KEY => 'prefix.',
	 *     ),
	 *    'lifetime' => 3600,
	 *    'prefix' => 'my_'
	 * ));
	 *
	 * $session->start();
	 *
	 * $session->set('var', 'some-value');
	 *
	 * echo $session->get('var');
	 *</code>
	 */
	
	class Memcached extends \Dao\Session\Adapter implements \ArrayAccess, \Traversable, \IteratorAggregate, \Countable, \Dao\Session\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_lifetime;

		protected $_memcached;

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
