typedef struct {
	able_edge_t e;
	uint8_t *r;
	size_t rc;
} able_port_t;

typedef struct {
	uint16_t sc;
	uint16_t bc;
	uint32_t i;
	uint8_t b[0];
} able_port_mesg_t;

int
able_port_clip(able_port_t *port, void *data, size_t size);

able_port_mesg_t *
able_port_recv(able_port_t *port);

int
able_port_send_hold(able_port_t *port, uint16_t size, void **data, uint32_t mark);

int
able_port_send_done(able_port_t *port, uint16_t size);

int
able_port_send(able_port_t *port, void *data, uint16_t size, uint32_t mark);
