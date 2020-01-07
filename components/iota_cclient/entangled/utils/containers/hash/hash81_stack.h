/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH81_STACK_H__
#define __UTILS_CONTAINERS_HASH_HASH81_STACK_H__

#include "utstack.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#define hash81_stack_empty(stack) STACK_EMPTY(stack)
#define HASH_STACK_FOREACH(stack, iter) for (iter = stack; iter != NULL; iter = iter->next)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash81_stack_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_81];
  struct hash81_stack_entry_s *next;
}
hash81_stack_entry_t;

typedef hash81_stack_entry_t *hash81_stack_t;

retcode_t hash81_stack_push(hash81_stack_t *const stack, flex_trit_t const *const hash);
void hash81_stack_pop(hash81_stack_t *const stack);
flex_trit_t *hash81_stack_peek(hash81_stack_t const stack);
void hash81_stack_free(hash81_stack_t *const stack);
size_t hash81_stack_count(hash81_stack_t const stack);
flex_trit_t *hash81_stack_at(hash81_stack_t const stack, size_t const index);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH81_STACK_H__
