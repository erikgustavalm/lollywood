#include "vm.h"
#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define STK_SIZE 255

uint8_t *stack;
size_t stack_ptr;

const char int_size = sizeof(int);

uint8_t *program_data;

size_t instruction_ptr;

void init()
{
    stack = malloc(sizeof(uint8_t) * STK_SIZE);
    stack_ptr = 0;
    instruction_ptr = 0;
}

void run_opcode(uint8_t opcode, struct env *e)
{
    int *a, *b;
    char *c;
    char *start;

    ++instruction_ptr;

    switch (opcode) {
    case PUSH_SI:
	a = (int *)(stack + stack_ptr);
	*a = *(int *)&program_data[instruction_ptr];
	stack_ptr += int_size;
	instruction_ptr += int_size;
	printf("Pushed signed integer: %d to stack, sp: %ld\n", *a, stack_ptr);
	break;

    case PUSH_STR:
	// Saving start only for testing
	start = (char *)(stack + stack_ptr);
	while (program_data[instruction_ptr]) {
	    c = (char *)(stack + stack_ptr);
	    *c = *(char *)&program_data[instruction_ptr++];
	    stack_ptr++;
	}
	c = (char *)(stack + stack_ptr);
	*c = 0;
	stack_ptr++;
	printf("Pushed string: \"%s\" to stack, sp: %ld\n", start, stack_ptr);
	break;

    case ADD_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) + (*b);
	printf("Added two signed integers, result: %d, sp: %ld\n", *a, stack_ptr);
	break;

    case MULT_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) * (*b);
	printf("Multiplied two signed integers, result: %d, sp: %ld\n", *a, stack_ptr);
	break;

    case SUB_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) - (*b);
	printf("Subtract two signed integers, result: %d, sp: %ld\n", *a, stack_ptr);
	break;

    case PRINT_SI:
	a = (int *)(stack + stack_ptr - int_size);
	printf("%d\n", *a);
	break;

    case STORE_SI:
	stack_ptr -= int_size;
	a = (int *)(stack + stack_ptr);
	c = (char *)&program_data[instruction_ptr];
	env_add(e, c, (uint8_t *)a, int_size);
	printf("Stored signed integer \"%s\": %d in environment, sp: %ld\n", c, *a, stack_ptr);
	break;

    case LOAD_SI:
	c = (char *)&program_data[instruction_ptr];
	env_lookup(e, c, (stack + stack_ptr), int_size);
	printf("Loaded signed integer \"%s\": %d from environment, sp: %ld\n", c, *(int *)(stack + stack_ptr), stack_ptr + int_size);
	stack_ptr += int_size;
	break;

    case READ_SI:
	a = (int *)(stack + stack_ptr);
	scanf("%d", a);
	stack_ptr += int_size;
	printf("Read signed integer: %d from stdin, sp: %ld\n", *a, stack_ptr);
	break;

    default:
	break;
    }
}

int vm_run(uint8_t *program, size_t len)
{
    init();
    struct env *e = env_create();

    program_data = program;

    while (instruction_ptr < len) {
	run_opcode(program_data[instruction_ptr], e);
    }

    env_destroy(e);
    free(stack);
    return 0;
}
