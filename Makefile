# Configure this
CC=gcc

CFLAGS=-O3 -mpopcnt
LDFLAGS=-lnautyW1
W1=-DMAXN=WORDSIZE -DWORDSIZE=32

PRUNERS=clawfree

PROGPREFFIX=nauty-geng-
PROGS=$(addprefix ${PROGPREFFIX}, ${PRUNERS})

.PHONY: all debug

all: ${PROGS}

debug: CFLAGS+=-O0 -p -g -DINSTRUMENT
debug:all

${PROGPREFFIX}%: pruners.c geng.c
	${CC} -o $@ ${CFLAGS} ${W1} -DPREPRUNE=$* $^ ${LDFLAGS}

clean:
	rm -f ${PROGS}
