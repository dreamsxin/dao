<?php 

namespace Dao {

	/**
	 * Dao\Date
	 *
	 * Provides utilities to work with dates
	 */
	
	abstract class Date {

		const YEAR = 31556926;

		const MONTH = 2629744;

		const WEEK = 604800;

		const DAY = 86400;

		const HOUR = 3600;

		const MINUTE = 60;

		const MONTHS_LONG = %B;

		const MONTHS_SHORT = %b;

		public static $timestamp_format;

		public static $timezone;

		protected static $_months;

		/**
		 * Returns the offset (in seconds) between two time zones. Use this to
		 * display dates to users in different time zones.
		 *
		 *     $seconds = \Dao\Date::offset('America/Chicago', 'GMT');
		 *
		 * @param string $remote
		 * @param string $local
		 * @param mixed $date
		 * @return int
		 */
		public static function offset($remote, $local=null, $now=null){ }


		/**
		 * Number of seconds in a minute, incrementing by a step. Typically used as
		 * a shortcut for generating a list that can used in a form.
		 *
		 *     $seconds = \Dao\Date::seconds(); // 00, 01, 02, 03, ..., 58, 59
		 *
		 * @param int $step
		 * @param int $start
		 * @param int $end
		 * @return array
		 */
		public static function seconds($step=null, $start=null, $end=null){ }


		/**
		 * Number of minutes in an hour, incrementing by a step. Typically used as
		 * a shortcut for generating a list that can be used in a form.
		 *
		 *     $minutes = \Dao\Date::minutes(); // 00, 05, 10, 15, ..., 50, 55
		 *
		 * @param int $step
		 * @return array
		 */
		public static function minutes($step=null){ }


		/**
		 * Number of hours in a day. Typically used as a shortcut for generating a
		 * list that can be used in a form.
		 *
		 *     $hours = \Dao\Date::hours(); // 01, 02, 03, ..., 10, 11, 12
		 *
		 * @param int $step
		 * @param boolean $is_long
		 * @param int $start
		 * @return array
		 */
		public static function hours($step=null, $long=null, $start=null){ }


		/**
		 * Adjusts a non-24-hour number into a 24-hour number.
		 *
		 *     $hour = \Dao\Date::adjust(3, 'pm'); // 15
		 *
		 * @param int $hour
		 * @param string $ampm
		 * @return string
		 */
		public static function adjust($hour, $ampm){ }


		/**
		 * Returns AM or PM, based on a given hour (in 24 hour format).
		 *
		 *     $type = \Dao\Date::ampm(12); // PM
		 *     $type = \Dao\Date::ampm(1);  // AM
		 *
		 * @param int $hour
		 * @return string
		 */
		public static function ampm($hour){ }


		/**
		 * Number of days in a given month and year. Typically used as a shortcut
		 * for generating a list that can be used in a form.
		 *
		 *     \Dao\Date::days(4, 2010); // 1, 2, 3, ..., 28, 29, 30
		 *
		 * @param int $month
		 * @param int $year
		 * @return array
		 */
		public static function days($month, $year=null){ }


		/**
		 * Number of months in a year. Typically used as a shortcut for generating
		 * a list that can be used in a form.
		 *
		 * By default a mirrored array of $month_number => $month_number is returned
		 *
		 *     \Dao\Date::months();
		 *     // aray(1 => 1, 2 => 2, 3 => 3, ..., 12 => 12)
		 *
		 * But you can customise this by passing in either DAO_DATE_MONTHS_LONG
		 *
		 *     \Dao\Date::months(Dao\Utils\DAO_DATE_MONTHS_LONG);
		 *     // array(1 => 'January', 2 => 'February', ..., 12 => 'December')
		 *
		 * Or DAO_DATE_MONTHS_SHORT
		 *
		 *     \Dao\Date::months(Dao\Date::DAO_DATE_MONTHS_SHORT);
		 *     // array(1 => 'Jan', 2 => 'Feb', ..., 12 => 'Dec')
		 *
		 * @uses DAO_DATE_hours
		 * @param string $format
		 * @return array
		 */
		public static function months($format=null){ }


		/**
		 * Returns an array of years between a starting and ending year. By default,
		 * the the current year - 5 and current year + 5 will be used. Typically used
		 * as a shortcut for generating a list that can be used in a form.
		 *
		 *     $years = \Dao\Date::years(2000, 2010); // 2000, 2001, ..., 2009, 2010
		 *
		 * @param int $start
		 * @param int $end
		 * @return array
		 */
		public static function years($start=null, $end=null){ }


		/**
		 * Returns time difference between two timestamps, in human readable format.
		 * If the second timestamp is not given, the current time will be used.
		 * Also consider using [Dao\Date::fuzzy_span] when displaying a span.
		 *
		 *     $span = \Dao\Date::span(60, 182, 'minutes,seconds'); // array('minutes' => 2, 'seconds' => 2)
		 *     $span = \Dao\Date::span(60, 182, 'minutes'); // 2
		 *
		 * @param int $remote
		 * @param int $local
		 * @param string $output
		 * @return string/array
		 */
		public static function span($remote, $local=null, $output=null){ }


