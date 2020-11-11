#ifndef DAO_STORAGE_BTREE_UTILS_H_
#define DAO_STORAGE_BTREE_UTILS_H_

#include <stdint.h> /* uint64_t */

uint64_t _dao_storage_btree_compute_hashl(uint64_t key);
uint64_t _dao_htonll(uint64_t value);
uint64_t _dao_ntohll(uint64_t value);

#endif /* DAO_STORAGE_BTREE_UTILS_H_ */
