#include "src/vm_src/vm.h"

#include <stdlib.h>


int main(int argc, char *argv[])
{
    uint8_t program[] = {
	PUSH_SI,
	0,
	3,
	0,
	0,
	STORE_SI,
	'x',
	0,
	READ_SI,
	/* PUSH_SI, */
	/* 2, */
	/* 0, */
	/* 0, */
	/* 0, */
	STORE_SI,
	'y',
	0,
	LOAD_SI,
	'x',
	0,
	LOAD_SI,
	'y',
	0,
	ADD_SI
    };

    size_t len = sizeof(program) / sizeof(program[0]);

    vm_run(program, len);
    return 0;
}
