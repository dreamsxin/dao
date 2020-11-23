<?php 

namespace Dao {

	/**
	 * Dao\Xhprof
	 *
	 * Provides xhprof capabilities to Dao applications
	 */
	
	class Xhprof {

		const FLAG_MEMORY = 6;

		const FLAG_MEMORY_MU = 2;

		const FLAG_MEMORY_PMU = 4;

		const FLAG_CPU = 1;

		const FLAG_NO_BUILTINS = 8;

		const FLAG_MEMORY_ALLOC = 16;

		const FLAG_MEMORY_ALLOC_AS_MU = 48;

		/**
		 * Enable xhprof
		 *
		 * @param \Dao\Logger\AdapterInterface $logger
		 */
		public static function enable($flags){ }


		/**
		 * Disable xhprof
		 *
		 */
		public static function disable(){ }

	}
}
