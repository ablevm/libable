#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "port.h"
#include "link.h"
#include "core.h"
#include "host.h"

#define DP_MAX 33
#define CP_MAX 32

#define DSU(C, N) \
	((C)->dp < (N))

#define DSO(C, N) \
	((C)->dp + (N) > DP_MAX)

#define DSI(C) \
	(C)->d[(C)->dp] = (C)->d1; \
	(C)->dp = (C)->dp + 1; \
	(C)->d1 = (C)->d0;

#define DSD(C) \
	(C)->d0 = (C)->d1; \
	(C)->dp = (C)->dp - 1; \
	(C)->d1 = (C)->d[(C)->dp];

int
able_host_exec(able_host_t *host) {
	host->c.ts = host->ts;
	for (;;) {
		int y;
		y = able_core_exec(&host->c);
		if (y != -4) {
			return y;
		}
		switch (host->c.i) {
			case 0x80: { // wait ( p - f)
				if (DSU(&host->c, 1))
					return -6;
				uint32_t pn;
				pn = host->c.d0;
				if (pn >= host->pc) {
					host->c.d0 = 4;
					break;
				}
				host->c.d0 = able_host_node_wait_shim(host->n, &host->p[pn].e, NULL);
				if (host->c.d0 == 0)
					return -5;
				break;
			}
			case 0x81: { // clip ( a # p - f)
				if (DSU(&host->c, 3))
					return -6;
				uint32_t pn;
				pn = host->c.d0;
				DSD(&host->c);
				uint64_t u;
				u = host->c.d0;
				DSD(&host->c);
				uint64_t a;
				a = host->c.d0;
				if (pn >= host->pc) {
					host->c.d0 = 2;
					break;
				}
				if (a > UINT64_MAX - u) {
					host->c.d0 = 3;
					break;
				}
				if (a >= host->c.bc || a + u > host->c.bc) {
					host->c.d0 = 4;
					break;
				}
				host->c.d0 = able_port_clip(&host->p[pn], host->c.b + a, u);
				break;
			}
			case 0x82: { // recv ( p - a # f)
				if (DSU(&host->c, 1))
					return -6;
				if (DSO(&host->c, 2))
					return -7;
				uint32_t pn;
				pn = host->c.d0;
				if (pn >= host->pc) {
					host->c.d0 = 1;
					break;
				}
				able_port_mesg_t *m;
				m = able_port_recv(&host->p[pn]);
				if (m != NULL) {
					host->c.d0 = m->b - host->c.b;
					DSI(&host->c);
					host->c.d0 = m->bc;
					DSI(&host->c);
					host->c.d0 = 0;
				} else {
					host->c.d0 = 0;
					DSI(&host->c);
					host->c.d0 = 0;
					DSI(&host->c);
					host->c.d0 = -1;
				}
				break;
			}
			case 0x83: { // send ( a # l - f)
				if (DSU(&host->c, 3))
					return -6;
				uint32_t ln;
				ln = host->c.d0;
				DSD(&host->c);
				uint16_t u;
				u = host->c.d0;
				DSD(&host->c);
				uint64_t a;
				a = host->c.d0;
				if (ln >= host->lc) {
					host->c.d0 = 5;
					break;
				}
				if (a > UINT64_MAX - u) {
					host->c.d0 = 6;
					break;
				}
				if (a >= host->c.bc || a + u > host->c.bc) {
					host->c.d0 = 7;
					break;
				}
				if (u > UINT16_MAX - sizeof(able_port_mesg_t)) {
					host->c.d0 = 8;
					break;
				}
				host->c.d0 = able_host_link_send_shim(host->l[ln], host->c.b + a, u);
				break;
			}
			default:
				return y;
		}
	}

	// should not happen
	return 1;
}
