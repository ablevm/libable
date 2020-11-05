typedef int
(*able_task_exec_t)(void *user);

typedef struct {
	able_task_exec_t f;
	void *u;
	pthread_t c;
} able_task_t;

int
able_task_exec(able_task_t *task);

int
able_task_fork_exec(able_task_t *task);
