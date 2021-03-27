BIN	:= bin/lollywood
VM_BIN  := bin/lollywood_vm
TEST	:= bin/tests
SRC	:= ${wildcard src/*.c}
VM_SRC  := ${wildcard src/vm_src/*.c}
OBJ	:= ${patsubst src/%.c,obj/%.o,${SRC}}
VM_OBJ  := ${patsubst src/vm_src/%.c,obj/vm_obj/%.o,${VM_SRC}}
CC 	:= gcc
CFLAGS	:= -std=c11 -Wall -g
SHELL	:= /bin/bash
VG	:= valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

obj/%.o: src/%.c
	${CC} ${CFLAGS} -c -o $@ $^

obj/vm_obj/%.o: src/vm_src/%.c
	${CC} ${CFLAGS} -c -o $@ $^

${BIN}: ${OBJ}
	mkdir -p obj bin
	echo "compiling"
	${CC} ${CFLAGS} -o $@ ${OBJ} main.c

${VM_BIN}: ${VM_OBJ}
	mkdir -p obj/vm_obj bin
	echo "compiling vm"
	${CC} ${CFLAGS} -o $@ ${VM_OBJ} vm_main.c

run: ${BIN}
	${BIN}

memrun: clean ${BIN}
	${VG} ${BIN}

vmmemrun: clean ${VM_BIN}
	${VG} ${VM_BIN}

${TEST}:
	${CC} ${CFLAGS} -o $@ ${SRC} tests/unit_test.c -lcunit

tests: ${TEST}
	${VG} $^

.PHONY clean:
clean:
	rm -f ${BIN} ${OBJ} ${TEST} ${VM_OBJ} ${VM_BIN}
