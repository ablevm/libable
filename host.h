typedef struct {
	able_port_t *p;
	able_link_t *l;
	uint32_t pc;
	uint32_t lc;
	able_core_t c;
	void *u;
	uint64_t ts;
} able_host_t;

int
able_host_exec(able_host_t *host);

int
able_host_wait_shim(able_host_t *host, able_edge_t *edge, const struct timespec *time);
