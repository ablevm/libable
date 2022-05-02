#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "link.h"
#include <string.h>

int
able_link_join(able_link_t *link, able_edge_t *edge, uintptr_t mark, void *node) {
	int v;
	v = 0;
	while (!atomic_compare_exchange_weak(&link->sl, &v, -1)) {
		if (v == 1)
			return -1;
		v = 0;
	}
	link->e = edge;
	link->i = mark;
	link->n = node;
	atomic_store(&link->sl, 0);
	return 0;
}

int
able_link_send_long(able_link_t *link, size_t size, void **data) {
	if (size == 0)
		return 1;
	int v;
	v = 0;
	while (!atomic_compare_exchange_weak(&link->sl, &v, -1)) {
		if (v == 1)
			return -3;
		v = 0;
	}
	if (link->e == NULL) {
		atomic_store(&link->sl, 0);
		return 3;
	}
	int y;
	y = able_edge_send_long(link->e, size, data);
	if (y != 0) {
		atomic_store(&link->sl, 0);
		return y;
	}
	atomic_store(&link->sl, 1);
	return 0;
}

int
able_link_send_done(able_link_t *link, size_t size) {
	able_edge_t *e;
	e = link->e;
	int y;
	y = able_edge_send_done(e, size);
	if (y != 0) {
		atomic_store(&link->sl, 0);
		return y;
	}
	void *n;
	n = link->n;
	atomic_store(&link->sl, 0);
	return able_link_node_post_shim(n, e);
}

int
able_link_send(able_link_t *link, const void *data, size_t size) {
	void *mb;
	int y;
	y = able_link_send_long(link, size, &mb);
	if (y != 0)
		return y;
	memcpy(mb, data, size);
	return able_link_send_done(link, size);
}
