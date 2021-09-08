typedef struct {
	atomic_bool sl;
	pthread_mutex_t m;
	pthread_cond_t v;
	const able_edge_t *w;
} able_node_t;

int
able_node_init(able_node_t *node);

int
able_node_wait(able_node_t *node, const able_edge_t *edge, const struct timespec *time);

int
able_node_post(able_node_t *node, const able_edge_t *edge);
