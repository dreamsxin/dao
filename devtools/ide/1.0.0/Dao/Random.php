<?php 

namespace Dao {

	/**
	 * Dao\Random
	 *
	 *<code>
	 *  $random = new \Dao\Random();
	 *
	 *  // Random alnum string
	 *  $alnum = $random->alnum();
	 */
	
	class Random {

		const COLOR_RGB = 0;

		const COLOR_RGBA = 1;

		/**
		 * Generates a random alnum string
		 *
		 *<code>
		 *  $random = new \Dao\Random();
		 *  $alnum = $random->alnum();
		 *</code>
		 */
		public static function alnum($len=null){ }


		/**
		 * Generates a random alpha string
		 *
		 *<code>
		 *  $random = new \Dao\Random();
		 *  $alpha = $random->alpha();
		 *</code>
		 */
		public static function alpha($len=null){ }


		/**
		 * Generates a random hexdec string
		 *
		 *<code>
		 *  $random = new \Dao\Random();
		 *  $hexdec = $random->hexdec();
		 *</code>
		 */
		public static function hexdec($len=null){ }


		/**
		 * Generates a random numeric string
		 *
		 *<code>
		 *  $random = new \Dao\Random();
		 *  $numeric = $random->numeric();
		 *</code>
		 */
		public static function numeric($len=null){ }


		/**
		 * Generates a random nozero numeric string
		 *
		 *<code>
		 *  $random = new \Dao\Random();
		 *  $bytes = $random->nozero();
		 *</code>
		 */
		public static function nozero($len=null){ }


		/**
		 * Generates a random color
		 */
		public static function color($type=null){ }

	}
}
