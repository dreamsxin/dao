<?php 

namespace Dao\Annotations {

	/**
	 * Dao\Annotations\Annotation
	 *
	 * Represents a single annotation in an annotations collection
	 */
	
	class Annotation {

		protected $_name;

		protected $_arguments;

		protected $_exprArguments;

		/**
		 * \Dao\Annotations\Annotation constructor
		 *
		 * @param array $reflectionData
		 */
		public function __construct($reflectionData){ }


		/**
		 * Returns the annotation's name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Resolves an annotation expression
		 *
		 * @param array $expr
		 * @return mixed
		 */
		public function getExpression($expr){ }


		/**
		 * Returns the expression arguments without resolving
		 *
		 * @return array
		 */
		public function getExprArguments(){ }


		/**
		 * Returns the expression arguments
		 *
		 * @return array
		 */
		public function getArguments(){ }


		/**
		 * Returns the number of arguments that the annotation has
		 *
		 * @return int
		 */
		public function numberArguments(){ }


		/**
		 * Returns an argument in a specific position
		 *
		 * @return mixed
		 */
		public function getArgument($position){ }


		/**
		 * Checks if the annotation has a specific argument
		 *
		 * @return boolean
		 */
		public function hasArgument($position){ }


		public function getNamedArgument($position){ }


		public function getNamedParameter($position){ }


		public function hasNamedArgument($position){ }

	}
}
