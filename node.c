#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include <errno.h>

int
able_node_init(able_node_t *node) {
	atomic_store(&node->sl, 0);
	pthread_mutex_init(&node->m, NULL);
	pthread_cond_init(&node->v, NULL);
	node->w = NULL;
	return 0;
}

int
able_node_wait(able_node_t *node, const able_edge_t *edge, const struct timespec *time) {
	if (edge == NULL)
		return 0;
	if (atomic_load(&edge->rc) > 0)
		return 1;
	atomic_store(&node->sl, 1);
	pthread_mutex_lock(&node->m);
	node->w = edge;
	if (time == NULL) {
		while (atomic_load(&edge->rc) == 0)
			pthread_cond_wait(&node->v, &node->m);
		node->w = NULL;
		pthread_mutex_unlock(&node->m);
		atomic_store(&node->sl, 0);
		return 0;
	} else {
		int y;
		y = 0;
		while (atomic_load(&edge->rc) == 0 && y == 0)
			y = pthread_cond_timedwait(&node->v, &node->m, time);
		node->w = NULL;
		pthread_mutex_unlock(&node->m);
		atomic_store(&node->sl, 0);
		if (y == EINVAL)
			return 2;
		if (y == ETIMEDOUT)
			return 3;
		return 0;
	}
}

int
able_node_post(able_node_t *node, const able_edge_t *edge) {
	if (atomic_load(&node->sl) == 0)
		return 0;
	pthread_mutex_lock(&node->m);
	if (node->w != edge) {
		pthread_mutex_unlock(&node->m);
		return 0;
	}
	pthread_cond_signal(&node->v);
	pthread_mutex_unlock(&node->m);
	return 0;
}
