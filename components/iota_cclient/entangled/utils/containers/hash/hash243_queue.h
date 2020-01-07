/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH243_QUEUE_H__
#define __UTILS_CONTAINERS_HASH_HASH243_QUEUE_H__

#include "utlist.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash243_queue_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_243];
  struct hash243_queue_entry_s *next;
  struct hash243_queue_entry_s *prev;
} hash243_queue_entry_t;

typedef hash243_queue_entry_t *hash243_queue_t;

bool hash243_queue_empty(hash243_queue_t const queue);
retcode_t hash243_queue_push(hash243_queue_t *const queue, flex_trit_t const *const hash);
hash243_queue_entry_t *hash243_queue_pop(hash243_queue_t *const queue);
flex_trit_t *hash243_queue_peek(hash243_queue_t const queue);
void hash243_queue_free(hash243_queue_t *const queue);
size_t hash243_queue_count(hash243_queue_t const queue);
flex_trit_t *hash243_queue_at(hash243_queue_t const queue, size_t index);
retcode_t hash243_queue_copy(hash243_queue_t *dest, hash243_queue_t const src, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH243_QUEUE_H__
