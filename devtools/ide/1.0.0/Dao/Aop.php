<?php 

namespace Dao {

	/**
	 * Dao\Aop
	 *
	 * AOP aims to allow separation of cross-cutting concerns (cache, log, security, transactions, ...)
	 */
	
	class Aop {

		const KIND_BEFORE = 2;

		const KIND_AFTER = 4;

		const KIND_AROUND = 1;

		const KIND_PROPERTY = 32;

		const KIND_FUNCTION = 128;

		const KIND_METHOD = 64;

		const KIND_READ = 8;

		const KIND_WRITE = 16;

		const KIND_AROUND_WRITE_PROPERTY = 49;

		const KIND_AROUND_READ_PROPERTY = 41;

		const KIND_BEFORE_WRITE_PROPERTY = 50;

		const KIND_BEFORE_READ_PROPERTY = 42;

		const KIND_AFTER_WRITE_PROPERTY = 52;

		const KIND_AFTER_READ_PROPERTY = 44;

		const KIND_BEFORE_METHOD = 66;

		const KIND_AFTER_METHOD = 68;

		const KIND_AROUND_METHOD = 65;

		const KIND_BEFORE_FUNCTION = 130;

		const KIND_AFTER_FUNCTION = 132;

		const KIND_AROUND_FUNCTION = 129;

		/**
		 * Adds an advice to be run before the matching join points
		 *
		 *<code>
		 *	\Dao\Aop::addBefore('mytest::test()', function(Dao\Aop\Joinpoint $obj){
		 *		echo "before";
		 *	})
		 *</code>
		 *
		 * @param string $selector
		 * @param callable $handler
		 */
		public static function addBefore($pointcut, $advice){ }


		/**
		 * Adds an advice to be run after the matching join points
		 *
		 *<code>
		 *	\Dao\Aop::addAfter('mytest::test()', function(Dao\Aop\Joinpoint $obj){
		 *		echo "after";
		 *	})
		 *</code>
		 *
		 * @param string $selector
		 * @param callable $handler
		 */
		public static function addAfter($pointcut, $advice){ }


		/**
		 * Links advices that becomes active after the target normally returns from execution (no exception).
		 *
		 *<code>
		 *	\Dao\Aop::addAfterReturning('mytest::test()', function(Dao\Aop\Joinpoint $obj){
		 *		if ($obj->getReturnedValue() === null) {
		 *			echo "I'm updating {$obj->getMethodName()} in {$obj->getClassName()}, now returning this";
		 *			$obj->setReturnedValue($obj->getObject());
		 *		}
		 *	})
		 *</code>
		 *
		 * @param string $selector
		 * @param callable $handler
		 */
		public static function addAfterReturning($pointcut, $advice){ }


		/**
		 * Links advices that becomes active if the target raise an (uncaught) exception.
		 *
		 * @param string $selector
		 * @param callable $handler
		 */
		public static function addAfterThrowing($pointcut, $advice){ }


		/**
		 * Adds an advice to be run around the matching join points
		 *
		 *<code>
		 *	\Dao\Aop::addAround('mytest::test()', function(Dao\Aop\Joinpoint $obj){
		 *		return $obj->process();
		 *	})
		 *</code>
		 *
		 * @param string $selector
		 * @param callable $handler
		 */
		public static function addAround($pointcut, $advice){ }

	}
}
