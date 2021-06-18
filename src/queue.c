#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "job.h"
#include "queue.h"

Queue create_queue(size_t size, QUEUEABLE_TYPE type)
{
    return create_queue_resize(size, type, false);
}

Queue create_queue_resize(size_t size, QUEUEABLE_TYPE type, bool resize)
{
    Queue q = {.queue_size = size, .type = type, .resize = false};
    q.store = (Queueable *)malloc(sizeof(Queueable) * size);
    return q;
}

bool peek_queue(Queue *queue, Queueable *element)
{
    if (queue->number < 1)
        return false;
    // Copy out the element.
    *element = queue->store[queue->head];
    return true;
}

bool pop_queue(Queue *queue, Queueable *element)
{
    if (queue->number < 1)
        return false;
    // Copy out the element.
    *element = queue->store[queue->head];
    // Update queue status.
    queue->number--;
    queue->head = (queue->head + 1) % queue->queue_size;
    return true;
}

bool push_queue(Queue *queue, Queueable element)
{
    if (queue->number >= queue->queue_size)
        return false;
    // Copy in the element.
    queue->store[queue->head] = element;
    // Update queue status.
    queue->number++;
    queue->tail = (queue->tail + 1) % queue->queue_size;
    return true;
}

void flush_queue(Queue *queue)
{
    queue->number = 0;
    queue->head = 0;
    queue->tail = 0;
    memset(queue->store, 0, sizeof(Queueable) * queue->queue_size);
}
