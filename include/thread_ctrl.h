#ifndef THREAD_CTRL_H
#define THREAD_CTRL_H

#include <pthread.h>

void work_thread_variable_init(void *param,void *(*thread_loop)(void *));

#endif