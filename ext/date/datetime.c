/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "date/datetime.h"
#include "date.h"

#include <ext/date/php_date.h>

#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/output.h"


/**
 * Dao\Date\DateTime
 *
 *<code>
 * $datetime = new \Dao\Date\DateTime();
 * $start = $datetime->startOfDay();
 * $end = $datetime->endOfDay();
 *</code>
 */
zend_class_entry *dao_date_datetime_ce;

PHP_METHOD(Dao_Date_DateTime, __construct);
PHP_METHOD(Dao_Date_DateTime, setDateTime);
PHP_METHOD(Dao_Date_DateTime, startOfDay);
PHP_METHOD(Dao_Date_DateTime, endOfDay);
PHP_METHOD(Dao_Date_DateTime, startOfMonth);
PHP_METHOD(Dao_Date_DateTime, endOfMonth);
PHP_METHOD(Dao_Date_DateTime, startOfQuarter);
PHP_METHOD(Dao_Date_DateTime, endOfQuarter);
PHP_METHOD(Dao_Date_DateTime, startOfYear);
PHP_METHOD(Dao_Date_DateTime, endOfYear);
PHP_METHOD(Dao_Date_DateTime, startOfDecade);
PHP_METHOD(Dao_Date_DateTime, endOfDecade);
PHP_METHOD(Dao_Date_DateTime, startOfCentury);
PHP_METHOD(Dao_Date_DateTime, endOfCentury);
PHP_METHOD(Dao_Date_DateTime, modifyYear);
PHP_METHOD(Dao_Date_DateTime, modifyQuarter);
PHP_METHOD(Dao_Date_DateTime, modifyMonth);
PHP_METHOD(Dao_Date_DateTime, modifyDay);
PHP_METHOD(Dao_Date_DateTime, modifyHour);
PHP_METHOD(Dao_Date_DateTime, modifyMinute);
PHP_METHOD(Dao_Date_DateTime, modifySecond);
PHP_METHOD(Dao_Date_DateTime, __get);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, font)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_setdatetime, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, year, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, month, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, day, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, hour, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, minute, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, second, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifyyear, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, year, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifyquarter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, quarter, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifymonth, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, month, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifyday, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, day, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifyhour, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, hour, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifyminute, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, minute, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_date_datetime_modifysecond, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, second, IS_LONG, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_date_datetime_method_entry[] = {
	PHP_ME(Dao_Date_DateTime, __construct, arginfo_dao_date_datetime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Date_DateTime, setDateTime, arginfo_dao_date_datetime_setdatetime, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfDay, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfDay, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfMonth, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfMonth, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfQuarter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfQuarter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfYear, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfYear, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfDecade, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfDecade, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, startOfCentury, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, endOfCentury, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyYear, arginfo_dao_date_datetime_modifyyear, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyQuarter, arginfo_dao_date_datetime_modifyquarter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyMonth, arginfo_dao_date_datetime_modifymonth, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyDay, arginfo_dao_date_datetime_modifyday, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyHour, arginfo_dao_date_datetime_modifyhour, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifyMinute, arginfo_dao_date_datetime_modifyminute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, modifySecond, arginfo_dao_date_datetime_modifysecond, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Date_DateTime, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Date\DateTime initializer
 */
DAO_INIT_CLASS(Dao_Date_DateTime){

	zend_class_entry *datetime_ce;

	datetime_ce = php_date_get_date_ce();

	DAO_REGISTER_CLASS_EX(Dao\\Date, DateTime, date_datetime, datetime_ce, dao_date_datetime_method_entry, 0);

	return SUCCESS;
}

/**
 * Create a new instance
 *
 * @param string|null $time
 * @param \DateTimeZone|string|null $tz
 */
PHP_METHOD(Dao_Date_DateTime, __construct){

	zval *time = NULL, *tz = NULL, timezone = {};

	dao_fetch_params(0, 0, 2, &time, &tz);

	if (!time) {
		time = &DAO_GLOBAL(z_null);
	}

	if (!tz) {
		tz = &DAO_GLOBAL(z_null);
	}

	DAO_CALL_CE_STATIC(&timezone, dao_date_ce, "createdatetimezone", tz);
	DAO_CALL_PARENT(NULL, dao_date_datetime_ce, getThis(), "__construct", time, &timezone);
}

/**
 * Set the date and time all together
 *
 * @param int $year
 * @param int $month
 * @param int $day
 * @param int $hour
 * @param int $minute
 * @param int $second
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, setDateTime){

	zval *year, *month, *day, *hour, *minute, *second = NULL, datetime = {};

	dao_fetch_params(0, 5, 1, &year, &month, &day, &hour, &minute, &second);

	if (!second) {
		second = &DAO_GLOBAL(z_zero);
	}

	DAO_CALL_METHOD(&datetime, getThis(), "setdate", year, month, day);
	DAO_CALL_METHOD(return_value, &datetime, "settime", hour, minute, second);
}

/**
 * Resets the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfDay){

	DAO_CALL_METHOD(return_value, getThis(), "settime", &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfDay){

	zval hour = {}, minute = {}, second = {};

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "settime", &hour, &minute, &second);
}

/**
 * Resets the date to the first day of the month and the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfMonth){

	zval p = {}, year = {}, month = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_STRING(&p, "month");
	DAO_CALL_METHOD(&month, getThis(), "__get", &p);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &month, &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the date to end of the month and time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfMonth){

	zval p = {}, year = {}, month = {}, daysinmonth = {}, hour = {}, minute = {}, second = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_STRING(&p, "month");
	DAO_CALL_METHOD(&month, getThis(), "__get", &p);

	ZVAL_STRING(&p, "daysInMonth");
	DAO_CALL_METHOD(&daysinmonth, getThis(), "__get", &p);

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &month, &daysinmonth, &hour, &minute, &second);
}

/**
 * Resets the date to the first day of the quarter and the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfQuarter){

	zval p = {}, year = {}, quarter = {}, month = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_STRING(&p, "quarter");
	DAO_CALL_METHOD(&quarter, getThis(), "__get", &p);

	ZVAL_LONG(&month, ((Z_LVAL(quarter) - 1) * DAO_MONTHS_PER_QUARTER + 1));

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &month, &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the date to end of the quarter and time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfQuarter){

	zval p = {}, year = {}, quarter = {}, month = {}, daysinmonth = {}, hour = {}, minute = {}, second = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_STRING(&p, "quarter");
	DAO_CALL_METHOD(&quarter, getThis(), "__get", &p);

	ZVAL_LONG(&month, (Z_LVAL(quarter) * DAO_MONTHS_PER_QUARTER));

	ZVAL_STRING(&p, "daysInMonth");
	DAO_CALL_METHOD(&daysinmonth, getThis(), "__get", &p);

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &month, &daysinmonth, &hour, &minute, &second);
}

/**
 * Resets the date to the first day of the year and the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfYear){

	zval p = {}, year = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the date to end of the year and time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfYear){

	zval p = {}, year = {}, month = {}, day = {}, hour = {}, minute = {}, second = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_LONG(&month, 12);
	ZVAL_LONG(&day, 31);

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &year, &month, &day, &hour, &minute, &second);
}

/**
 * Resets the date to the first day of the year and the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfDecade){

	zval p = {}, year = {}, decade = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_LONG(&decade, (Z_LVAL(year) - (Z_LVAL(year) - 1) % DAO_YEARS_PER_DECADE));

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &decade, &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the date to end of the decade and time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfDecade){

	zval p = {}, year = {}, decade = {}, month = {}, day = {}, hour = {}, minute = {}, second = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_LONG(&decade, (Z_LVAL(year) - (Z_LVAL(year) - 1) % DAO_YEARS_PER_DECADE + DAO_YEARS_PER_DECADE - 1));

	ZVAL_LONG(&month, 12);
	ZVAL_LONG(&day, 31);

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &decade, &month, &day, &hour, &minute, &second);
}

/**
 * Resets the date to the first day of the century and the time to 00:00:00
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, startOfCentury){

	zval p = {}, year = {}, century = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_LONG(&century, (Z_LVAL(year) - (Z_LVAL(year) - 1) % DAO_YEARS_PER_CENTURY));

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &century, &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_one), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero), &DAO_GLOBAL(z_zero));
}

/**
 * Resets the date to end of the century and time to 23:59:59
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, endOfCentury){

	zval p = {}, year = {}, century = {}, month = {}, day = {}, hour = {}, minute = {}, second = {};

	ZVAL_STRING(&p, "year");
	DAO_CALL_METHOD(&year, getThis(), "__get", &p);

	ZVAL_LONG(&century, (Z_LVAL(year) - (Z_LVAL(year) - 1) % DAO_YEARS_PER_CENTURY + DAO_YEARS_PER_CENTURY - 1));

	ZVAL_LONG(&month, 12);
	ZVAL_LONG(&day, 31);

	ZVAL_LONG(&hour, 23);
	ZVAL_LONG(&minute, 59);
	ZVAL_LONG(&second, 59);

	DAO_CALL_METHOD(return_value, getThis(), "setdatetime", &century, &month, &day, &hour, &minute, &second);
}

/**
 * Add or Remove years from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyYear){

	zval *year, v = {};

	dao_fetch_params(0, 1, 0, &year);

	DAO_CONCAT_VS(&v, year, " year");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove quarters from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyQuarter){

	zval *quarter, month = {}, v = {};

	dao_fetch_params(0, 1, 0, &quarter);

	ZVAL_LONG(&month, Z_LVAL_P(quarter) * DAO_MONTHS_PER_QUARTER);
	DAO_CONCAT_VS(&v, &month, " month");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove months from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyMonth){

	zval *month, v = {};

	dao_fetch_params(0, 1, 0, &month);

	DAO_CONCAT_VS(&v, month, " month");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove days from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyDay){

	zval *day, v = {};

	dao_fetch_params(0, 1, 0, &day);

	DAO_CONCAT_VS(&v, day, " day");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove hours from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyHour){

	zval *hour, v = {};

	dao_fetch_params(0, 1, 0, &hour);

	DAO_CONCAT_VS(&v, hour, " hour");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove minutes from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifyMinute){

	zval *minute, v = {};

	dao_fetch_params(0, 1, 0, &minute);

	DAO_CONCAT_VS(&v, minute, " minute");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Add or Remove seconds from the instance
 *
 * @return \DateTime
 */
PHP_METHOD(Dao_Date_DateTime, modifySecond){

	zval *second, v = {};

	dao_fetch_params(0, 1, 0, &second);

	DAO_CONCAT_VS(&v, second, " second");

	DAO_CALL_METHOD(NULL, getThis(), "modify", &v);

	RETURN_THIS();
}

/**
 * Get a part of the Carbon object
 *
 * @param string $name
 *
 * @throws \InvalidArgumentException
 *
 * @return string|int|\DateTimeZone
 */
PHP_METHOD(Dao_Date_DateTime, __get){

	zval *property, lower_property = {}, format = {}, p = {}, v = {};

	dao_fetch_params(0, 1, 0, &property);
	DAO_ENSURE_IS_STRING(property);

	dao_fast_strtolower(&lower_property, property);

	if (DAO_IS_STRING(&lower_property, "year")) {
		ZVAL_STRING(&format, "Y");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "month")) {
		ZVAL_STRING(&format, "n");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "day")) {
		ZVAL_STRING(&format, "i");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "hour")) {
		ZVAL_STRING(&format, "G");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "minute")) {
		ZVAL_STRING(&format, "i");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "second")) {
		ZVAL_STRING(&format, "s");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "micro")) {
		ZVAL_STRING(&format, "u");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "dayofweek")) {
		ZVAL_STRING(&format, "w");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "dayofyear")) {
		ZVAL_STRING(&format, "z");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "weekofyear")) {
		ZVAL_STRING(&format, "W");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "daysinmonth")) {
		ZVAL_STRING(&format, "t");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "timestamp")) {
		ZVAL_STRING(&format, "U");
		DAO_CALL_METHOD(return_value, getThis(), "format", &format);
		convert_to_long(return_value);
		return;
	} else if (DAO_IS_STRING(&lower_property, "weekofmonth")) {
		ZVAL_STRING(&p, "day");
		DAO_CALL_METHOD(&v, getThis(), "__get", &p);
		RETURN_LONG(ceil((float)Z_LVAL(v) / DAO_DAYS_PER_WEEK));
	} else if (DAO_IS_STRING(&lower_property, "quarter")) {
		ZVAL_STRING(&p, "month");
		DAO_CALL_METHOD(&v, getThis(), "__get", &p);
		RETURN_LONG(ceil((float)Z_LVAL(v) / DAO_MONTHS_PER_QUARTER));
	} else if (DAO_IS_STRING(&lower_property, "offset")) {
		DAO_CALL_METHOD(return_value, getThis(), "getoffset");
	} else if (DAO_IS_STRING(&lower_property, "offsethours")) {
		DAO_CALL_METHOD(&v, getThis(), "getoffset");
		RETURN_LONG(Z_LVAL(v) / DAO_SECONDS_PER_MINUTE / DAO_MINUTES_PER_HOUR);
	} else if (DAO_IS_STRING(&lower_property, "dst")) {
		ZVAL_STRING(&format, "I");
		DAO_CALL_METHOD(&v, getThis(), "format", &format);
		convert_to_long(&v);
		RETURN_BOOL(Z_LVAL(v));
	}
	DAO_THROW_EXCEPTION_FORMAT(spl_ce_InvalidArgumentException, "Unknown getter '%s'", Z_STRVAL_P(property));
	return;
}
