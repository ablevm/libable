typedef struct {
	atomic_bool sl;
	able_port_t *p;
	uint32_t i;
	void *n;
} able_link_t;

int
able_link_send_hold(able_link_t *link, uint16_t size, void **data);

int
able_link_send_done(able_link_t *link, uint16_t size);

int
able_link_send(able_link_t *link, const void *data, uint16_t size);

int
able_link_node_post_shim(void *node, const able_edge_t *edge);
