
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

#ifndef DAO_KERNEL_CONCAT_H
#define DAO_KERNEL_CONCAT_H

#include "php_dao.h"

#define DAO_SCONCAT(result, op1) \
	 dao_concat_self(result, op1);
#define DAO_SCONCAT_STR(result, op1) \
	 dao_concat_self_str(result, op1, sizeof(op1)-1);

#define DAO_CONCAT_SV(result, op1, op2) \
	 dao_concat_sv(result, op1, sizeof(op1)-1, op2, 0);
#define DAO_SCONCAT_SV(result, op1, op2) \
	 dao_concat_sv(result, op1, sizeof(op1)-1, op2, 1);

#define DAO_CONCAT_SVS(result, op1, op2, op3) \
	 dao_concat_svs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, 0);
#define DAO_SCONCAT_SVS(result, op1, op2, op3) \
	 dao_concat_svs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, 1);

#define DAO_CONCAT_SVSV(result, op1, op2, op3, op4) \
	 dao_concat_svsv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, 0);
#define DAO_SCONCAT_SVSV(result, op1, op2, op3, op4) \
	 dao_concat_svsv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, 1);

#define DAO_CONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 dao_concat_svsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, 0);
#define DAO_SCONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 dao_concat_svsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, 1);

#define DAO_CONCAT_SVSVSV(result, op1, op2, op3, op4, op5, op6) \
	 dao_concat_svsvsv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, 0);
#define DAO_SCONCAT_SVSVSV(result, op1, op2, op3, op4, op5, op6) \
	 dao_concat_svsvsv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, 1);

#define DAO_CONCAT_SVSVSVS(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_svsvsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, op7, sizeof(op7)-1, 0);
#define DAO_SCONCAT_SVSVSVS(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_svsvsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, op7, sizeof(op7)-1, 1);

#define DAO_CONCAT_SVSVSVSVS(result, op1, op2, op3, op4, op5, op6, op7, op8, op9) \
	 dao_concat_svsvsvsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, op7, sizeof(op7)-1, op8, op9, sizeof(op9)-1, 0);

#define DAO_SCONCAT_SVSVSVSVS(result, op1, op2, op3, op4, op5, op6, op7, op8, op9) \
	 dao_concat_svsvsvsvs(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, op6, op7, sizeof(op7)-1, op8, op9, sizeof(op9)-1, 1);

#define DAO_CONCAT_SVSVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_svsvv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, 0);
#define DAO_SCONCAT_SVSVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_svsvv(result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, 1);

#define DAO_CONCAT_SVV(result, op1, op2, op3) \
	 dao_concat_svv(result, op1, sizeof(op1)-1, op2, op3, 0);
#define DAO_SCONCAT_SVV(result, op1, op2, op3) \
	 dao_concat_svv(result, op1, sizeof(op1)-1, op2, op3, 1);
#define DAO_CONCAT_SVVV(result, op1, op2, op3, op4) \
	 dao_concat_svvv(result, op1, sizeof(op1)-1, op2, op3, op4, 0);
#define DAO_SCONCAT_SVVV(result, op1, op2, op3, op4) \
	 dao_concat_svvv(result, op1, sizeof(op1)-1, op2, op3, op4, 1);

#define DAO_CONCAT_SVVS(result, op1, op2, op3, op4) \
	 dao_concat_svvs(result, op1, sizeof(op1)-1, op2, op3, op4, sizeof(op4)-1, 0);
#define DAO_SCONCAT_SVVS(result, op1, op2, op3, op4) \
	 dao_concat_svvs(result, op1, sizeof(op1)-1, op2, op3, op4, sizeof(op4)-1, 1);

#define DAO_CONCAT_SS(result, op1, op2) \
	 dao_concat_ss(result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, 0);
#define DAO_SCONCAT_SS(result, op1, op2) \
	 dao_concat_ss(result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, 1);

#define DAO_CONCAT_SSS(result, op1, op2, op3) \
	 dao_concat_sss(result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, op3, sizeof(op3)-1, 0);
#define DAO_SCONCAT_SSS(result, op1, op2, op3) \
	 dao_concat_sss(result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, op3, sizeof(op3)-1, 1);

