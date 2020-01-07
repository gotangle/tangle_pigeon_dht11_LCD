/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH6561_QUEUE_H__
#define __UTILS_CONTAINERS_HASH_HASH6561_QUEUE_H__

#include "utlist.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash6561_queue_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_6561];
  struct hash6561_queue_entry_s *next;
  struct hash6561_queue_entry_s *prev;
} hash6561_queue_entry_t;

typedef hash6561_queue_entry_t *hash6561_queue_t;

bool hash6561_queue_empty(hash6561_queue_t const queue);
retcode_t hash6561_queue_push(hash6561_queue_t *const queue, flex_trit_t const *const hash);
hash6561_queue_entry_t *hash6561_queue_pop(hash6561_queue_t *const queue);
flex_trit_t *hash6561_queue_peek(hash6561_queue_t const queue);
void hash6561_queue_free(hash6561_queue_t *const queue);
size_t hash6561_queue_count(hash6561_queue_t const queue);
flex_trit_t *hash6561_queue_at(hash6561_queue_t const queue, size_t index);
retcode_t hash6561_queue_copy(hash6561_queue_t *dest, hash6561_queue_t const src, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH6561_QUEUE_H__
