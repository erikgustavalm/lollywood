BIN	:= bin/lollywood
TEST	:= bin/tests
SRC	:= ${wildcard src/*.c}
OBJ	:= ${patsubst src/%.c,obj/%.o,${SRC}}
CC 	:= gcc
CFLAGS	:= -std=c11 -Wall -g
SHELL	:= /bin/bash
VG	:= valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

obj/%.o: src/%.c
	${CC} ${CFLAGS} -c -o $@ $^

${BIN}: ${OBJ}
	mkdir -p obj bin
	echo "compiling"
	${CC} ${CFLAGS} -o $@ ${OBJ} main.c

run: ${BIN}
	${BIN}

memrun: clean ${BIN}
	${VG} ${BIN}

${TEST}:
	${CC} ${CFLAGS} -o $@ ${SRC} tests/unit_test.c -lcunit

tests: ${TEST}
	${VG} $^

.PHONY clean:
clean:
	rm -f ${BIN} ${OBJ} ${TEST}
