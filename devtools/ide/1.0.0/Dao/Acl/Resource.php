<?php 

namespace Dao\Acl {

	/**
	 * Dao\Acl\Resource
	 *
	 * This class defines resource entity and its description
	 *
	 */
	
	class Resource implements \Dao\Acl\ResourceInterface {

		protected $_name;

		protected $_description;

		/**
		 * \Dao\Acl\Resource constructor
		 *
		 * @param string $name
		 * @param string $description
		 */
		public function __construct($name, $description=null){ }


		/**
		 * Returns the resource name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Returns resource description
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
