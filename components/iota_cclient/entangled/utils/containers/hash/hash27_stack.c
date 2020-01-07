/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdlib.h>

#include "utils/containers/hash/hash27_stack.h"

retcode_t hash27_stack_push(hash27_stack_t *const stack, flex_trit_t const *const hash) {
  hash27_stack_entry_t *entry = NULL;

  if ((entry = (hash27_stack_entry_t *)malloc(sizeof(hash27_stack_entry_t))) == NULL) {
    return RC_OOM;
  }

  memcpy(entry->hash, hash, FLEX_TRIT_SIZE_27);
  STACK_PUSH(*stack, entry);

  return RC_OK;
}

void hash27_stack_pop(hash27_stack_t *const stack) {
  hash27_stack_entry_t *tmp = NULL;

  tmp = *stack;
  STACK_POP(*stack, *stack);
  free(tmp);
}

flex_trit_t *hash27_stack_peek(hash27_stack_t const stack) { return (flex_trit_t *)(STACK_TOP(stack)->hash); }

void hash27_stack_free(hash27_stack_t *const stack) {
  hash27_stack_entry_t *iter = NULL;

  while (!STACK_EMPTY(*stack)) {
    STACK_POP(*stack, iter);
    free(iter);
  }
}

size_t hash27_stack_count(hash27_stack_t const stack) {
  hash27_stack_entry_t *tmp = NULL;
  size_t count = 0;

  STACK_COUNT(stack, tmp, count);

  return count;
}

flex_trit_t *hash27_stack_at(hash27_stack_t const stack, size_t const index) {
  hash27_stack_entry_t *iter = NULL;
  size_t count = 0;

  HASH_STACK_FOREACH(stack, iter) {
    if (count == index) {
      return (flex_trit_t *)(iter->hash);
    }
    count++;
  }
  return NULL;
}
