<?php 

namespace Dao\Forms {

	/**
	 * Dao\Forms\Manager
	 *
	 * Manages forms within the application. Allowing the developer to access them from
	 * any part of the application
	 */
	
	class Manager {

		protected $_forms;

		public function __construct(){ }


		/**
		 * Creates a form registering it in the forms manager
		 *
		 * @param string $name
		 * @param object $entity
		 * @return \Dao\Forms\Form
		 */
		public function create($name=null, $entity=null){ }


		/**
		 * Returns a form by its name
		 *
		 * @param string $name
		 * @return \Dao\Forms\Form
		 */
		public function get($name){ }


		/**
		 * Checks if a form is registered in the forms manager
		 *
		 * @param string $name
		 * @return boolean
		 */
		public function has($name){ }


		/**
		 * Registers a form in the Forms Manager
		 *
		 * @param string $name
		 * @param \Dao\Forms\Form $form
		 * @return \Dao\Forms\Manager
		 */
		public function set($name, $form){ }

	}
}
