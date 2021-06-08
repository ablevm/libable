typedef struct {
	uint8_t *b;
	size_t bc;
	uint64_t p;
	uint8_t i;
	int64_t *d;
	uint16_t dc;
	uint16_t dp;
	uint64_t *c;
	uint16_t cc;
	uint16_t cp;
	uint64_t r[32];
	uint64_t ts;
} able_core_t;

int
able_core_exec(able_core_t *core);

#define ABLE_CORE_RI_MAX 31

#define ABLE_CORE_A(S, V) \
	(((V) + ((S) - 1)) & -(S))

#define ABLE_CORE_F(M, A, B) \
	(((A) & ~(M)) | (B))

#define ABLE_CORE_V(T, E, A) \
	(*(T *)((E) + (A)))

#define ABLE_CORE_G(M, E, S) \
	((E) > UINT64_MAX - (S) || (E) + (S) > (M))

#define ABLE_CORE_DSU(C, N) \
	((C)->dp < (N))

#define ABLE_CORE_DSO(C, N) \
	((C)->dp + (N) > (C)->dc)

#define ABLE_CORE_DSI(C) \
	(C)->dp++;

#define ABLE_CORE_DSD(C) \
	(C)->dp--;

#define ABLE_CORE_DSV(C, N) \
	(C)->d[(C)->dp - N]

#define ABLE_CORE_CSU(C, N) \
	((C)->cp < (N))

#define ABLE_CORE_CSO(C, N) \
	((C)->cp + (N) > (C)->cc)

#define ABLE_CORE_CSI(C) \
	(C)->cp++;

#define ABLE_CORE_CSD(C) \
	(C)->cp--;

#define ABLE_CORE_CSV(C, N) \
	(C)->c[(C)->cp - N]
