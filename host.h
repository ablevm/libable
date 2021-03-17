typedef struct {
	void *n;
	able_port_t *p;
	uint32_t pc;
	void **l;
	uint32_t lc;
	able_core_t c;
	uint64_t ts;
} able_host_t;

int
able_host_exec(able_host_t *host);

int
able_host_node_wait_shim(void *node, able_edge_t *edge, const struct timespec *time);

int
able_host_link_send_shim(void *link, void *data, size_t size);
