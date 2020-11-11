
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
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

#ifndef DAO_DATE_H
#define DAO_DATE_H

#include "php_dao.h"

#define DAO_DATE_YEAR          31556926
#define DAO_DATE_MONTH         2629744
#define DAO_DATE_WEEK          604800
#define DAO_DATE_DAY           86400
#define DAO_DATE_HOUR          3600
#define DAO_DATE_MINUTE        60

#define DAO_YEARS_PER_CENTURY   100
#define DAO_YEARS_PER_DECADE    10
#define DAO_MONTHS_PER_YEAR     12
#define DAO_MONTHS_PER_QUARTER  3
#define DAO_WEEKS_PER_YEAR      52
#define DAO_DAYS_PER_WEEK       7
#define DAO_HOURS_PER_DAY       24
#define DAO_MINUTES_PER_HOUR    60
#define DAO_SECONDS_PER_MINUTE  60

#define DAO_DATE_MONTHS_LONG   "%B"
#define DAO_DATE_MONTHS_SHORT  "%b"

extern zend_class_entry *dao_date_ce;

DAO_INIT_CLASS(Dao_Date);

#endif /* DAO_DATE_H */
