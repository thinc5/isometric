#ifndef JOB_H
#define JOB_H

#include <stdbool.h>

typedef struct Job
{
    void *data;
    bool (*process)(void *data);
    void (*render)(void *data);
} Job;

#endif
