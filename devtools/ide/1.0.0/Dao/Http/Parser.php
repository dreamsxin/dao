<?php 

namespace Dao\Http {

	/**
	 * Dao\Http\Parser
	 *
	 *<code>
	 *	$parser = new Dao\Http\Parser(Dao\Http\Parser::TYPE_BOTH);
	 *  $result = $parser->execute($body);
	 *</code>
	 */
	
	class Parser {

		const TYPE_REQUEST = 0;

		const HTTP_RESPONSE = 1;

		const TYPE_BOTH = 2;

		const STATUS_NONE = 0;

		const STATUS_BEGIN = 1;

		const STATUS_URL = 2;

		const STATUS_STATUS = 3;

		const STATUS_FIELD = 4;

		const STATUS_VALUE = 5;

		const STATUS_HEADER_END = 6;

		const STATUS_BODY = 7;

		const STATUS_CHUNK = 8;

		const STATUS_END = 9;

		protected $_type;

		/**
		 * \Dao\Http\Parser constructor
		 *
		 * @param int $type
		 */
		public function __construct($type=null){ }


		/**
		 * Gets parse status
		 *
		 * @return int
		 */
		public function status(){ }


		/**
		 * parse
		 *
		 * @param string $body http message.
		 * @return array|boolean $result
		 */
		public function execute($body, $output=null){ }


		/**
		 * parse cookies
		 *
		 * @param string $str
		 * @return array
		 */
		public function parseCookie($str){ }

	}
}