#define DAO_CONCAT_VS(result, op1, op2) \
	 dao_concat_vs(result, op1, op2, sizeof(op2)-1, 0);
#define DAO_SCONCAT_VS(result, op1, op2) \
	 dao_concat_vs(result, op1, op2, sizeof(op2)-1, 1);


#define DAO_CONCAT_VSV(result, op1, op2, op3) \
	 dao_concat_vsv(result, op1, op2, sizeof(op2)-1, op3, 0);
#define DAO_SCONCAT_VSV(result, op1, op2, op3) \
	 dao_concat_vsv(result, op1, op2, sizeof(op2)-1, op3, 1);

#define DAO_CONCAT_VSVS(result, op1, op2, op3, op4) \
	 dao_concat_vsvs(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, 0);
#define DAO_SCONCAT_VSVS(result, op1, op2, op3, op4) \
	 dao_concat_vsvs(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, 1);

#define DAO_CONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vsvsv(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, 0);
#define DAO_SCONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vsvsv(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, 1);

#define DAO_CONCAT_VSVSVS(result, op1, op2, op3, op4, op5, op6) \
	 dao_concat_vsvsvs(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, 0);
#define DAO_SCONCAT_VSVSVS(result, op1, op2, op3, op4, op5, op6) \
	 dao_concat_vsvsvs(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, 1);

#define DAO_CONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_vsvsvsv(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, op7, 0);
#define DAO_SCONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_vsvsvsv(result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, op7, 1);

#define DAO_CONCAT_VSVV(result, op1, op2, op3, op4) \
	 dao_concat_vsvv(result, op1, op2, sizeof(op2)-1, op3, op4, 0);
#define DAO_SCONCAT_VSVV(result, op1, op2, op3, op4) \
	 dao_concat_vsvv(result, op1, op2, sizeof(op2)-1, op3, op4, 1);

#define DAO_CONCAT_VSVVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vsvvv(result, op1, op2, sizeof(op2)-1, op3, op4, op5, 0);
#define DAO_SCONCAT_VSVVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vsvvv(result, op1, op2, sizeof(op2)-1, op3, op4, op5, 1);

#define DAO_CONCAT_VV(result, op1, op2) \
	 dao_concat_vv(result, op1, op2, 0);
#define DAO_SCONCAT_VV(result, op1, op2) \
	 dao_concat_vv(result, op1, op2, 1);

#define DAO_CONCAT_VVS(result, op1, op2, op3) \
	 dao_concat_vvs(result, op1, op2, op3, sizeof(op3)-1, 0);
#define DAO_SCONCAT_VVS(result, op1, op2, op3) \
	 dao_concat_vvs(result, op1, op2, op3, sizeof(op3)-1, 1);

#define DAO_CONCAT_VVSV(result, op1, op2, op3, op4) \
	 dao_concat_vvsv(result, op1, op2, op3, sizeof(op3)-1, op4, 0);
#define DAO_SCONCAT_VVSV(result, op1, op2, op3, op4) \
	 dao_concat_vvsv(result, op1, op2, op3, sizeof(op3)-1, op4, 1);

#define DAO_CONCAT_VVV(result, op1, op2, op3) \
	 dao_concat_vvv(result, op1, op2, op3, 0);
#define DAO_SCONCAT_VVV(result, op1, op2, op3) \
	 dao_concat_vvv(result, op1, op2, op3, 1);

#define DAO_CONCAT_VVVSV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vvvsv(result, op1, op2, op3, op4, sizeof(op4)-1, op5, 0);
#define DAO_SCONCAT_VVVSV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vvvsv(result, op1, op2, op3, op4, sizeof(op4)-1, op5, 1);

#define DAO_CONCAT_VVVV(result, op1, op2, op3, op4) \
	 dao_concat_vvvv(result, op1, op2, op3, op4, 0);
#define DAO_SCONCAT_VVVV(result, op1, op2, op3, op4) \
	 dao_concat_vvvv(result, op1, op2, op3, op4, 1);

