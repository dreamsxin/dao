<?php 

namespace Dao {

	/**
	 * Dao\Text
	 *
	 * Provides utilities to work with texts
	 */
	
	abstract class Text {

		const RANDOM_ALNUM = 0;

		const RANDOM_ALPHA = 1;

		const RANDOM_HEXDEC = 2;

		const RANDOM_NUMERIC = 3;

		const RANDOM_NOZERO = 4;

		/**
		 * Converts strings to camelize style
		 *
		 *<code>
		 *	echo \Dao\Text::camelize('coco_bongo'); //CocoBongo
		 *</code>
		 *
		 * @param string $str
		 * @return string
		 */
		public static function camelize($str){ }


		/**
		 * Uncamelize strings which are camelized
		 *
		 *<code>
		 *	echo \Dao\Text::uncamelize('CocoBongo'); //coco_bongo
		 *</code>
		 *
		 * @param string $str
		 * @return string
		 */
		public static function uncamelize($str){ }


		/**
		 * Adds a number to a string or increment that number if it already is defined
		 *
		 *<code>
		 *	echo \Dao\Text::increment("a"); // "a_1"
		 *	echo \Dao\Text::increment("a_1"); // "a_2"
		 *</code>
		 *
		 * @param string $str
		 * @param string|int $separator
		 * @return string
		 */
		public static function increment($str, $separator=null){ }


		/**
		 * Adds a number to a string or decrement that number if it already is defined
		 *
		 *<code>
		 *	echo \Dao\Text::decrement("a"); // "a_1"
		 *	echo \Dao\Text::decrement("a_1"); // "a_0"
		 *</code>
		 *
		 * @param string $str
		 * @param string|int $separator
		 * @return string
		 */
		public static function decrement($str, $separator=null){ }


		/**
		 * Generates a random string based on the given type. Type is one of the RANDOM_* constants
		 *
		 *<code>
		 *	echo \Dao\Text::random(Dao\Text::RANDOM_ALNUM); //"aloiwkqz"
		 *</code>
		 *
		 * @param int $type
		 * @param int $length
		 * @return string
		 */
		public static function random($type, $length=null){ }


		/**
		 * Check if a string starts with a given string
		 *
		 *<code>
		 *	echo \Dao\Text::startsWith("Hello", "He"); // true
		 *	echo \Dao\Text::startsWith("Hello", "he"); // false
		 *	echo \Dao\Text::startsWith("Hello", "he", false); // true
		 *</code>
		 *
		 * @param string $str
		 * @param string $start
		 * @param boolean $ignoreCase
		 * @return boolean
		 */
		public static function startsWith($str, $start, $ignoreCase=null){ }


		/**
		 * Check if a string ends with a given string
		 *
		 *<code>
		 *	echo \Dao\Text::endsWith("Hello", "llo"); // true
		 *	echo \Dao\Text::endsWith("Hello", "LLO"); // false
		 *	echo \Dao\Text::endsWith("Hello", "LLO", false); // true
		 *</code>
		 *
		 * @param string $str
		 * @param string $end
		 * @param boolean $ignoreCase
		 * @return boolean
		 */
		public static function endsWith($str, $end, $ignoreCase=null){ }


		/**
		 * Lowercases a string, this function makes use of the mbstring extension if available
		 *
		 * @param string $str
		 * @return string
		 */
		public static function lower($str){ }


		/**
		 * Uppercases a string, this function makes use of the mbstring extension if available
		 *
		 * @param string $str
		 * @return string
		 */
		public static function upper($str){ }


		/**
		 * Returns human readable sizes
		 *
		 * @param int $size
		 * @param string $forceUnit
		 * @param string $format
		 * @param boolean $si
		 * @return string
		 */
		public static function bytes($size, $forceUnit=null, $format=null, $si=null){ }


		/**
		 * Reduces multiple slashes in a string to single slashes
		 *
		 * <code>
		 *    echo \Dao\Text::reduceSlashes("foo//bar/baz"); // foo/bar/baz
		 *    echo \Dao\Text::reduceSlashes("http://foo.bar///baz/buz"); // http://foo.bar/baz/buz
		 * </code>
		 */
		public static function reduceSlashes($str){ }


		/**
		 * Concatenates strings using the separator only once without duplication in places concatenation
		 *
		 * <code>
		 *    $str = \Dao\Text::concat("/", "/tmp/", "/folder_1/", "/folder_2", "folder_3/");
		 *    echo $str; // /tmp/folder_1/folder_2/folder_3/
		 * </code>
		 *
		 * @param string separator
		 * @param string a
		 * @param string b
		 * @param string ...N
		 */
		public static function concat($separator, $strA, $strB){ }


		/**
		 * Makes a phrase underscored instead of spaced
		 *
		 * <code>
		 *   echo \Dao\Text::underscore('look behind'); // 'look_behind'
		 *   echo \Dao\Text::underscore('Awesome \Dao'); // 'Awesome_Dao'
		 * </code>
		 */
		public static function underscore($str){ }


		/**
		 * Makes an underscored or dashed phrase human-readable
		 *
		 * <code>
		 *   echo \Dao\Text::humanize('start-a-horse'); // 'start a horse'
		 *   echo \Dao\Text::humanize('five_cats'); // 'five cats'
		 * </code>
		 */
		public static function humanize($str){ }


		/**
		 * Limits a phrase to a given number of characters.
		 *
		 * <code>
		 *   $text = \Dao\Text::limitChars($text, 30);
		 * </code>
		 *
		 * @param string $str
		 * @param int $limit
		 * @param string $end
		 * @return string
		 */
		public static function limitChars($str, $limit, $end=null){ }

	}
}
