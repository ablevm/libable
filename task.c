#include <pthread.h>
#include "task.h"

int
able_task_exec(able_task_t *task) {
	int y;
	while ((y = task->ef(task->t)) < 0);
	return y;
}

void *
able_task_exec_shim(void *task) {
	able_task_exec(task);
	return NULL;
}

int
able_task_fork_exec(able_task_t *task) {
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&task->c, &thread_attr, able_task_exec_shim, task);
	pthread_attr_destroy(&thread_attr);
	return 0;
}
