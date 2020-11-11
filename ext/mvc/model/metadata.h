
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
  +------------------------------------------------------------------------+
*/

#ifndef DAO_MVC_MODEL_METADATA_H
#define DAO_MVC_MODEL_METADATA_H

#include "php_dao.h"

extern zend_class_entry *dao_mvc_model_metadata_ce;

DAO_INIT_CLASS(Dao_Mvc_Model_MetaData);

#define DAO_MVC_MODEL_METADATA_MODELS_ATTRIBUTES                   0
#define DAO_MVC_MODEL_METADATA_MODELS_PRIMARY_KEY                  1
#define DAO_MVC_MODEL_METADATA_MODELS_NON_PRIMARY_KEY              2
#define DAO_MVC_MODEL_METADATA_MODELS_NOT_NULL                     3
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES                   4
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_NUMERIC           5
#define DAO_MVC_MODEL_METADATA_MODELS_DATE_AT                      6
#define DAO_MVC_MODEL_METADATA_MODELS_DATE_IN                      7
#define DAO_MVC_MODEL_METADATA_MODELS_IDENTITY_COLUMN              8
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_BIND              9
#define DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_INSERT    10
#define DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_UPDATE    11
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_DEFAULT_VALUE          12
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_SZIE                   13
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_SCALE                  14
#define DAO_MVC_MODEL_METADATA_MODELS_DATA_BYTE                   15

#define DAO_MVC_MODEL_METADATA_MODELS_COLUMN_MAP                   0
#define DAO_MVC_MODEL_METADATA_MODELS_REVERSE_COLUMN_MAP           1

#endif /* DAO_MVC_MODEL_METADATA_H */
