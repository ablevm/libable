#include <stdatomic.h>
#include "edge.h"
#include <pthread.h>
#include "node.h"
#include "port.h"
#include "link.h"
#include "core.h"
#include "host.h"

#define ICD(C) \
	(C)->d[(C)->dp] = (C)->d1; \
	(C)->dp = (C)->dp + 1 & 31; \
	(C)->d1 = (C)->d0;

#define DCD(C) \
	(C)->d0 = (C)->d1; \
	(C)->dp = (C)->dp - 1 & 31; \
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
				uint32_t pn;
				pn = host->c.d0;
				if (pn >= host->pc) {
					host->c.d0 = 4;
					break;
				}
				host->c.d0 = able_host_wait_shim(host, &host->p[pn].e, NULL);
				if (host->c.d0 == 0)
					return -5;
				break;
			}
			case 0x81: { // clip ( a # p - f)
				uint32_t pn;
				pn = host->c.d0;
				DCD(&host->c);
				uint64_t u;
				u = host->c.d0;
				DCD(&host->c);
				uint64_t a;
				a = host->c.d0;
				if (pn >= host->pc) {
					host->c.d0 = 2;
					break;
				}
				if (a > 0 && (a > UINT64_MAX - u)) {
					host->c.d0 = 3;
					break;
				}
				if (a + u > host->c.bc) {
					host->c.d0 = 4;
					break;
				}
				host->c.d0 = able_port_clip(&host->p[pn], host->c.b + a, u);
				break;
			}
			case 0x82: { // recv ( p - a # f)
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
					ICD(&host->c);
					host->c.d0 = m->bc;
					ICD(&host->c);
					host->c.d0 = 0;
				} else {
					host->c.d0 = 0;
					ICD(&host->c);
					host->c.d0 = 0;
					ICD(&host->c);
					host->c.d0 = -1;
				}
				break;
			}
			case 0x83: { // send ( a # l - f)
				uint32_t ln;
				ln = host->c.d0;
				DCD(&host->c);
				uint16_t u;
				u = host->c.d0;
				DCD(&host->c);
				uint64_t a;
				a = host->c.d0;
				if (ln >= host->lc) {
					host->c.d0 = 5;
					break;
				}
				if (a > 0 && (a > UINT64_MAX - u)) {
					host->c.d0 = 6;
					break;
				}
				if (a + u > host->c.bc) {
					host->c.d0 = 7;
					break;
				}
				if (u > UINT16_MAX - sizeof(able_port_mesg_t)) {
					host->c.d0 = 8;
					break;
				}
				host->c.d0 = able_link_send(&host->l[ln], host->c.b + a, u);
				break;
			}
			default:
				return y;
		}
	}

	// should not happen
	return 1;
}
