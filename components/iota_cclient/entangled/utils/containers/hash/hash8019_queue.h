/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH8019_QUEUE_H__
#define __UTILS_CONTAINERS_HASH_HASH8019_QUEUE_H__

#include "utlist.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash8019_queue_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_8019];
  struct hash8019_queue_entry_s *next;
  struct hash8019_queue_entry_s *prev;
} hash8019_queue_entry_t;

typedef hash8019_queue_entry_t *hash8019_queue_t;

bool hash8019_queue_empty(hash8019_queue_t const queue);
retcode_t hash8019_queue_push(hash8019_queue_t *const queue, flex_trit_t const *const hash);
hash8019_queue_entry_t *hash8019_queue_pop(hash8019_queue_t *const queue);
flex_trit_t *hash8019_queue_peek(hash8019_queue_t const queue);
void hash8019_queue_free(hash8019_queue_t *const queue);
size_t hash8019_queue_count(hash8019_queue_t const queue);
flex_trit_t *hash8019_queue_at(hash8019_queue_t const queue, size_t index);
retcode_t hash8019_queue_copy(hash8019_queue_t *dest, hash8019_queue_t const src, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH8019_QUEUE_H__