		/**
		 * Returns time, in human readable format.
		 * Also consider using [Dao\Date::fuzzy_span2] when displaying a span.
		 *
		 *     $span = \Dao\Date::span(182, 'minutes,seconds'); // array('minutes' => 3, 'seconds' => 2)
		 *     $span = \Dao\Date::span(182, 'minutes'); // 3
		 *
		 * @param int $time
		 * @param string $output
		 * @return string/array
		 */
		public static function span2($remote, $output=null){ }


		/**
		 * Returns the difference between a time and now in a "fuzzy" way.
		 * Displaying a fuzzy time instead of a date is usually faster to read and understand.
		 *
		 *     $span = \Dao\Date::fuzzy_span(time() - 10); // "moments ago"
		 *     $span = \Dao\Date::fuzzy_span(time() + 20); // "in moments"
		 *
		 * A second parameter is available to manually set the "local" timestamp,
		 * however this parameter shouldn't be needed in normal usage and is only
		 * included for unit tests
		 *
		 * @param int $timestamp
		 * @param int $local_timestamp
		 * @return string
		 */
		public static function fuzzy_span($timestamp, $local_timestamp=null){ }


		/**
		 * Returns the time in a "fuzzy" way.
		 * Displaying a fuzzy time instead of a date is usually faster to read and understand.
		 *
		 *     $span = \Dao\Date::fuzzy_span2(60); // "1 minutes"
		 *     $span = \Dao\Date::fuzzy_span2(10); // "10 seconds"
		 *
		 * A second parameter is available to manually set the "local" timestamp,
		 * however this parameter shouldn't be needed in normal usage and is only
		 * included for unit tests
		 *
		 * @param int $timestamp
		 * @param string $output
		 * @return string
		 */
		public static function fuzzy_span2($timestamp, $output=null){ }


		/**
		 * Converts a UNIX timestamp to DOS format. There are very few cases where
		 * this is needed, but some binary formats use it (eg: zip files.)
		 * Converting the other direction is done using {@link DAO_DATE_dos2unix}.
		 *
		 *     $dos = \Dao\Date::unix2dos($unix);
		 *
		 * @param int $timestamp
		 * @return int
		 */
		public static function unix2dos($timestamp=null){ }


		/**
		 * Converts a DOS timestamp to UNIX format.There are very few cases where
		 * this is needed, but some binary formats use it (eg: zip files.)
		 *
		 *     $unix = \Dao\Date::dos2unix($dos);
		 *
		 * @param int $timestamp
		 * @return int
		 */
		public static function dos2unix($timestamp=null){ }


		/**
		 * Returns a date/time string with the specified timestamp format
		 *
		 *     $time = \Dao\Date::formatted_time('5 minutes ago');
		 *
		 * @param string $datetime_str
		 * @param string $timestamp_format
		 * @param string $timezone
		 * @return string
		 */
		public static function formatted_time($datetime_str=null, $timestamp_format=null, $timezone=null){ }


		/**
		 * @param \DateInterval $interval
		 * @return int|string Returns usually integer, but string if result is too big (> PHP_INT_MAX)
		 */
		public static function intervalToSeconds(\DateInterval $interval){ }


		/**
		 * @param \DateTimeZone|string|int|null $zone
		 * @return int|string Returns usually integer, but string if result is too big (> PHP_INT_MAX)
		 */
		public static function createDateTimeZone($timezone=null){ }


		/**
		 *  Filters a variable
		 *
		 *     $ret = \Dao\Date::filter('2012-01-22');
		 *     $ret = \Dao\Date::filter('2012-01-22 11:00:00', 'Y-m-d H:i:s');
		 *
		 * @param string $date_str
		 * @param string $date_format
		 * @param string $delimiter
		 * @return string|array
		 */
		public static function filter($date, $format=null, $delimiter=null){ }


		/**
		 * Checks if a value is a valid date
		 *
		 *     $ret = \Dao\Date::valid('2012-01-22');
		 *     $ret = \Dao\Date::valid('2012-01-22 11:00:00', 'Y-m-d H:i:s');
		 *
		 * @param string $date_str
		 * @param string $date_format
		 * @return boolean
		 */
		public static function valid($date, $format=null){ }


		/**
		 * Gets two date difference.
		 *
		 *     $ret = \Dao\Date::diff('2012-01-22', '2018-05-10');
		 *
		 * @param string $date1
		 * @param string $date2
		 * @param string $differenceFormat
		 * @param string $format
		 * @return int
		 */
		public static function diff($date1, $date2, $diffFormat=null, $format=null){ }


		/**
		 * Gets two date difference.
		 *
		 *     $ret = \Dao\Date::add('2012-01-22', '1 days');
		 *
		 * @param string $date1
		 * @param string $interval
		 * @param string $format
		 * @return string
		 */
		public static function add($date1, $interval=null, $format=null){ }

	}
}
