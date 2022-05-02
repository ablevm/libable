typedef struct {
	uint8_t *r;
	size_t rc;
} able_misc_host_buff_t;

typedef struct {
	void *n;
	able_edge_t *e;
	uint32_t ec;
	able_misc_host_buff_t *b;
	uint32_t bc;
	void **l;
	uint32_t lc;
	able_misc_core_t c;
	uint64_t ts;
} able_misc_host_t;

int
able_misc_host_exec(able_misc_host_t *host);

int
able_misc_host_node_wait_shim(void *node, const able_edge_t *edge, const struct timespec *time);

int
able_misc_host_link_send_shim(void *link, const void *data, size_t size);
