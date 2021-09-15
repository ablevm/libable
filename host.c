#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "port.h"
#include "link.h"
#include "core.h"
#include "host.h"

#define DSU ABLE_CORE_DSU
#define DSO ABLE_CORE_DSO
#define DSI ABLE_CORE_DSI
#define DSD ABLE_CORE_DSD

#define DS0 ABLE_CORE_DSV(&host->c, 1)

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
				pn = DS0;
				if (pn >= host->pc) {
					DS0 = 4;
					break;
				}
				DS0 = able_host_node_wait_shim(host->n, &host->p[pn].e, NULL);
				if (DS0 == 0)
					return -5;
				break;
			}
			case 0x81: { // clip ( a # p - f)
				if (DSU(&host->c, 3))
					return -6;
				uint32_t pn;
				pn = DS0;
				DSD(&host->c);
				uint64_t u;
				u = DS0;
				DSD(&host->c);
				uint64_t a;
				a = DS0;
				if (pn >= host->pc) {
					DS0 = 2;
					break;
				}
				if (a > UINT64_MAX - u) {
					DS0 = 3;
					break;
				}
				if (a >= host->c.bc || a + u > host->c.bc) {
					DS0 = 4;
					break;
				}
				DS0 = able_port_clip(&host->p[pn], host->c.b + a, u);
				break;
			}
			case 0x82: { // recv ( p - a # f)
				if (DSU(&host->c, 1))
					return -6;
				if (DSO(&host->c, 2))
					return -7;
				uint32_t pn;
				pn = DS0;
				if (pn >= host->pc) {
					DS0 = 1;
					break;
				}
				able_port_mesg_t *m;
				m = able_port_recv(&host->p[pn]);
				if (m != NULL) {
					DS0 = m->b - host->c.b;
					DSI(&host->c);
					DS0 = m->bc;
					DSI(&host->c);
					DS0 = 0;
				} else {
					DS0 = 0;
					DSI(&host->c);
					DS0 = 0;
					DSI(&host->c);
					DS0 = -1;
				}
				break;
			}
			case 0x83: { // send ( a # l - f)
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
				if (a >= host->c.bc || a + u > host->c.bc) {
					DS0 = 7;
					break;
				}
				if (u > UINT16_MAX - sizeof(able_port_mesg_t)) {
					DS0 = 8;
					break;
				}
				DS0 = able_host_link_send_shim(host->l[ln], host->c.b + a, u);
				break;
			}
			default:
				return y;
		}
	}

	// should not happen
	return 1;
}
