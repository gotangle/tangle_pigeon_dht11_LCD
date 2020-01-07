/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef __UTILS_CONTAINERS_HASH_HASH8019_STACK_H__
#define __UTILS_CONTAINERS_HASH_HASH8019_STACK_H__

#include "utstack.h"

#include "common/errors.h"
#include "common/trinary/flex_trit.h"

#define hash8019_stack_empty(stack) STACK_EMPTY(stack)
#define HASH_STACK_FOREACH(stack, iter) for (iter = stack; iter != NULL; iter = iter->next)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash8019_stack_entry_s {
  flex_trit_t hash[FLEX_TRIT_SIZE_8019];
  struct hash8019_stack_entry_s *next;
}
hash8019_stack_entry_t;

typedef hash8019_stack_entry_t *hash8019_stack_t;

retcode_t hash8019_stack_push(hash8019_stack_t *const stack, flex_trit_t const *const hash);
void hash8019_stack_pop(hash8019_stack_t *const stack);
flex_trit_t *hash8019_stack_peek(hash8019_stack_t const stack);
void hash8019_stack_free(hash8019_stack_t *const stack);
size_t hash8019_stack_count(hash8019_stack_t const stack);
flex_trit_t *hash8019_stack_at(hash8019_stack_t const stack, size_t const index);

#ifdef __cplusplus
}
#endif

#endif  // __UTILS_CONTAINERS_HASH_HASH8019_STACK_H__
