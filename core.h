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
