
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

#include "http/client/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_client_adapterinterface_ce;

static const zend_function_entry dao_http_client_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setUserAgent, arginfo_dao_http_client_adapterinterface_setuseragent)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setAuth, arginfo_dao_http_client_adapterinterface_setauth)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setHeader, arginfo_dao_http_client_adapterinterface_setheader)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setHeaders, arginfo_dao_http_client_adapterinterface_setheaders)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setData, arginfo_dao_http_client_adapterinterface_setdata)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setFiles, arginfo_dao_http_client_adapterinterface_setfiles)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, get, arginfo_dao_http_client_adapterinterface_get)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, head, arginfo_dao_http_client_adapterinterface_head)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, post, arginfo_dao_http_client_adapterinterface_post)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, put, arginfo_dao_http_client_adapterinterface_put)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, delete, arginfo_dao_http_client_adapterinterface_delete)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setUri, arginfo_dao_http_client_adapterinterface_seturi)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, getUri, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setBaseUri, arginfo_dao_http_client_adapterinterface_setbaseuri)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setMethod, arginfo_dao_http_client_adapterinterface_setmethod)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, setTimeOut, arginfo_dao_http_client_adapterinterface_settimeout)
	PHP_ABSTRACT_ME(Dao_Http_Client_AdapterInterface, send, arginfo_dao_http_client_adapterinterface_send)
	PHP_FE_END
};


/**
 * Dao\Http\Client\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_Client_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http\\Client, AdapterInterface, http_client_adapterinterface, dao_http_client_adapterinterface_method_entry);

	return SUCCESS;
}

DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setUserAgent);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setAuth);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setHeader);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setHeaders);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setData);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setFile);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, get);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, head);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, post);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, put);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, delete);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setUri);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, getUri);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setBaseUri);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setMethod);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, setTimeOut);
DAO_DOC_METHOD(Dao_Http_Client_AdapterInterface, send);
