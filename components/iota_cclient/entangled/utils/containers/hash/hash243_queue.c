/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdlib.h>

#include "utils/containers/hash/hash243_queue.h"

bool hash243_queue_empty(hash243_queue_t const queue) { return (queue == NULL); }

retcode_t hash243_queue_push(hash243_queue_t *const queue, flex_trit_t const *const hash) {
  hash243_queue_entry_t *entry = NULL;

  if ((entry = (hash243_queue_entry_t *)malloc(sizeof(hash243_queue_entry_t))) == NULL) {
    return RC_OOM;
  }
  memcpy(entry->hash, hash, FLEX_TRIT_SIZE_243);
  CDL_APPEND(*queue, entry);

  return RC_OK;
}

hash243_queue_entry_t *hash243_queue_pop(hash243_queue_t *const queue) {
  hash243_queue_entry_t *front = NULL;

  front = *queue;
  if (front != NULL) {
    CDL_DELETE(*queue, front);
  }

  return front;
}

flex_trit_t *hash243_queue_peek(hash243_queue_t const queue) {
  if (queue == NULL) {
    return NULL;
  }
  return (flex_trit_t *)(queue->hash);
}

void hash243_queue_free(hash243_queue_t *const queue) {
  hash243_queue_entry_t *iter = NULL, *tmp1 = NULL, *tmp2 = NULL;

  CDL_FOREACH_SAFE(*queue, iter, tmp1, tmp2) {
    CDL_DELETE(*queue, iter);
    free(iter);
  }
}

size_t hash243_queue_count(hash243_queue_t const queue) {
  hash243_queue_entry_t *iter = NULL;
  size_t count = 0;
  CDL_COUNT(queue, iter, count);
  return count;
}

flex_trit_t *hash243_queue_at(hash243_queue_t const queue, size_t index) {
  hash243_queue_entry_t *iter = NULL;
  size_t count = 0;
  CDL_FOREACH(queue, iter) {
    if (count == index) {
      return (flex_trit_t *)(iter->hash);
    }
    count++;
  }
  return NULL;
}

retcode_t hash243_queue_copy(hash243_queue_t *dest, hash243_queue_t const src, size_t size) {
  size_t count = 1;
  hash243_queue_entry_t *iter = NULL;
  // if src is empty, returns NULL.
  if (src == NULL) {
    return RC_NULL_PARAM;
  }

  // copying elements to dest
  CDL_FOREACH(src, iter) {
    if (hash243_queue_push(dest, iter->hash) == RC_OK) {
      if (count == size) {
        break;
      }
      count++;
    } else {
      return RC_OOM;
    }
  }

  return RC_OK;
}
