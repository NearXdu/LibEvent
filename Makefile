PROGS=echo \
      epoll
CLEANFILES = core core.* *.core *.o temp.* *.out typescript* \
		*.lc *.lh *.bsdi *.sparc *.uw


LIBEVENT_DIRECTORY = /home/zhangxiao/libevent/src/src2
LIBEVENT_INCLUDE = $(LIBEVENT_DIRECTORY)/include
LIBEVENT_LIBRARY = $(LIBEVENT_DIRECTORY)/lib

SRC=${shell pwd}/src

OUTPUT:=${shell pwd}/bin
MAKE_BIN_DIR := ${shell mkdir -p $(OUTPUT) }



all : ${PROGS}


CFLAGS+=-g  -I${LIBEVENT_INCLUDE} 
LDFLAGS+=-L${LIBEVENT_LIBRARY} -levent -lpthread -lrt

echo:${SRC}/echo.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
epoll:${SRC}/epoll.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}




clean:
	@rm -rf ${OUTPUT} \
	    @rm -rf ${SRC}/*.o


.PHONY: all clean
