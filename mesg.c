#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "link.h"
#include "mesg.h"
#include <string.h>

int
able_mesg_link_send_long(able_link_t *link, uint16_t size, void **data) {
	size_t sc;
	sc = ABLE_MESG_SIZE(able_mesg_t, size);
	if (sc > UINT16_MAX)
		return 4;
	able_mesg_t *m;
	int y;
	y = able_link_send_long(link, sc, (void **)&m);
	if (y != 0)
		return y;
	m->i = link->i;
	*data = m->b;
	return 0;
}

int
able_mesg_link_send_done(able_link_t *link, uint16_t size) {
	size_t sc;
	sc = ABLE_MESG_SIZE(able_mesg_t, size);
	if (sc > UINT16_MAX)
		return 4;
	able_mesg_t *m;
	m = (able_mesg_t *)link->e->s;
	m->sc = sc;
	m->bc = size;
	return able_link_send_done(link, sc);
}

int
able_mesg_link_send(able_link_t *link, const void *data, uint16_t size) {
	void *mb;
	int y;
	y = able_mesg_link_send_long(link, size, &mb);
	if (y != 0)
		return y;
	memcpy(mb, data, size);
	return able_mesg_link_send_done(link, size);
}
