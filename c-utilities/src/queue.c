#include "queue.h"
#include "logger.h"

#include <errno.h>
#include <string.h>// For strerror

extern Logger *logger;

static int max_queue;

Queue *new_queue( int capacity, bool thread_safe ) {
    log_debug( logger, "Creating new queue with capacity %d and thread safety %s",
               capacity, thread_safe ? "enabled" : "disabled" );
    Queue *queue = (Queue *) malloc( sizeof( Queue ) );
    if ( queue == NULL ) {
        return NULL;
    }

    queue->size = 0;
    queue->capacity = capacity;
    queue->thread_safe = thread_safe;
    queue->shutdown = false;
    queue->head = NULL;
    queue->tail = NULL;

    max_queue = 0;

    // Initialize cross-platform mutex and cond
    if (thread_safe) {
        cp_mutex_init(&queue->mutex);
        cp_cond_init(&queue->cond);
    }

    return queue;
}

int enqueue( Queue *queue, tensors_struct *tensors ) {
    if (queue->thread_safe) cp_mutex_lock(&queue->mutex);

    if ( queue->shutdown ) {
        if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);
        return 1;// Indicate failure due to shutdown
    }

    if ( queue->size >= queue->capacity ) {
        // Drop the oldest item (head of the queue)
        QueueItem *old_head = queue->head;
        queue->head = old_head->next;
        queue->size--;

        if ( queue->head == NULL ) {
            // The queue is now empty after removing the head
            queue->tail = NULL;
        }

        // Free the tensors_struct and its data
        if ( old_head->tensors != NULL ) {
            deep_free_tensors_struct( old_head->tensors );
            old_head->tensors = NULL;
        }
        free( old_head );
        old_head = NULL;
    }

    // Proceed to enqueue the new item
    QueueItem *item = (QueueItem *) malloc( sizeof( QueueItem ) );
    if ( item == NULL ) {
        if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);
        return 1;// Indicate failure due to malloc error
    }

    item->tensors = tensors;
    item->next = NULL;

    if ( queue->size == 0 ) {
        queue->head = item;
        queue->tail = item;
    } else {
        queue->tail->next = item;
        queue->tail = item;
    }

    queue->size++;

    if ( max_queue < queue->size ) max_queue = queue->size;

    if (queue->thread_safe) cp_cond_signal(&queue->cond);

    if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);

    return 0;// Success
}

tensors_struct *dequeue( Queue *queue, long timeout_ms ) {
    tensors_struct *tensors = NULL;
    if (queue->thread_safe) cp_mutex_lock(&queue->mutex);

    while ( queue->size == 0 && !queue->shutdown ) {
        int ret;
        if ( timeout_ms <= 0 ) {
            if (queue->thread_safe) {
                ret = cp_cond_wait(&queue->cond, &queue->mutex);
                if ( ret != 0 ) {
                    log_error( logger, "Cond wait failed in dequeue" );
                    cp_mutex_unlock(&queue->mutex);
                    return NULL;
                }
            } else {
                // Not thread safe, just break to avoid infinite loop
                break;
            }
        } else {
            if (queue->thread_safe) {
                ret = cp_cond_timedwait(&queue->cond, &queue->mutex, timeout_ms);
                if ( ret != 0 ) {
                    cp_mutex_unlock(&queue->mutex);
                    return NULL;// Timeout or error occurred
                }
            } else {
                // Not thread safe, just break to avoid infinite loop
                break;
            }
        }
    }

    if ( queue->shutdown && queue->size == 0 ) {
        if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);
        return NULL;
    }

    if ( queue->size > 0 ) {
        QueueItem *item = queue->head;
        queue->head = item->next;
        queue->size--;

        if ( queue->head == NULL ) {
            queue->tail = NULL;
        }

        tensors = item->tensors;
        free( item );
        item = NULL;
    }

    if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);
    return tensors;
}

void shutdown_queue( Queue *queue ) {
    if (queue->thread_safe) cp_mutex_lock(&queue->mutex);

    queue->shutdown = true;

    // Wake up all waiting threads
    if (queue->thread_safe) cp_cond_broadcast(&queue->cond);

    if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);
}

void free_queue( Queue *queue ) {
    if ( queue == NULL ) {
        return;
    }

    // Shutdown the queue first
    shutdown_queue( queue );

    // Lock the mutex to safely free the queue
    if (queue->thread_safe) cp_mutex_lock(&queue->mutex);

    // Free all items in the queue
    QueueItem *current = queue->head;
    while ( current != NULL ) {
        QueueItem *next = current->next;
        if ( current->tensors != NULL ) {
            // Free the tensors_struct and its data
            deep_free_tensors_struct( current->tensors );
            current->tensors = NULL;
        }
        free( current );
        current = next;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    if (queue->thread_safe) cp_mutex_unlock(&queue->mutex);

    if (queue->thread_safe) {
        cp_mutex_destroy(&queue->mutex);
        cp_cond_destroy(&queue->cond);
    }

    free( queue );
}
