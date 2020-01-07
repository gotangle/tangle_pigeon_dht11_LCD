/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH27_QUEUE_H__
#define __UTILS_CONTAINERS_HASH_HASH27_QUEUE_H__

#include "utlist.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash27_queue_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_27];
  struct hash27_queue_entry_s *next;
  struct hash27_queue_entry_s *prev;
} hash27_queue_entry_t;

typedef hash27_queue_entry_t *hash27_queue_t;

bool hash27_queue_empty(hash27_queue_t const queue);
retcode_t hash27_queue_push(hash27_queue_t *const queue, flex_trit_t const *const hash);
hash27_queue_entry_t *hash27_queue_pop(hash27_queue_t *const queue);
flex_trit_t *hash27_queue_peek(hash27_queue_t const queue);
void hash27_queue_free(hash27_queue_t *const queue);
size_t hash27_queue_count(hash27_queue_t const queue);
flex_trit_t *hash27_queue_at(hash27_queue_t const queue, size_t index);
retcode_t hash27_queue_copy(hash27_queue_t *dest, hash27_queue_t const src, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH27_QUEUE_H__
