typedef struct {
	atomic_int sl;
	uint8_t *s;
	size_t sc;
	atomic_size_t rc;
	size_t tc;
} able_edge_t;

int
able_edge_clip(able_edge_t *edge, void *data, size_t size);

size_t
able_edge_recv(able_edge_t *edge);

int
able_edge_send_long(able_edge_t *edge, size_t size, void **data);

int
able_edge_send_done(able_edge_t *edge, size_t size);

int
able_edge_send(able_edge_t *edge, const void *data, size_t size);
