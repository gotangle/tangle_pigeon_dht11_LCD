/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdlib.h>

#include "utils/containers/hash/hash6561_stack.h"

retcode_t hash6561_stack_push(hash6561_stack_t *const stack, flex_trit_t const *const hash) {
  hash6561_stack_entry_t *entry = NULL;

  if ((entry = (hash6561_stack_entry_t *)malloc(sizeof(hash6561_stack_entry_t))) == NULL) {
    return RC_OOM;
  }

  memcpy(entry->hash, hash, FLEX_TRIT_SIZE_6561);
  STACK_PUSH(*stack, entry);

  return RC_OK;
}

void hash6561_stack_pop(hash6561_stack_t *const stack) {
  hash6561_stack_entry_t *tmp = NULL;

  tmp = *stack;
  STACK_POP(*stack, *stack);
  free(tmp);
}

flex_trit_t *hash6561_stack_peek(hash6561_stack_t const stack) { return (flex_trit_t *)(STACK_TOP(stack)->hash); }

void hash6561_stack_free(hash6561_stack_t *const stack) {
  hash6561_stack_entry_t *iter = NULL;

  while (!STACK_EMPTY(*stack)) {
    STACK_POP(*stack, iter);
    free(iter);
  }
}

size_t hash6561_stack_count(hash6561_stack_t const stack) {
  hash6561_stack_entry_t *tmp = NULL;
  size_t count = 0;

  STACK_COUNT(stack, tmp, count);

  return count;
}

flex_trit_t *hash6561_stack_at(hash6561_stack_t const stack, size_t const index) {
  hash6561_stack_entry_t *iter = NULL;
  size_t count = 0;

  HASH_STACK_FOREACH(stack, iter) {
    if (count == index) {
      return (flex_trit_t *)(iter->hash);
    }
    count++;
  }
  return NULL;
}
