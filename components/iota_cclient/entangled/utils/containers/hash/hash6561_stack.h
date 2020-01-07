/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH6561_STACK_H__
#define __UTILS_CONTAINERS_HASH_HASH6561_STACK_H__

#include "utstack.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#define hash6561_stack_empty(stack) STACK_EMPTY(stack)
#define HASH_STACK_FOREACH(stack, iter) for (iter = stack; iter != NULL; iter = iter->next)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash6561_stack_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_6561];
  struct hash6561_stack_entry_s *next;
}
hash6561_stack_entry_t;

typedef hash6561_stack_entry_t *hash6561_stack_t;

retcode_t hash6561_stack_push(hash6561_stack_t *const stack, flex_trit_t const *const hash);
void hash6561_stack_pop(hash6561_stack_t *const stack);
flex_trit_t *hash6561_stack_peek(hash6561_stack_t const stack);
void hash6561_stack_free(hash6561_stack_t *const stack);
size_t hash6561_stack_count(hash6561_stack_t const stack);
flex_trit_t *hash6561_stack_at(hash6561_stack_t const stack, size_t const index);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH6561_STACK_H__
