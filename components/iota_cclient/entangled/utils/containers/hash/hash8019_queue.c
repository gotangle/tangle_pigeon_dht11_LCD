/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdlib.h>

#include "utils/containers/hash/hash8019_queue.h"

bool hash8019_queue_empty(hash8019_queue_t const queue) { return (queue == NULL); }

retcode_t hash8019_queue_push(hash8019_queue_t *const queue, flex_trit_t const *const hash) {
  hash8019_queue_entry_t *entry = NULL;

  if ((entry = (hash8019_queue_entry_t *)malloc(sizeof(hash8019_queue_entry_t))) == NULL) {
    return RC_OOM;
  }
  memcpy(entry->hash, hash, FLEX_TRIT_SIZE_8019);
  CDL_APPEND(*queue, entry);

  return RC_OK;
}

hash8019_queue_entry_t *hash8019_queue_pop(hash8019_queue_t *const queue) {
  hash8019_queue_entry_t *front = NULL;

  front = *queue;
  if (front != NULL) {
    CDL_DELETE(*queue, front);
  }

  return front;
}

flex_trit_t *hash8019_queue_peek(hash8019_queue_t const queue) {
  if (queue == NULL) {
    return NULL;
  }
  return (flex_trit_t *)(queue->hash);
}

void hash8019_queue_free(hash8019_queue_t *const queue) {
  hash8019_queue_entry_t *iter = NULL, *tmp1 = NULL, *tmp2 = NULL;

  CDL_FOREACH_SAFE(*queue, iter, tmp1, tmp2) {
    CDL_DELETE(*queue, iter);
    free(iter);
  }
}

size_t hash8019_queue_count(hash8019_queue_t const queue) {
  hash8019_queue_entry_t *iter = NULL;
  size_t count = 0;
  CDL_COUNT(queue, iter, count);
  return count;
}

flex_trit_t *hash8019_queue_at(hash8019_queue_t const queue, size_t index) {
  hash8019_queue_entry_t *iter = NULL;
  size_t count = 0;
  CDL_FOREACH(queue, iter) {
    if (count == index) {
      return (flex_trit_t *)(iter->hash);
    }
    count++;
  }
  return NULL;
}

retcode_t hash8019_queue_copy(hash8019_queue_t *dest, hash8019_queue_t const src, size_t size) {
  size_t count = 1;
  hash8019_queue_entry_t *iter = NULL;
  // if src is empty, returns NULL.
  if (src == NULL) {
    return RC_NULL_PARAM;
  }

  // copying elements to dest
  CDL_FOREACH(src, iter) {
    if (hash8019_queue_push(dest, iter->hash) == RC_OK) {
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
