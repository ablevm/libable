#include <stdatomic.h>
#include "edge.h"
#include <string.h>

int
able_edge_clip(able_edge_t *edge, void *data, size_t size) {
	if (atomic_load(&edge->rc) > 0)
		return 1;
	int v;
	v = 0;
	while (!atomic_compare_exchange_weak(&edge->sl, &v, -1)) {
		if (v == 1)
			return -1;
		v = 0;
	}
	if (atomic_load(&edge->rc) > 0) {
		atomic_store(&edge->sl, 0);
		return 1;
	}
	edge->s = data;
	edge->sc = size;
	atomic_store(&edge->sl, 0);
	return 0;
}

size_t
able_edge_recv(able_edge_t *edge) {
	return atomic_exchange(&edge->rc, 0);
}

int
able_edge_send_long(able_edge_t *edge, size_t size, void **data) {
	if (size == 0)
		return 1;
	int v;
	v = 0;
	while (!atomic_compare_exchange_weak(&edge->sl, &v, -1)) {
		if (v == 1)
			return -1;
		v = 0;
	}
	if (edge->sc < size) {
		atomic_store(&edge->sl, 0);
		return -2;
	}
	atomic_store(&edge->sl, 1);
	edge->tc = size;
	*data = edge->s;
	return 0;
}

int
able_edge_send_done(able_edge_t *edge, size_t size) {
	if (size == 0) {
		atomic_store(&edge->sl, 0);
		return 0;
	}
	if (size > edge->tc) {
		atomic_store(&edge->sl, 0);
		return 2;
	}
	edge->s += size;
	edge->sc -= size;
	atomic_fetch_add(&edge->rc, size);
	atomic_store(&edge->sl, 0);
	return 0;
}

int
able_edge_send(able_edge_t *edge, const void *data, size_t size) {
	void *m;
	int y;
	y = able_edge_send_long(edge, size, &m);
	if (y != 0)
		return y;
	memcpy(m, data, size);
	return able_edge_send_done(edge, size);
}
