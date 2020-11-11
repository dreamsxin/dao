
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

#include "http/request/fileinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_request_fileinterface_ce;

static const zend_function_entry dao_http_request_fileinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, getSize, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, getTempName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, getRealType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Request_FileInterface, moveTo, arginfo_dao_http_request_fileinterface_moveto)
	PHP_FE_END
};

/**
 * Dao\Http\Request\FileInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_Request_FileInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http\\Request, FileInterface, http_request_fileinterface, dao_http_request_fileinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the file size of the uploaded file
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, getSize);

/**
 * Returns the real name of the uploaded file
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, getName);

/**
 * Returns the temporal name of the uploaded file
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, getTempName);

/**
 * Returns the mime type reported by the browser
 * This mime type is not completely secure, use getRealType() instead
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, getType);

/**
 * Gets the real mime type of the upload file using finfo
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, getRealType);

/**
 * Move the temporary file to a destination
 *
 * @param string $destination
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Request_FileInterface, moveTo);
