#include "thread_ctrl.h"

void work_thread_variable_init(void *param, void *(*thread_loop)(void *))
{
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, thread_loop, param);
	pthread_detach(thread_id);
}