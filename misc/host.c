#include <stdatomic.h>
#include <pthread.h>
#include <able/able.h>
#include "core.h"
#include "host.h"

#define DSU ABLE_MISC_CORE_DSU
#define DSO ABLE_MISC_CORE_DSO
#define DSI ABLE_MISC_CORE_DSI
#define DSD ABLE_MISC_CORE_DSD

#define DS0 ABLE_MISC_CORE_DSV(&host->c, 1)

int
able_misc_host_exec(able_misc_host_t *host) {
	host->c.ts = host->ts;
	for (;;) {
		int y;
		y = able_misc_core_exec(&host->c);
		if (y != -4) {
			return y;
		}
		switch (host->c.i) {
			case 0x80: { // wait ( t p - n)
				if (DSU(&host->c, 2))
					return -6;
				uint32_t en;
				en = DS0;
				DSD(&host->c);
				if (en >= host->ec) {
					DS0 = 4;
					break;
				}
				struct timespec ts;
				struct timespec *tp;
				tp = NULL;
				uint64_t tv;
				tv = DS0;
				if (tv != -1) {
					ts.tv_sec = tv / 1000000000;
					ts.tv_nsec = tv % 1000000000;
					tp = &ts;
				}
				DS0 = able_misc_host_node_wait_shim(host->n, &host->e[en], tp);
				if (DS0 == 0)
					return -5;
				break;
			}
			case 0x81: { // clip ( a # p - n)
				if (DSU(&host->c, 3))
					return -6;
				uint32_t en;
				en = DS0;
				DSD(&host->c);
				uint64_t u;
				u = DS0;
				DSD(&host->c);
				uint64_t a;
				a = DS0;
				if (en >= host->ec) {
					DS0 = 2;
					break;
				}
				if (a > UINT64_MAX - u) {
					DS0 = 3;
					break;
				}
				if (a >= host->c.mc || a + u > host->c.mc) {
					DS0 = 4;
					break;
				}
				able_misc_host_buff_t *b;
				b = &host->b[en];
				if (b->rc > 0) {
					DS0 = 1;
					break;
				}
				uint8_t *s;
				s = host->c.m + a;
				int y;
				y = able_edge_clip(&host->e[en], s, u);
				if (y == 0)
					b->r = s;
				DS0 = y;
				break;
			}
			case 0x82: { // recv ( p - a # n)
				if (DSU(&host->c, 1))
					return -6;
				if (DSO(&host->c, 2))
					return -7;
				uint32_t en;
				en = DS0;
				if (en >= host->ec) {
					DS0 = 1;
					break;
				}
				able_misc_host_buff_t *b;
				b = &host->b[en];
				if (b->rc == 0)
					b->rc += able_edge_recv(&host->e[en]);
				able_mesg_t *m;
				if (b->rc < sizeof(able_mesg_t)) {
					DS0 = 0;
					DSI(&host->c);
					DS0 = 0;
					DSI(&host->c);
					DS0 = -1;
					break;
				}
				m = (able_mesg_t *)b->r;
				b->r += m->sc;
				b->rc -= m->sc;
				DS0 = m->b - host->c.m;
				DSI(&host->c);
				DS0 = m->bc;
				DSI(&host->c);
				DS0 = 0;
				break;
			}
			case 0x83: { // send ( a # l - n)
				if (DSU(&host->c, 3))
					return -6;
				uint32_t ln;
				ln = DS0;
				DSD(&host->c);
				uint16_t u;
				u = DS0;
				DSD(&host->c);
				uint64_t a;
				a = DS0;
				if (ln >= host->lc) {
					DS0 = 5;
					break;
				}
				if (a > UINT64_MAX - u) {
					DS0 = 6;
					break;
				}
				if (a >= host->c.mc || a + u > host->c.mc) {
					DS0 = 7;
					break;
				}
				DS0 = able_misc_host_link_send_shim(host->l[ln], host->c.m + a, u);
				break;
			}
			default:
				return y;
		}
	}

	// should not happen
	return 1;
}
