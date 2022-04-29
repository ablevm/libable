LIB?=able
SUB?=misc
SRCS=core.c host.c
HDRS=core.h host.h misc.h

.include "config.mk"

OBJS+=${SRCS:N*.h:R:S/$/.o/}

.PHONY: build clean install uninstall

build: lib${LIB}_${SUB}.a

clean:
	-rm -vf ${OBJS} lib${LIB}_${SUB}.a

install: lib${LIB}_${SUB}.a
	@mkdir -p ${LIBDIR}/
	install -m 0644 lib${LIB}_${SUB}.a ${LIBDIR}/lib${LIB}_${SUB}.a
	@mkdir -p ${INCDIR}/${LIB}/${SUB}/
	install -m 0444 ${HDRS} ${INCDIR}/${LIB}/${SUB}/

uninstall:
	-rm -vf ${LIBDIR}/lib${LIB}_${SUB}.a
	-rm -vrf ${INCDIR}/${LIB}/${SUB}/

lib${LIB}_${SUB}.a: ${HDRS} ${OBJS}
	@rm -vf lib${PRO}_${LIB}.a
	${AR} ${ARFLAGS} lib${LIB}_${SUB}.a ${OBJS}
	${RANLIB} lib${LIB}_${SUB}.a
