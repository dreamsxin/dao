<?php 

namespace Dao\Acl {

	/**
	 * Dao\Acl\Role
	 *
	 * This class defines role entity and its description
	 *
	 */
	
	class Role implements \Dao\Acl\RoleInterface {

		protected $_name;

		protected $_description;

		/**
		 * \Dao\Acl\Role description
		 *
		 * @param string $name
		 * @param string $description
		 */
		public function __construct($name, $description=null){ }


		/**
		 * Returns the role name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Returns role description
		 *
		 * @return string
		 */
		public function getDescription(){ }


		/**
		 * Magic method __toString
		 *
		 * @return string
		 */
		public function __toString(){ }

	}
}
