/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH243_STACK_H__
#define __UTILS_CONTAINERS_HASH_HASH243_STACK_H__

#include "utstack.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#define hash243_stack_empty(stack) STACK_EMPTY(stack)
#define HASH_STACK_FOREACH(stack, iter) for (iter = stack; iter != NULL; iter = iter->next)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash243_stack_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_243];
  struct hash243_stack_entry_s *next;
}
hash243_stack_entry_t;

typedef hash243_stack_entry_t *hash243_stack_t;

retcode_t hash243_stack_push(hash243_stack_t *const stack, flex_trit_t const *const hash);
void hash243_stack_pop(hash243_stack_t *const stack);
flex_trit_t *hash243_stack_peek(hash243_stack_t const stack);
void hash243_stack_free(hash243_stack_t *const stack);
size_t hash243_stack_count(hash243_stack_t const stack);
flex_trit_t *hash243_stack_at(hash243_stack_t const stack, size_t const index);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH243_STACK_H__
