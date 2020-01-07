/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH81_QUEUE_H__
#define __UTILS_CONTAINERS_HASH_HASH81_QUEUE_H__

#include "utlist.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash81_queue_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_81];
  struct hash81_queue_entry_s *next;
  struct hash81_queue_entry_s *prev;
} hash81_queue_entry_t;

typedef hash81_queue_entry_t *hash81_queue_t;

bool hash81_queue_empty(hash81_queue_t const queue);
retcode_t hash81_queue_push(hash81_queue_t *const queue, flex_trit_t const *const hash);
hash81_queue_entry_t *hash81_queue_pop(hash81_queue_t *const queue);
flex_trit_t *hash81_queue_peek(hash81_queue_t const queue);
void hash81_queue_free(hash81_queue_t *const queue);
size_t hash81_queue_count(hash81_queue_t const queue);
flex_trit_t *hash81_queue_at(hash81_queue_t const queue, size_t index);
retcode_t hash81_queue_copy(hash81_queue_t *dest, hash81_queue_t const src, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH81_QUEUE_H__
