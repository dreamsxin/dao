<?php 

namespace Dao\Acl {

	/**
	 * Dao\Acl\Adapter initializer
	 */
	
	abstract class Adapter extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Acl\AdapterInterface {

		protected $_defaultAccess;

		protected $_accessGranted;

		protected $_activeRole;

		protected $_activeResource;

		protected $_activeAccess;

		/**
		 * Sets the default access level (Dao\Acl::ALLOW or \Dao\Acl::DENY)
		 *
		 * @param int $defaultAccess
		 */
		public function setDefaultAction($defaultAccess){ }


		/**
		 * Returns the default ACL access level
		 *
		 * @return int
		 */
		public function getDefaultAction(){ }


		/**
		 * Returns the role which the list is checking if it's allowed to certain resource/access
		 *
		 * @return string
		 */
		public function getActiveRole(){ }


		/**
		 * Returns the resource which the list is checking if some role can access it
		 *
		 * @return string
		 */
		public function getActiveResource(){ }


		/**
		 * Returns the access which the list is checking if some role can access it
		 *
		 * @return string
		 */
		public function getActiveAccess(){ }

	}
}
