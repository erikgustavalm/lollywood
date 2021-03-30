#include "src/vm_src/vm.h"

#include <stdlib.h>


int main(int argc, char *argv[])
{
    /* double d = 55.0; */
    /* uint8_t *sa = (uint8_t *)&d; */
    /* for (int i = 0; i < 8; ++i) { */
    /* 	printf("%d - [%d]\n",i, sa[i]); */
    /* } */

    uint8_t program[] = {
	PUSH_NUM,
	0,0,0,0,0,0,89,64,
	STORE_NUM,
	'x',
	0,
	PUSH_NUM,
	0,0,0,0,0,128,75,64,
	STORE_NUM,
	'y',
	0,
	LOAD_CAR,
	'x',
	0,
	LOAD_OFF,
	LOAD_CAR,
	'y',
	0,
	LOAD_OFF,
	ADD_NUM,
	PRINT_NUM
	/* JUMP, */
	/* 16,0,0,0, */
	/* PUSH_SI, */
	/* 100,0,0,0, */
	/* ADD_SI, */
	/* JUMP, */
	/* 22,0,0,0, */
	/* READ_SI, */
	/* JUMP, */
	/* 5,0,0,0, */
	/* PRINT_SI, */
    };

    size_t len = sizeof(program) / sizeof(program[0]);

    vm_run(program, len);
    return 0;
}
