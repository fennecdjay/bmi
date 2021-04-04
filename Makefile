CFLAGS   += -Iinclude -fPIC -std=c99
WARNINGS += -Wall -Wextra -Wpedantic
SRC      := $(wildcard src/*.c)
OBJ      := ${SRC:.c=.o}
PRG      := libbmi

ifeq ($(shell uname), Darwin)
AR = /usr/bin/libtool
AR_OPT = -static $^ -o $@
else
AR = ar
AR_OPT = rcs $@ $^
endif

CFLAGS += ${WARNINGS}

all: static dynamic test

.PHONY:  static dynamic
static:  ${PRG}.a
dynamic: ${PRG}.so

${PRG}.a: ${OBJ}
	${AR} ${AR_OPT}

${PRG}.so: ${OBJ}
	${CC} -shared $< -o $@

test: ${PRG}.a
	${CC} ${CFLAGS} -I. main.c $< -o test

.c.o:
	${CC} ${CFLAGS} $< -c -o ${<:.c=.o}

clean:
	rm -rf ${PRG}.a ${PRG}.so ${OBJ}
