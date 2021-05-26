#include <stdint.h>
#include <stddef.h>
#include "core.h"

#define A ABLE_CORE_A
#define F ABLE_CORE_F
#define V ABLE_CORE_V
#define G ABLE_CORE_G
#define DSU ABLE_CORE_DSU
#define DSO ABLE_CORE_DSO
#define DSI ABLE_CORE_DSI
#define DSD ABLE_CORE_DSD
#define CSU ABLE_CORE_CSU
#define CSO ABLE_CORE_CSO
#define CSI ABLE_CORE_CSI
#define CSD ABLE_CORE_CSD

#define DS0 ABLE_CORE_DSV(core, 1)
#define DS1 ABLE_CORE_DSV(core, 2)
#define DS2 ABLE_CORE_DSV(core, 3)
#define CS0 ABLE_CORE_CSV(core, 1)

// ;
#define INSTX0(S) \
	if (CSU(core, 1)) \
		return -6; \
	core->p = F(UINT##S##_MAX, core->p, (uint##S##_t)CS0); \
	CSD(core);

// ex
#define INSTX1(S) { \
		if (CSU(core, 1)) \
			return -6; \
		uint##S##_t t; \
		t = CS0; \
		CS0 = core->p; \
		core->p = F(UINT##S##_MAX, core->p, t); \
	}

// name ;
#define INSTX2(S) { \
		uint64_t p; \
		p = A(sizeof(uint##S##_t), core->p); \
		if (G(core->bc, p, sizeof(uint##S##_t))) \
			return -2; \
		uint##S##_t i; \
		i = V(uint##S##_t, core->b, p); \
		core->p = F(UINT##S##_MAX, p, i); \
	}

// name
#define INSTX3(S) { \
		if (CSO(core, 1)) \
			return -7; \
		uint64_t p; \
		p = A(sizeof(uint##S##_t), core->p); \
		if (G(core->bc, p, sizeof(uint##S##_t))) \
			return -2; \
		uint##S##_t i; \
		i = V(uint##S##_t, core->b, p); \
		CSI(core); \
		CS0 = p + sizeof(uint##S##_t); \
		core->p = F(UINT##S##_MAX, p, i); \
	}

// if
#define INSTX4(S) \
	if (DSU(core, 1)) \
		return -6; \
	if (DS0 == 0) { \
		uint64_t p; \
		p = A(sizeof(uint##S##_t), core->p); \
		if (G(core->bc, p, sizeof(uint##S##_t))) \
			return -2; \
		uint##S##_t i; \
		i = V(uint##S##_t, core->b, p); \
		core->p = F(UINT##S##_MAX, p, i); \
	} else \
		core->p = A(sizeof(uint##S##_t), core->p) + sizeof(uint##S##_t); \
	DSD(core);

// -if
#define INSTX5(S) \
	if (DSU(core, 1)) \
		return -6; \
	if (DS0 < 0) { \
		uint64_t p; \
		p = A(sizeof(uint##S##_t), core->p); \
		if (G(core->bc, p, sizeof(uint##S##_t))) \
			return -2; \
		uint##S##_t i; \
		i = V(uint##S##_t, core->b, p); \
		core->p = F(UINT##S##_MAX, p, i); \
	} else \
		core->p = A(sizeof(uint##S##_t), core->p) + sizeof(uint##S##_t); \
	DSD(core);

// next
#define INSTX6(S) \
	if (CSU(core, 1)) \
		return -6; \
	if (CS0 != 0) { \
		uint64_t p; \
		p = A(sizeof(uint##S##_t), core->p); \
		if (G(core->bc, p, sizeof(uint##S##_t))) \
			return -2; \
		CS0--; \
		uint##S##_t i; \
		i = V(uint##S##_t, core->b, p); \
		core->p = F(UINT##S##_MAX, p, i); \
	} else { \
		CSD(core); \
		core->p = A(sizeof(uint##S##_t), core->p) + sizeof(uint##S##_t); \
	}

// lit
#define INSTX7(S) { \
		if (DSO(core, 1)) \
			return -7; \
		uint64_t p; \
		p = A(sizeof(int##S##_t), core->p); \
		if (G(core->bc, p, sizeof(int##S##_t))) \
			return -2; \
		DSI(core); \
		DS0 = V(int##S##_t, core->b, p); \
		core->p = p + sizeof(int##S##_t); \
	}

// r!
#define INSTX8(S) { \
		if (DSU(core, 1)) \
			return -6; \
		if (G(core->bc, core->p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, core->p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		core->r[i] = F(UINT##S##_MAX, core->p, (uint##S##_t)DS0); \
		core->p++; \
		DSD(core); \
	}

// r@
#define INSTX9(S) { \
		if (DSO(core, 1)) \
			return -7; \
		if (G(core->bc, core->p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, core->p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		core->p++; \
		DSI(core); \
		DS0 = (int##S##_t)core->r[i]; \
	}

// @r
#define INSTXA(S) { \
		if (DSO(core, 1)) \
			return -7; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		DSI(core); \
		DS0 = V(int##S##_t, core->b, a); \
		core->r[i] = b; \
		core->p = p; \
	}

// !r
#define INSTXB(S) { \
		if (DSU(core, 1)) \
			return -6; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		V(int##S##_t, core->b, a) = DS0; \
		DSD(core); \
		core->r[i] = b; \
		core->p = p; \
	}

// @r+
#define INSTXC(S) { \
		if (DSO(core, 1)) \
			return -7; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		DSI(core); \
		DS0 = V(int##S##_t, core->b, a); \
		core->r[i] = b + sizeof(int##S##_t); \
		core->p = p; \
	}

// !r+
#define INSTXD(S) { \
		if (DSU(core, 1)) \
			return -6; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		V(int##S##_t, core->b, a) = DS0; \
		DSD(core); \
		core->r[i] = b + sizeof(int##S##_t); \
		core->p = p; \
	}

// -@r
#define INSTXE(S) { \
		if (DSO(core, 1)) \
			return -7; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]) - sizeof(int##S##_t); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		DSI(core); \
		DS0 = V(int##S##_t, core->b, a); \
		core->r[i] = b; \
		core->p = p; \
	}

// -!r
#define INSTXF(S) { \
		if (DSU(core, 1)) \
			return -6; \
		uint64_t p; \
		p = core->p; \
		if (G(core->bc, p, sizeof(uint8_t))) \
			return -2; \
		uint8_t i; \
		i = V(uint8_t, core->b, p); \
		if (i > ABLE_CORE_RI_MAX) \
			return -8; \
		p++; \
		uint64_t b; \
		b = A(sizeof(int##S##_t), core->r[i]) - sizeof(int##S##_t); \
		uint64_t a; \
		a = A(sizeof(int##S##_t), b + core->r[i ^ 1]); \
		if (G(core->bc, a, sizeof(int##S##_t))) \
			return -2; \
		V(int##S##_t, core->b, a) = DS0; \
		DSD(core); \
		core->r[i] = b; \
		core->p = p; \
	}

// ;
#define INST00 INSTX0(32)
// ex
#define INST01 INSTX1(32)
// name ;
#define INST02 INSTX2(32)
// name
#define INST03 INSTX3(32)
// if
#define INST04 INSTX4(32)
// -if
#define INST05 INSTX5(32)
// next
#define INST06 INSTX6(32)
// lit
#define INST07 INSTX7(32)
// r!
#define INST08 INSTX8(32)
// r@
#define INST09 INSTX9(32)
// @r
#define INST0A INSTXA(32)
// !r
#define INST0B INSTXB(32)
// @r+
#define INST0C INSTXC(32)
// !r+
#define INST0D INSTXD(32)
// @r-
#define INST0E INSTXE(32)
// !r-
#define INST0F INSTXF(32)

// push
#define INST10 \
	if (DSU(core, 1)) \
		return -6; \
	if (CSO(core, 1)) \
		return -7; \
	CSI(core); \
	CS0 = DS0; \
	DSD(core);
// pop
#define INST11 \
	if (CSU(core, 1)) \
		return -6; \
	if (DSO(core, 1)) \
		return -7; \
	DSI(core); \
	DS0 = CS0; \
	CSD(core);
// lshift
#define INST12 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 << DS0; \
	DSD(core);
// ashift
#define INST13 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 >> DS0; \
	DSD(core);
// not
#define INST14 \
	if (DSU(core, 1)) \
		return -6; \
	DS0 = ~DS0;
// and
#define INST15 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 & DS0; \
	DSD(core);
// or
#define INST16 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 | DS0; \
	DSD(core);
// xor
#define INST17 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 ^ DS0; \
	DSD(core);
// +
#define INST18 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 + DS0; \
	DSD(core);
// -
#define INST19 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 - DS0; \
	DSD(core);
// *
#define INST1A \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS1 * DS0; \
	DSD(core);
// /mod
#define INST1B { \
		if (DSU(core, 2)) \
			return -6; \
		if (DS0 == 0) \
			return -3; \
		int64_t t0, t1; \
		t0 = DS0; \
		t1 = DS1; \
		DS1 = t1 % t0; \
		DS0 = t1 / t0; \
	}
// drop
#define INST1C \
	if (DSU(core, 1)) \
		return -6; \
	DSD(core);
// dup
#define INST1D \
	if (DSU(core, 1)) \
		return -6; \
	if (DSO(core, 1)) \
		return -7; \
	DSI(core); \
	DS0 = DS1;
// over
#define INST1E { \
		if (DSU(core, 2)) \
			return -6; \
		if (DSO(core, 1)) \
			return -7; \
		int64_t t; \
		t = DS1; \
		DSI(core); \
		DS0 = t; \
	}
// swap
#define INST1F { \
		if (DSU(core, 2)) \
			return -6; \
		int64_t t; \
		t = DS0; \
		DS0 = DS1; \
		DS1 = t; \
	}

// ;
#define INST20 INSTX0(8)
// ex
#define INST21 INSTX1(8)
// name ;
#define INST22 INSTX2(8)
// name
#define INST23 INSTX3(8)
// if
#define INST24 INSTX4(8)
// -if
#define INST25 INSTX5(8)
// next
#define INST26 INSTX6(8)
// lit
#define INST27 INSTX7(8)
// r!
#define INST28 INSTX8(8)
// r@
#define INST29 INSTX9(8)
// @r
#define INST2A INSTXA(8)
// !r
#define INST2B INSTXB(8)
// @r+
#define INST2C INSTXC(8)
// !r+
#define INST2D INSTXD(8)
// @r-
#define INST2E INSTXE(8)
// !r-
#define INST2F INSTXF(8)

// ;
#define INST30 INSTX0(16)
// ex
#define INST31 INSTX1(16)
// name ;
#define INST32 INSTX6(16)
// name
#define INST33 INSTX3(16)
// if
#define INST34 INSTX4(16)
// -if
#define INST35 INSTX5(16)
// next
#define INST36 INSTX6(16)
// lit
#define INST37 INSTX7(16)
// r!
#define INST38 INSTX8(16)
// r@
#define INST39 INSTX9(16)
// @r
#define INST3A INSTXA(16)
// !r
#define INST3B INSTXB(16)
// @r+
#define INST3C INSTXC(16)
// !r+
#define INST3D INSTXD(16)
// @r-
#define INST3E INSTXE(16)
// !r-
#define INST3F INSTXF(16)

// ;
#define INST40 INSTX0(64)
// ex
#define INST41 INSTX1(64)
// name ;
#define INST42 INSTX2(64)
// name
#define INST43 INSTX3(64)
// if
#define INST44 INSTX4(64)
// -if
#define INST45 INSTX5(64)
// next
#define INST46 INSTX6(64)
// lit
#define INST47 INSTX7(64)
// r!
#define INST48 INSTX8(64)
// r@
#define INST49 INSTX9(64)
// @r
#define INST4A INSTXA(64)
// !r
#define INST4B INSTXB(64)
// @r+
#define INST4C INSTXC(64)
// !r+
#define INST4D INSTXD(64)
// @r-
#define INST4E INSTXE(64)
// !r-
#define INST4F INSTXF(64)

// i@
#define INST50 \
	if (CSU(core, 1)) \
		return -6; \
	if (DSO(core, 1)) \
		return -7; \
	DSI(core); \
	DS0 = CS0;
// i!
#define INST51 \
	if (DSU(core, 1)) \
		return -6; \
	if (CSU(core, 1)) \
		return -6; \
	CS0 = DS0; \
	DSD(core);
// =
#define INST52 \
	if (DSU(core, 2)) \
		return -6; \
	if (DS1 == DS0) \
		DS1 = -1; \
	else \
		DS1 = 0; \
	DSD(core);
// <
#define INST53 \
	if (DSU(core, 2)) \
		return -6; \
	if (DS1 < DS0) \
		DS1 = -1; \
	else \
		DS1 = 0; \
	DSD(core);
// negate
#define INST54 \
	if (DSU(core, 1)) \
		return -6; \
	DS0 = -DS0;
// abs
#define INST55 \
	if (DSU(core, 1)) \
		return -6; \
	if (DS0 < 0) \
		DS0 = -DS0;
// min
#define INST56 \
	if (DSU(core, 2)) \
		return -6; \
	if (DS0 < DS1) \
		DS1 = DS0; \
	DSD(core);
// max
#define INST57 \
	if (DSU(core, 2)) \
		return -6; \
	if (DS0 > DS1) \
		DS1 = DS0; \
	DSD(core);
// rshift
#define INST58 \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = (uint64_t)DS1 >> (uint64_t)DS0; \
	DSD(core);
// u<
#define INST59 \
	if (DSU(core, 2)) \
		return -6; \
	if ((uint64_t)DS1 < (uint64_t)DS0) \
		DS1 = -1; \
	else \
		DS1 = 0; \
	DSD(core);
// u*
#define INST5A \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = (uint64_t)DS1 * (uint64_t)DS0; \
	DSD(core);
// u/mod
#define INST5B { \
		if (DSU(core, 2)) \
			return -6; \
		if (DS0 == 0) \
			return -3; \
		uint64_t t0, t1; \
		t0 = DS0; \
		t1 = DS1; \
		DS1 = t1 % t0; \
		DS0 = t1 / t0; \
	}
// nip
#define INST5C \
	if (DSU(core, 2)) \
		return -6; \
	DS1 = DS0; \
	DSD(core);
// tuck
#define INST5D \
	if (DSU(core, 2)) \
		return -6; \
	if (DSO(core, 1)) \
		return -7; \
	DSI(core); \
	DS0 = DS1; \
	DS1 = DS2; \
	DS2 = DS0;
// rot
#define INST5E { \
		if (DSU(core, 3)) \
			return -6; \
		int64_t t; \
		t = DS2; \
		DS2 = DS1; \
		DS1 = DS0; \
		DS0 = t; \
	}
// -rot
#define INST5F { \
		if (DSU(core, 3)) \
			return -6; \
		int64_t t; \
		t = DS0; \
		DS0 = DS1; \
		DS1 = DS2; \
		DS2 = t; \
	}

#define NEXT \
	if (core->ts == 0) \
		return -1; \
	core->ts--; \
	if (G(core->bc, core->p, sizeof(uint8_t))) \
		return -2; \
	core->i = V(uint8_t, core->b, core->p); \
	core->p++; \
	goto *inst[core->i];

int
able_core_exec(able_core_t *core) {
	static const void *inst[] = {
		[0x00] = &&inst00,
		&&inst01,
		&&inst02,
		&&inst03,
		&&inst04,
		&&inst05,
		&&inst06,
		&&inst07,
		&&inst08,
		&&inst09,
		&&inst0a,
		&&inst0b,
		&&inst0c,
		&&inst0d,
		&&inst0e,
		&&inst0f,
		&&inst10,
		&&inst11,
		&&inst12,
		&&inst13,
		&&inst14,
		&&inst15,
		&&inst16,
		&&inst17,
		&&inst18,
		&&inst19,
		&&inst1a,
		&&inst1b,
		&&inst1c,
		&&inst1d,
		&&inst1e,
		&&inst1f,
		&&inst20,
		&&inst21,
		&&inst22,
		&&inst23,
		&&inst24,
		&&inst25,
		&&inst26,
		&&inst27,
		&&inst28,
		&&inst29,
		&&inst2a,
		&&inst2b,
		&&inst2c,
		&&inst2d,
		&&inst2e,
		&&inst2f,
		&&inst30,
		&&inst31,
		&&inst32,
		&&inst33,
		&&inst34,
		&&inst35,
		&&inst36,
		&&inst37,
		&&inst38,
		&&inst39,
		&&inst3a,
		&&inst3b,
		&&inst3c,
		&&inst3d,
		&&inst3e,
		&&inst3f,
		&&inst40,
		&&inst41,
		&&inst42,
		&&inst43,
		&&inst44,
		&&inst45,
		&&inst46,
		&&inst47,
		&&inst48,
		&&inst49,
		&&inst4a,
		&&inst4b,
		&&inst4c,
		&&inst4d,
		&&inst4e,
		&&inst4f,
		&&inst50,
		&&inst51,
		&&inst52,
		&&inst53,
		&&inst54,
		&&inst55,
		&&inst56,
		&&inst57,
		&&inst58,
		&&inst59,
		&&inst5a,
		&&inst5b,
		&&inst5c,
		&&inst5d,
		&&inst5e,
		&&inst5f,
		[0x60 ... 0xff] = &&instxx,
	};
	NEXT;
inst00:
	INST00;
	NEXT;
inst01:
	INST01;
	NEXT;
inst02:
	INST02;
	NEXT;
inst03:
	INST03;
	NEXT;
inst04:
	INST04;
	NEXT;
inst05:
	INST05;
	NEXT;
inst06:
	INST06;
	NEXT;
inst07:
	INST07;
	NEXT;
inst08:
	INST08;
	NEXT;
inst09:
	INST09;
	NEXT;
inst0a:
	INST0A;
	NEXT;
inst0b:
	INST0B;
	NEXT;
inst0c:
	INST0C;
	NEXT;
inst0d:
	INST0D;
	NEXT;
inst0e:
	INST0E;
	NEXT;
inst0f:
	INST0F;
	NEXT;
inst10:
	INST10;
	NEXT;
inst11:
	INST11;
	NEXT;
inst12:
	INST12;
	NEXT;
inst13:
	INST13;
	NEXT;
inst14:
	INST14;
	NEXT;
inst15:
	INST15;
	NEXT;
inst16:
	INST16;
	NEXT;
inst17:
	INST17;
	NEXT;
inst18:
	INST18;
	NEXT;
inst19:
	INST19;
	NEXT;
inst1a:
	INST1A;
	NEXT;
inst1b:
	INST1B;
	NEXT;
inst1c:
	INST1C;
	NEXT;
inst1d:
	INST1D;
	NEXT;
inst1e:
	INST1E;
	NEXT;
inst1f:
	INST1F;
	NEXT;
inst20:
	INST20;
	NEXT;
inst21:
	INST21;
	NEXT;
inst22:
	INST22;
	NEXT;
inst23:
	INST23;
	NEXT;
inst24:
	INST24;
	NEXT;
inst25:
	INST25;
	NEXT;
inst26:
	INST26;
	NEXT;
inst27:
	INST27;
	NEXT;
inst28:
	INST28;
	NEXT;
inst29:
	INST29;
	NEXT;
inst2a:
	INST2A;
	NEXT;
inst2b:
	INST2B;
	NEXT;
inst2c:
	INST2C;
	NEXT;
inst2d:
	INST2D;
	NEXT;
inst2e:
	INST2E;
	NEXT;
inst2f:
	INST2F;
	NEXT;
inst30:
	INST30;
	NEXT;
inst31:
	INST31;
	NEXT;
inst32:
	INST32;
	NEXT;
inst33:
	INST33;
	NEXT;
inst34:
	INST34;
	NEXT;
inst35:
	INST35;
	NEXT;
inst36:
	INST36;
	NEXT;
inst37:
	INST37;
	NEXT;
inst38:
	INST38;
	NEXT;
inst39:
	INST39;
	NEXT;
inst3a:
	INST3A;
	NEXT;
inst3b:
	INST3B;
	NEXT;
inst3c:
	INST3C;
	NEXT;
inst3d:
	INST3D;
	NEXT;
inst3e:
	INST3E;
	NEXT;
inst3f:
	INST3F;
	NEXT;
inst40:
	INST40;
	NEXT;
inst41:
	INST41;
	NEXT;
inst42:
	INST42;
	NEXT;
inst43:
	INST43;
	NEXT;
inst44:
	INST44;
	NEXT;
inst45:
	INST45;
	NEXT;
inst46:
	INST46;
	NEXT;
inst47:
	INST47;
	NEXT;
inst48:
	INST48;
	NEXT;
inst49:
	INST49;
	NEXT;
inst4a:
	INST4A;
	NEXT;
inst4b:
	INST4B;
	NEXT;
inst4c:
	INST4C;
	NEXT;
inst4d:
	INST4D;
	NEXT;
inst4e:
	INST4E;
	NEXT;
inst4f:
	INST4F;
	NEXT;
inst50:
	INST50;
	NEXT;
inst51:
	INST51;
	NEXT;
inst52:
	INST52;
	NEXT;
inst53:
	INST53;
	NEXT;
inst54:
	INST54;
	NEXT;
inst55:
	INST55;
	NEXT;
inst56:
	INST56;
	NEXT;
inst57:
	INST57;
	NEXT;
inst58:
	INST58;
	NEXT;
inst59:
	INST59;
	NEXT;
inst5a:
	INST5A;
	NEXT;
inst5b:
	INST5B;
	NEXT;
inst5c:
	INST5C;
	NEXT;
inst5d:
	INST5D;
	NEXT;
inst5e:
	INST5E;
	NEXT;
inst5f:
	INST5F;
	NEXT;
instxx:
	return -4;
}
