#ifndef MUTEX_CTRL_H
#define MUTEX_CTRL_H

#include <pthread.h>

typedef pthread_mutex_t mutex_t;

void mutex_init(mutex_t *mutex);
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);

#endif