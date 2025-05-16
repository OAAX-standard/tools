// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_QUEUE_H_
#define C_UTILITIES_INCLUDE_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tensors_struct.h"  // NOLINT(build/include_subdir)

typedef struct QueueItem {
  tensors_struct *tensors;  // The item in the queue
  struct QueueItem *next;   // Pointer to the next item in the queue
} QueueItem;

#ifdef _WIN32
#include <windows.h>
typedef struct {
  int32_t size;
  int32_t capacity;
  bool shutdown;
  QueueItem *head, *tail;
  CRITICAL_SECTION mutex;      // Windows critical section for thread-safety
  CONDITION_VARIABLE cond;     // Windows condition variable for thread-safety
} Queue;
#else
#include <pthread.h>
typedef struct {
  int32_t size;
  int32_t capacity;
  bool shutdown;
  QueueItem *head, *tail;
  pthread_mutex_t mutex;       // POSIX mutex for thread-safety
  pthread_cond_t cond;         // POSIX condition variable for thread-safety
} Queue;
#endif

/**
 * @brief Create a new queue.
 *
 * @param capacity The maximum number of items that the queue can hold.
 * @param thread_safe Whether the queue should be thread-safe.
 *
 * @return A pointer to the newly created queue.
 */
Queue *new_queue(int capacity);

/**
 * @brief Enqueue a new item to the queue.
 *
 * @note The function will only copy the reference to the item, not the item
 * itself. The caller is responsible for managing the memory of the item.
 *
 * @param queue The queue to which the item should be enqueued.
 * @param tensors The item to be enqueued.
 *
 * @return 0 if the item is enqueued successfully, and non-zero otherwise.
 */
int enqueue(Queue *queue, tensors_struct *tensors);

/**
 * @brief Dequeue an item from the queue.
 *
 * @param queue The queue from which the item should be dequeued.
 * @param timeout_ms The timeout in milliseconds. If the timeout is nonpositive,
 * the function will block indefinitely.
 *
 * @return The dequeued item.
 */
tensors_struct *dequeue(Queue *queue, int64_t timeout_ms);

/**
 * @brief Shutdown the queue. No further items can be enqueued.
 *
 * @param queue The queue to be shut down.
 */
void shutdown_queue(Queue *queue);

/**
 * @brief Free the queue and all its items.
 *
 * @warning The function will free the memory of the items in the queue.
 *
 * @param queue The queue to be freed.
 */
void free_queue(Queue *queue);

#endif  // C_UTILITIES_INCLUDE_QUEUE_H_
