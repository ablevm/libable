#include <stdatomic.h>
#include "edge.h"
#include "port.h"
#include <string.h>

#define A(S, V) \
	(((V) + ((S) - 1)) & -(S))

int
able_port_clip(able_port_t *port, void *data, size_t size) {
	if (port->rc > 0)
		return 1;
	int y;
	y = able_edge_clip(&port->e, data, size);
	if (y != 0)
		return y;
	port->r = data;
	return 0;
}

able_port_mesg_t *
able_port_recv(able_port_t *port) {
	if (port->rc == 0) {
		size_t n;
		n = able_edge_recv(&port->e);
		if (n == 0)
			return NULL;
		port->rc = n;
	}
	able_port_mesg_t *m;
	m = (able_port_mesg_t *)port->r;
	port->r += m->sc;
	port->rc -= m->sc;
	return m;
}

int
able_port_send_long(able_port_t *port, uint16_t size, void **data, uint32_t mark) {
	if (size == 0)
		return 1;
	uint16_t sc;
	sc = A(sizeof(uint64_t), sizeof(able_port_mesg_t) + size);
	able_port_mesg_t *m;
	int y;
	y = able_edge_send_long(&port->e, sc, (void **)&m);
	if (y != 0)
		return y;
	m->i = mark;
	*data = m->b;
	return 0;
}

int
able_port_send_done(able_port_t *port, uint16_t size) {
	if (size == 0)
		return able_edge_send_done(&port->e, 0);
	uint16_t sc;
	sc = A(sizeof(uint64_t), sizeof(able_port_mesg_t) + size);
	able_port_mesg_t *m;
	m = (able_port_mesg_t *)port->e.s;
	m->sc = sc;
	m->bc = size;
	return able_edge_send_done(&port->e, sc);
}

int
able_port_send(able_port_t *port, void *data, uint16_t size, uint32_t mark) {
	void *mb;
	int y;
	y = able_port_send_long(port, size, &mb, mark);
	if (y != 0)
		return y;
	memcpy(mb, data, size);
	return able_port_send_done(port, size);
}
