// Windows version of queue.c
// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32

#include "queue.h"  // NOLINT(build/include_subdir)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "logger.h"          // NOLINT(build/include_subdir)
#include "tensors_struct.h"  // NOLINT(build/include_subdir)

extern Logger *logger;

// Helper for timeout calculation
typedef struct {
  DWORD start_tick;
  DWORD timeout_ms;
} TimeoutHelper;

static void init_timeout_helper(TimeoutHelper *helper, int timeout_ms) {
  helper->start_tick = GetTickCount();
  helper->timeout_ms = (DWORD)timeout_ms;
}

static DWORD remaining_timeout(TimeoutHelper *helper) {
  if (helper->timeout_ms <= 0) return INFINITE;
  DWORD now = GetTickCount();
  DWORD elapsed = now - helper->start_tick;
  if (elapsed >= helper->timeout_ms) return 0;
  return helper->timeout_ms - elapsed;
}

Queue *new_queue(int capacity) {
  log_debug(logger, "Creating new queue with capacity %d", capacity);
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (queue == NULL) {
    return NULL;
  }
  queue->size = 0;
  queue->capacity = capacity;
  queue->shutdown = false;
  queue->head = NULL;
  queue->tail = NULL;
  InitializeCriticalSection(&queue->mutex);
  InitializeConditionVariable(&queue->cond);
  return queue;
}

int enqueue(Queue *queue, tensors_struct *tensors) {
  EnterCriticalSection(&queue->mutex);
  if (queue->shutdown) {
    LeaveCriticalSection(&queue->mutex);
    return 1;
  }
  if (queue->size >= queue->capacity) {
    QueueItem *old_head = queue->head;
    queue->head = old_head->next;
    queue->size--;
    if (queue->head == NULL) {
      queue->tail = NULL;
    }
    if (old_head->tensors != NULL) {
      deep_free_tensors_struct(old_head->tensors);
      old_head->tensors = NULL;
    }
    free(old_head);
    old_head = NULL;
  }
  QueueItem *item = (QueueItem *)malloc(sizeof(QueueItem));
  if (item == NULL) {
    LeaveCriticalSection(&queue->mutex);
    return 1;
  }
  item->tensors = tensors;
  item->next = NULL;
  if (queue->size == 0) {
    queue->head = item;
    queue->tail = item;
  } else {
    queue->tail->next = item;
    queue->tail = item;
  }
  queue->size++;
  WakeConditionVariable(&queue->cond);
  LeaveCriticalSection(&queue->mutex);
  return 0;
}

tensors_struct *dequeue(Queue *queue, int64_t timeout_ms) {
  tensors_struct *tensors = NULL;
  EnterCriticalSection(&queue->mutex);
  TimeoutHelper helper;
  init_timeout_helper(&helper, (int)timeout_ms);
  while (queue->size == 0 && !queue->shutdown) {
    DWORD wait_ms = remaining_timeout(&helper);
    if (wait_ms == 0) {
      LeaveCriticalSection(&queue->mutex);
      return NULL;
    }
    BOOL ok = SleepConditionVariableCS(&queue->cond, &queue->mutex, wait_ms);
    if (!ok) {
      if (GetLastError() == ERROR_TIMEOUT) {
        LeaveCriticalSection(&queue->mutex);
        return NULL;
      }
      log_error(logger, "Cond wait failed in dequeue: %lu", GetLastError());
      LeaveCriticalSection(&queue->mutex);
      return NULL;
    }
  }
  if (queue->shutdown && queue->size == 0) {
    LeaveCriticalSection(&queue->mutex);
    return NULL;
  }
  if (queue->size > 0) {
    QueueItem *item = queue->head;
    queue->head = item->next;
    queue->size--;
    if (queue->head == NULL) {
      queue->tail = NULL;
    }
    tensors = item->tensors;
    free(item);
    item = NULL;
  }
  LeaveCriticalSection(&queue->mutex);
  return tensors;
}

void shutdown_queue(Queue *queue) {
  EnterCriticalSection(&queue->mutex);
  queue->shutdown = true;
  WakeAllConditionVariable(&queue->cond);
  LeaveCriticalSection(&queue->mutex);
}

void free_queue(Queue *queue) {
  if (queue == NULL) {
    return;
  }
  shutdown_queue(queue);
  EnterCriticalSection(&queue->mutex);
  QueueItem *current = queue->head;
  while (current != NULL) {
    QueueItem *next = current->next;
    if (current->tensors != NULL) {
      deep_free_tensors_struct(current->tensors);
      current->tensors = NULL;
    }
    free(current);
    current = next;
  }
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  LeaveCriticalSection(&queue->mutex);
  DeleteCriticalSection(&queue->mutex);
  // No need to destroy condition variable in Windows
  free(queue);
}

#endif  // _WIN32
