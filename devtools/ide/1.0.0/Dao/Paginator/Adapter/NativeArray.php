<?php 

namespace Dao\Paginator\Adapter {

	/**
	 * Dao\Paginator\Adapter\NativeArray
	 *
	 * Pagination using a PHP array as source of data
	 *
	 *<code>
	 *	$paginator = new \Dao\Paginator\Adapter\Model(
	 *		array(
	 *			"data"  => array(
	 *				array('id' => 1, 'name' => 'Artichoke'),
	 *				array('id' => 2, 'name' => 'Carrots'),
	 *				array('id' => 3, 'name' => 'Beet'),
	 *				array('id' => 4, 'name' => 'Lettuce'),
	 *				array('id' => 5, 'name' => '')
	 *			),
	 *			"limit" => 2,
	 *			"page"  => $currentPage
	 *		)
	 *	);
	 *</code>
	 *
	 */
	
	class NativeArray extends \Dao\Paginator\Adapter implements \Dao\Paginator\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_data;

		/**
		 * \Dao\Paginator\Adapter\NativeArray constructor
		 *
		 * @param array $config
		 */
		public function __construct($config){ }


		/**
		 * Returns a slice of the resultset to show in the pagination
		 *
		 * @return \stdClass
		 */
		public function getPaginate(){ }

	}
}
