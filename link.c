#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "port.h"
#include "link.h"
#include <stdbool.h>
#include <string.h>

int
able_link_send_long(able_link_t *link, uint16_t size, void **data) {
	if (size == 0)
		return 1;
	bool z;
	z = 0;
	if (!atomic_compare_exchange_weak(&link->sl, &z, 1))
		return -3;
	if (link->p == NULL) {
		atomic_store(&link->sl, 0);
		return 3;
	}
	int y;
	y = able_port_send_long(link->p, size, data, link->i);
	if (y != 0)
		atomic_store(&link->sl, 0);
	return y;
}

int
able_link_send_done(able_link_t *link, uint16_t size) {
	able_port_t *p;
	p = link->p;
	if (size == 0) {
		int y;
		y = able_port_send_done(p, 0);
		atomic_store(&link->sl, 0);
		return y;
	}
	void *n;
	n = link->n;
	int y;
	y = able_port_send_done(p, size);
	if (y != 0) {
		atomic_store(&link->sl, 0);
		return y;
	}
	atomic_store(&link->sl, 0);
	return able_link_node_post_shim(n, &p->e);
}

int
able_link_send(able_link_t *link, const void *data, uint16_t size) {
	void *mb;
	int y;
	y = able_link_send_long(link, size, &mb);
	if (y != 0)
		return y;
	memcpy(mb, data, size);
	return able_link_send_done(link, size);
}
