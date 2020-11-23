<?php 

namespace Dao\Flash {

	/**
	 * Dao\Flash\Session
	 *
	 * Temporarily stores the messages in session, then messages can be printed in the next request
	 */
	
	class Session extends \Dao\Flash implements \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface, \Dao\FlashInterface {

		/**
		 * Returns the messages stored in session
		 *
		 * @param boolean $remove
		 * @return array
		 */
		protected function _getSessionMessages(){ }


		/**
		 * Stores the messages in session
		 *
		 * @param array $messages
		 */
		protected function _setSessionMessages(){ }


		/**
		 * Adds a message to the session flasher
		 *
		 * @param string $type
		 * @param string $message
		 */
		public function message($type, $message){ }


		/**
		 * Returns the messages in the session flasher
		 *
		 * @param string $type
		 * @param boolean $remove
		 * @return array
		 */
		public function getMessages($type=null, $remove=null){ }


		/**
		 * Prints the messages in the session flasher
		 *
		 * @param string $type
		 * @param boolean $remove
		 */
		public function output($type=null, $remove=null){ }


		/**
		 * bool \Dao\Flash\Session::has(string $type)
		 *
		 * @see https://github.com/phalcon/cphalcon/issues/1342
		 */
		public function has($type){ }

	}
}
