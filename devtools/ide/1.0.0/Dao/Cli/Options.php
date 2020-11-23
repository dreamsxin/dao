<?php 

namespace Dao\Cli {

	/**
	 * Dao\Cli\Options
	 *
	 *<code>
	 *
	 * $ops = new \Dao\Cli\Options('Dao CLI');
	 *  $ops->add([
	 * 	'type' => \Dao\Cli\Options::TYPE_INT,
	 * 	'name' => 'min'
	 * ]);
	 *  $ops->add([
	 * 	'type' => \Dao\Cli\Options::TYPE_INT,
	 * 	'name' => 'max',
	 * 	'shortName' => 'm',
	 * 	'required' => false,
	 * 	'desc' => "int",
	 * 	'help' => "must be int",
	 * 	'defaultValue' => 1
	 * ]);
	 * $ops->add(\Dao\Cli\Options::TYPE_STRING, 'name', 'n', true, "name", "must be string", "Dao");
	 * $values = $ops->parse();
	 *
	 *</code>
	 */
	
	class Options extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface {

		const TYPE_ANY = 0;

		const TYPE_INT = 1;

		const TYPE_FLOAT = 2;

		const TYPE_BOOLEAN = 3;

		const TYPE_STRING = 4;

		const TYPE_ARRAY = 5;

		protected $_title;

		protected $_program;

		protected $_argString;

		protected $_desc;

		protected $_types;

		protected $_options;

		protected $_longopts;

		protected $_descs;

		protected $_helps;

		protected $_required;

		protected $_defaultValues;

		protected $_names;

		protected $_shortNames;

		/**
		 * \Dao\Cli\Options constructor
		 */
		public function __construct($title=null, $program=null, $argString=null, $desc=null, $options=null, $dependencyInjector=null){ }


		/**
		 * Add option
		 */
		public function add($arg, $name=null, $shortname=null, $required=null, $desc=null, $help=null, $defaultValue=null){ }


		/**
		 * Print help
		 */
		public function help(){ }


		/**
		 * Parse and return values
		 */
		public function parse($optind=null){ }


		public function addOption($arg, $name=null, $shortname=null, $required=null, $desc=null, $help=null, $defaultValue=null){ }

	}
}
