typedef struct {
	able_port_t *p;
	uint32_t i;
	void *u;
	able_link_t *l;
} able_wire_t;

void
able_wire_bind(able_wire_t *wire, able_port_t *port, uint32_t mark, void *user);

int
able_wire_join(able_wire_t *wire, able_link_t *link);
