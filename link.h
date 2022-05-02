typedef struct {
	atomic_int sl;
	able_edge_t *e;
	uintptr_t i;
	void *n;
} able_link_t;

int
able_link_join(able_link_t *link, able_edge_t *edge, uintptr_t mark, void *node);

int
able_link_send_long(able_link_t *link, size_t size, void **data);

int
able_link_send_done(able_link_t *link, size_t size);

int
able_link_send(able_link_t *link, const void *data, size_t size);

int
able_link_node_post_shim(void *node, const able_edge_t *edge);
