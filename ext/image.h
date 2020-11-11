
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
  |          Vladimir Kolesnikov <vladimir@free-sevastopol.com>            |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_IMAGE_H
#define DAO_IMAGE_H

#include "php_dao.h"

/* Resizing constraints */
#define DAO_IMAGE_NONE           1
#define DAO_IMAGE_WIDTH          2
#define DAO_IMAGE_HEIGHT         3
#define DAO_IMAGE_AUTO           4
#define DAO_IMAGE_INVERSE        5
#define DAO_IMAGE_PRECISE        6
#define DAO_IMAGE_TENSILE        7
#define DAO_IMAGE_NARROW         8

/* Flipping directions */
#define DAO_IMAGE_HORIZONTAL    11
#define DAO_IMAGE_VERTICAL      12

/* Driver: GD, ImageMagick, etc */
#define DAO_IMAGE_GD            21
#define DAO_IMAGE_IMAGICK       22


extern zend_class_entry *dao_image_ce;

DAO_INIT_CLASS(Dao_Image);

#endif /* DAO_IMAGE_H */
