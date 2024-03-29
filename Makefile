LIB?=able
SRCS=edge.c node.c link.c mesg.c task.c
HDRS=edge.h node.h link.h mesg.h task.h able.h

.include "config.mk"

OBJS+=${SRCS:N*.h:R:S/$/.o/}

.PHONY: build clean install uninstall

build: lib${LIB}.a

clean:
	-rm -vf ${OBJS} lib${LIB}.a

install: lib${LIB}.a
	@mkdir -p ${LIBDIR}
	install -m 0644 lib${LIB}.a ${LIBDIR}/lib${LIB}.a
	@mkdir -p ${INCDIR}/${LIB}
.for HDR in ${HDRS}
	cp ${HDR} ${INCDIR}/${LIB}/${HDR}
.endfor

uninstall:
	-rm -vf ${LIBDIR}/lib${LIB}.a
	-rm -vrf ${INCDIR}/${LIB}

lib${LIB}.a: ${HDRS} ${OBJS}
	@rm -vf lib${LIB}.a
	${AR} ${ARFLAGS} lib${LIB}.a ${OBJS}
	${RANLIB} lib${LIB}.a
