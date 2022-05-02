#define ABLE_MESG_SIZE(H, B) \
	(((sizeof(H) + (B)) + ((sizeof(uint64_t)) - 1)) & -(sizeof(uint64_t)))

typedef struct {
	uint16_t sc;
	uint16_t bc;
	uint32_t i;
	uint8_t b[0];
} able_mesg_t;

int
able_mesg_link_send_long(able_link_t *link, uint16_t size, void **data);

int
able_mesg_link_send_done(able_link_t *link, uint16_t size);

int
able_mesg_link_send(able_link_t *link, const void *data, uint16_t size);
