typedef struct {
	uint8_t *b;
	size_t bc;
	uint64_t p;
	uint8_t i;
	uint8_t dp;
	int64_t d0;
	int64_t d1;
	uint64_t d[32];
	uint8_t cp;
	uint64_t c0;
	uint64_t c[32];
	uint64_t r[32];
	uint64_t ts;
} able_core_t;

int
able_core_exec(able_core_t *core);

#define ABLE_CORE_DP_MAX 33
#define ABLE_CORE_CP_MAX 32
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
	((C)->dp + (N) > ABLE_CORE_DP_MAX)

#define ABLE_CORE_DSI(C) \
	(C)->d[(C)->dp] = (C)->d1; \
	(C)->dp = (C)->dp + 1; \
	(C)->d1 = (C)->d0;

#define ABLE_CORE_DSD(C) \
	(C)->d0 = (C)->d1; \
	(C)->dp = (C)->dp - 1; \
	(C)->d1 = (C)->d[(C)->dp];

#define ABLE_CORE_CSU(C, N) \
	((C)->cp < (N))

#define ABLE_CORE_CSO(C, N) \
	((C)->cp + (N) > ABLE_CORE_CP_MAX)

#define ABLE_CORE_CSI(C) \
	(C)->c[(C)->cp] = (C)->c0; \
	(C)->cp = (C)->cp + 1;

#define ABLE_CORE_CSD(C) \
	(C)->cp = (C)->cp - 1; \
	(C)->c0 = (C)->c[(C)->cp];