#define DAO_CONCAT_VVVVSVV(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_vvvvsvv(result, op1, op2, op3, op4, op5, sizeof(op5)-1, op6, op7, 0);
#define DAO_SCONCAT_VVVVSVV(result, op1, op2, op3, op4, op5, op6, op7) \
	 dao_concat_vvvvsvv(result, op1, op2, op3, op4, op5, sizeof(op5)-1, op6, op7, 1);

#define DAO_CONCAT_VVVVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vvvvv(result, op1, op2, op3, op4, op5, 0);
#define DAO_SCONCAT_VVVVV(result, op1, op2, op3, op4, op5) \
	 dao_concat_vvvvv(result, op1, op2, op3, op4, op5, 1);


void dao_concat_sv(zval *result, const char *op1, uint32_t op1_len, zval *op2, int self_var);
void dao_concat_svs(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, int self_var);
void dao_concat_svsv(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, int self_var);
void dao_concat_svsvs(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, const char *op5, uint32_t op5_len, int self_var);
void dao_concat_svsvsv(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, const char *op5, uint32_t op5_len, zval *op6, int self_var);
void dao_concat_svsvsvs(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, const char *op5, uint32_t op5_len, zval *op6, const char *op7, uint32_t op7_len, int self_var);
void dao_concat_svsvsvsvs(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, const char *op5, uint32_t op5_len, zval *op6, const char *op7, uint32_t op7_len, zval *op8, const char *op9, uint32_t op9_len, int self_var);
void dao_concat_svsvv(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, zval *op5, int self_var);
void dao_concat_svv(zval *result, const char *op1, uint32_t op1_len, zval *op2, zval *op3, int self_var);
void dao_concat_svvv(zval *result, const char *op1, uint32_t op1_len, zval *op2, zval *op3, zval *op4, int self_var);
void dao_concat_svvs(zval *result, const char *op1, uint32_t op1_len, zval *op2, zval *op3, const char *op4, uint32_t op4_len, int self_var);
void dao_concat_ss(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, int self_var);
void dao_concat_sss(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, const char *op3, uint32_t op3_len, int self_var);

void dao_concat_vs(zval *result, zval *op1, const char *op2, uint32_t op2_len, int self_var);
void dao_concat_vsv(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, int self_var);
void dao_concat_vsvs(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, int self_var);
void dao_concat_vsvsv(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, zval *op5, int self_var);
void dao_concat_vsvsvs(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, zval *op5, const char *op6, uint32_t op6_len, int self_var);
void dao_concat_vsvsvsv(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, zval *op5, const char *op6, uint32_t op6_len, zval *op7, int self_var);
void dao_concat_vsvv(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, zval *op4, int self_var);
void dao_concat_vsvvv(zval *result, zval *op1, const char *op2, uint32_t op2_len, zval *op3, zval *op4, zval *op5, int self_var);
void dao_concat_vv(zval *result, zval *op1, zval *op2, int self_var);
void dao_concat_vvs(zval *result, zval *op1, zval *op2, const char *op3, uint32_t op3_len, int self_var);
void dao_concat_vvsv(zval *result, zval *op1, zval *op2, const char *op3, uint32_t op3_len, zval *op4, int self_var);
void dao_concat_vvv(zval *result, zval *op1, zval *op2, zval *op3, int self_var);
void dao_concat_vvvsv(zval *result, zval *op1, zval *op2, zval *op3, const char *op4, uint32_t op4_len, zval *op5, int self_var);
void dao_concat_vvvv(zval *result, zval *op1, zval *op2, zval *op3, zval *op4, int self_var);
void dao_concat_vvvvsvv(zval *result, zval *op1, zval *op2, zval *op3, zval *op4, const char *op5, uint32_t op5_len, zval *op6, zval *op7, int self_var);
void dao_concat_vvvvv(zval *result, zval *op1, zval *op2, zval *op3, zval *op4, zval *op5, int self_var);

void dao_concat_self(zval *left, zval *right);
void dao_concat_self_str(zval *left, const char *right, int right_length);

void dao_concat_self_long(zval *left, const long right);
void dao_concat_self_char(zval *left, unsigned char right);
#endif /* DAO_KERNEL_CONCAT_H */
