#include "src/vm_src/vm.h"

#include <stdlib.h>


int main(int argc, char *argv[])
{
    uint8_t program[] = {
	JUMP,
	16,0,0,0,
	PUSH_SI,
	100,0,0,0,
	ADD_SI,
	JUMP,
	22,0,0,0,
	READ_SI,
	JUMP,
	5,0,0,0,
	PRINT_SI,
    };

    size_t len = sizeof(program) / sizeof(program[0]);

    vm_run(program, len);
    return 0;
}
