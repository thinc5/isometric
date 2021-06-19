#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "job.h"

typedef enum QUEUEABLE_TYPE
{
    QUEUEABLE_NONE,
    QUEUEABLE_JOB,
    QUEUEABLE_TYPES
} QUEUEABLE_TYPE;

typedef union Queueable
{
    Job job;
} Queueable;

typedef struct Queue
{
    size_t queue_size;
    size_t number;
    size_t head;
    size_t tail;
    QUEUEABLE_TYPE type;
    Queueable *store;
    bool resize;
} Queue;

Queue create_queue(size_t size, QUEUEABLE_TYPE type);

Queue create_queue_resize(size_t size, QUEUEABLE_TYPE type, bool resize);

bool peek_queue(Queue *queue, Queueable *element);

bool pop_queue(Queue *queue, Queueable *element);

bool push_queue(Queue *queue, Queueable element);

void flush_queue(Queue *queue);

#endif
