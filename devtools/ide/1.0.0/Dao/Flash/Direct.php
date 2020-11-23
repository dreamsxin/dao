<?php 

namespace Dao\Flash {

	/**
	 * Dao\Flash\Direct
	 *
	 * This is a variant of the Dao\Flash that inmediately outputs any message passed to it
	 */
	
	class Direct extends \Dao\Flash implements \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface, \Dao\FlashInterface {

		/**
		 * Outputs a message
		 *
		 * @param  string $type
		 * @param  string $message
		 * @return string
		 */
		public function message($type, $message){ }

	}
}
