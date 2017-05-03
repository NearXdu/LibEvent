PROGS=echo \
      epoll \
      block_http_client \
      method \
      method2 \
      timer\
      test \
      alarm \
      active \
      eventbuffer \
      eventbufferserver \
      watermark \
      evbuffer \
      evconn

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
block_http_client:${SRC}/block_http_client.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
method:${SRC}/method.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
method2:${SRC}/method2.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
timer:${SRC}/timer.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
test:${SRC}/test.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
alarm:${SRC}/alarm.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
active:${SRC}/active.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
eventbuffer:${SRC}/eventbuffer.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
eventbufferserver:${SRC}/eventbufferserver.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
watermark:${SRC}/watermark.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
evbuffer:${SRC}/evbuffer.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
evconn:${SRC}/evconn.o
	@${CC} ${CFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}



clean:
	@rm -rf ${OUTPUT} \
	    @rm -rf ${SRC}/*.o


.PHONY: all clean
