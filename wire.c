#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "port.h"
#include "link.h"
#include "wire.h"
#include <stdbool.h>

void
able_wire_bind(able_wire_t *wire, able_port_t *port, uint32_t mark, void *node) {
	wire->p = port;
	wire->i = mark;
	wire->n = node;
}

int
able_wire_join(able_wire_t *wire, able_link_t *link) {
	bool z;
	z = 0;
	if (!atomic_compare_exchange_weak(&link->sl, &z, 1))
		return -3;
	link->p = wire->p;
	link->i = wire->i;
	link->n = wire->n;
	atomic_store(&link->sl, 0);
	wire->l = link;
	return 0;
}
