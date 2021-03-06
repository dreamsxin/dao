<?php 

namespace Dao\Acl\Adapter {

	/**
	 * Dao\Acl\Adapter\Memory
	 *
	 * Manages ACL lists in memory
	 *
	 *<code>
	 *
	 *	$acl = new Dao\Acl\Adapter\Memory();
	 *
	 *	$acl->setDefaultAction(Dao\Acl::DENY);
	 *
	 *	//Register roles
	 *	$roles = array(
	 *		'users' => new Dao\Acl\Role('Users'),
	 *		'guests' => new Dao\Acl\Role('Guests')
	 *	);
	 *	foreach ($roles as $role) {
	 *		$acl->addRole($role);
	 *	}
	 *
	 *	//Private area resources
	 *  $privateResources = array(
	 *		'companies' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
	 *		'products' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
	 *		'invoices' => array('index', 'profile')
	 *	);
	 *	foreach ($privateResources as $resource => $actions) {
	 *		$acl->addResource(new Dao\Acl\Resource($resource), $actions);
	 *	}
	 *
	 *	//Public area resources
	 *	$publicResources = array(
	 *		'index' => array('index'),
	 *		'about' => array('index'),
	 *		'session' => array('index', 'register', 'start', 'end'),
	 *		'contact' => array('index', 'send')
	 *	);
	 *  foreach ($publicResources as $resource => $actions) {
	 *		$acl->addResource(new Dao\Acl\Resource($resource), $actions);
	 *	}
	 *
	 *  //Grant access to public areas to both users and guests
	 *	foreach ($roles as $role){
	 *		foreach ($publicResources as $resource => $actions) {
	 *			$acl->allow($role->getName(), $resource, '*');
	 *		}
	 *	}
	 *
	 *	//Grant access to private area to role Users
	 *  foreach ($privateResources as $resource => $actions) {
	 * 		foreach ($actions as $action) {
	 *			$acl->allow('Users', $resource, $action);
	 *		}
	 *	}
	 *
	 *</code>
	 */
	
	class Memory extends \Dao\Acl\Adapter implements \Dao\Acl\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_rolesNames;

		protected $_roles;

		protected $_resourcesNames;

		protected $_resources;

		protected $_access;

		protected $_funcs;

		protected $_roleInherits;

		protected $_accessList;

		/**
		 * \Dao\Acl\Adapter\Memory constructor
		 *
		 */
		public function __construct(){ }


		/**
		 * Adds a role to the ACL list. Second parameter allows inheriting access data from other existing role
		 *
		 * Example:
		 * <code>
		 * 	$acl->addRole(new \Dao\Acl\Role('administrator'), 'consultant');
		 * 	$acl->addRole('administrator', 'consultant');
		 * </code>
		 *
		 * @param  \Dao\Acl\RoleInterface|string $role
		 * @param  array|string $accessInherits
		 * @return boolean
		 */
		public function addRole($role, $accessInherits=null){ }


		/**
		 * Do a role inherit from another existing role
		 *
		 * @param string $roleName
		 * @param string $roleToInherit
		 */
		public function addInherit($roleName, $roleToInherit){ }


		/**
		 * Check whether role exist in the roles list
		 *
		 * @param  string $roleName
		 * @return boolean
		 */
		public function isRole($roleName){ }


		/**
		 * Check whether resource exist in the resources list
		 *
		 * @param  string $resourceName
		 * @return boolean
		 */
		public function isResource($resourceName){ }


		/**
		 * Adds a resource to the ACL list
		 *
		 * Access names can be a particular action, by example
		 * search, update, delete, etc or a list of them
		 *
		 * Example:
		 * <code>
		 * //Add a resource to the the list allowing access to an action
		 * $acl->addResource(new \Dao\Acl\Resource('customers'), 'search');
		 * $acl->addResource('customers', 'search');
		 *
		 * //Add a resource  with an access list
		 * $acl->addResource(new \Dao\Acl\Resource('customers'), array('create', 'search'));
		 * $acl->addResource('customers', array('create', 'search'));
		 * </code>
		 *
		 * @param \Dao\Acl\Resource|string $resource
		 * @param array $accessList
		 * @return boolean
		 */
		public function addResource($resource, $accessList=null){ }


		/**
		 * Adds access to resources
		 *
		 * @param string $resourceName
		 * @param mixed $accessList
		 */
		public function addResourceAccess($resourceName, $accessList){ }


		/**
		 * Removes an access from a resource
		 *
		 * @param string $resourceName
		 * @param mixed $accessList
		 */
		public function dropResourceAccess($resourceName, $accessList){ }


		/**
		 * Checks if a role has access to a resource
		 *
		 * @param string $roleName
		 * @param string $resourceName
		 * @param string $access
		 * @param string $action
		 * @param callable $callback
		 */
		protected function _allowOrDeny($roleName, $resourceName, $access, $action, $callback=null){ }


		/**
		 * Allow access to a role on a resource
		 *
		 * You can use '*' as wildcard
		 *
		 * Example:
		 * <code>
		 * //Allow access to guests to search on customers
		 * $acl->allow('guests', 'customers', 'search');
		 *
		 * //Allow access to guests to search or create on customers
		 * $acl->allow('guests', 'customers', array('search', 'create'));
		 *
		 * //Allow access to any role to browse on products
		 * $acl->allow('*', 'products', 'browse');
		 *
		 * //Allow access to any role to browse on any resource
		 * $acl->allow('*', '*', 'browse');
		 * </code>
		 *
		 * @param string $roleName
		 * @param string $resourceName
		 * @param mixed $access
		 * @param callable $callback
		 */
		public function allow($roleName, $resourceName, $access, $callback=null){ }


		/**
		 * Deny access to a role on a resource
		 *
		 * You can use '*' as wildcard
		 *
		 * Example:
		 * <code>
		 * //Deny access to guests to search on customers
		 * $acl->deny('guests', 'customers', 'search');
		 *
		 * //Deny access to guests to search or create on customers
		 * $acl->deny('guests', 'customers', array('search', 'create'));
		 *
		 * //Deny access to any role to browse on products
		 * $acl->deny('*', 'products', 'browse');
		 *
		 * //Deny access to any role to browse on any resource
		 * $acl->deny('*', '*', 'browse');
		 * </code>
		 *
		 * @param string $roleName
		 * @param string $resourceName
		 * @param mixed $access
		 * @param callable $callback
		 * @return boolean
		 */
		public function deny($roleName, $resourceName, $access, $callback=null){ }


		/**
		 * Check whether a role is allowed to access an action from a resource
		 *
		 * <code>
		 * //Does andres have access to the customers resource to create?
		 * $acl->isAllowed('andres', 'Products', 'create');
		 *
		 * //Do guests have access to any resource to edit?
		 * $acl->isAllowed('guests', '*', 'edit');
		 * </code>
		 *
		 * @param mixed $role
		 * @param mixed $resource
		 * @param string $access
		 * @param mixed $data
		 * @return boolean
		 */
		public function isAllowed($role, $resource, $access, $data=null){ }


		/**
		 * Return an array with every role registered in the list
		 *
		 * @return \Dao\Acl\Role[]
		 */
		public function getRoles(){ }


		/**
		 * Return an array with every resource registered in the list
		 *
		 * @return \Dao\Acl\Resource[]
		 */
		public function getResources(){ }


		/**
		 * Return the access action
		 *
		 * @return string[]
		 */
		public function getAccess(){ }

	}
}
