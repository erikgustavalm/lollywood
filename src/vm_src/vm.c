#include "vm.h"
#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define STK_SIZE 255

uint8_t *stack;
size_t stack_ptr;

const char num_size = sizeof(double);

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
    double *a, *b;
    int *ip;
    char *c;
    char *start;
    int debug_int;
    uint64_t *ptr;

    ++instruction_ptr;

    switch (opcode) {
    case PUSH_NUM:
	a = (double *)(stack + stack_ptr);
	*a = *(double *)&program_data[instruction_ptr];
	stack_ptr += num_size;
	instruction_ptr += num_size;
	printf("Pushed num: %lf to stack, sp: %ld\n", *a, stack_ptr);
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

    case ADD_NUM:
	a = (double *)(stack + stack_ptr - 2*num_size);
	b = (double *)(stack + stack_ptr - num_size);
	stack_ptr -= num_size;
	*a = (*a) + (*b);
	printf("Added two nums, result: %lf, sp: %ld\n", *a, stack_ptr);
	break;

    case MULT_NUM:
	a = (double *)(stack + stack_ptr - 2*num_size);
	b = (double *)(stack + stack_ptr - num_size);
	stack_ptr -= num_size;
	*a = (*a) * (*b);
	printf("Multiplied two nums, result: %lf, sp: %ld\n", *a, stack_ptr);
	break;

    case SUB_NUM:
	a = (double *)(stack + stack_ptr - 2*num_size);
	b = (double *)(stack + stack_ptr - num_size);
	stack_ptr -= num_size;
	*a = (*a) - (*b);
	printf("Subtract two nums, result: %lf, sp: %ld\n", *a, stack_ptr);
	break;

    case PRINT_NUM:
	a = (double *)(stack + stack_ptr - num_size);
	printf("%lf\n", *a);
	break;

    case STORE_NUM:
	stack_ptr -= num_size;
	a = (double *)(stack + stack_ptr);
	c = (char *)&program_data[instruction_ptr];
	env_add_atom(e, c, (uint8_t *)a, num_size);
	printf("Stored num \"%s\": %lf in environment, sp: %ld\n", c, *a, stack_ptr);
	break;
    case STORE_CONS:

	break;
    case LOAD_CAR:
	c = (char *)&program_data[instruction_ptr];
	debug_int = stack_ptr;
	stack_ptr += env_lookup_car(e, c, (stack + stack_ptr));
	printf("Loaded car address \"%s\": %ld from environment, sp: %ld\n", c, *(uint64_t *)(stack + debug_int), stack_ptr);
	break;

    case LOAD_CDR:
	c = (char *)&program_data[instruction_ptr];
	debug_int = stack_ptr;
	stack_ptr += env_lookup_cdr(e, c, (stack + stack_ptr));
	printf("Loaded cdr: %ld from \"%s\", sp: %ld\n", *(long int *)(stack + debug_int), c, stack_ptr);
	break;

    case LOAD_OFF:
	stack_ptr -= num_size;
	ptr = (uint64_t *)(stack + stack_ptr);
	debug_int = stack_ptr;
	stack_ptr += env_lookup_ptr(e, *ptr, (stack + stack_ptr));
	printf("Loaded car: %lf from pointer %lx, sp: %ld\n", *(double *)(stack + debug_int), *ptr, stack_ptr);
	break;

    case READ_NUM:
	a = (double *)(stack + stack_ptr);
	scanf("%lf", a);
	stack_ptr += num_size;
	printf("Read num: %lf from stdin, sp: %ld\n", *a, stack_ptr);
	break;

    case CONUM_ZERO: // Check if topmost numgned int is zero, add a byte as a result
	a = (double *)(stack + stack_ptr - num_size);
	++stack_ptr;
	c = (char *)(stack + stack_ptr);
	switch (*(int *)a) {
	case 0:
	    *c = 1;
	    break;
	default:
	    *c = 0;
	    break;
	}
	break;

    case CONUM_NOT_ZERO:
	a = (double *)(stack + stack_ptr - num_size);
	++stack_ptr;
	c = (char *)(stack + stack_ptr);
	switch (*(int *)a) {
	case 0:
	    *c = 0;
	    break;
	default:
	    *c = 1;
	    break;
	}
	break;

    case CONUM_NEQ:
	a = (double *)(stack + stack_ptr - 2*num_size);
	b = (double *)(stack + stack_ptr - num_size);
	*a -= (*b);
	++stack_ptr;
	c = (char *)(stack + stack_ptr);
	switch (*(int *)a) {
	case 0:
	    *c = 0;
	    break;
	default:
	    *c = 1;
	    break;
	}
	break;

    case CONUM_EQ:
	a = (double *)(stack + stack_ptr - 2*num_size);
	b = (double *)(stack + stack_ptr - num_size);
	*a -= (*b);
	++stack_ptr;
	c = (char *)(stack + stack_ptr);
	switch (*(int *)a) {
	case 0:
	    *c = 1;
	    break;
	default:
	    *c = 0;
	    break;
	}
	break;

    case BRAT:
	c = (char *)(stack + stack_ptr);
	--stack_ptr;
	switch (*c) {
	case 0:
	    instruction_ptr += num_size;
	    break;
	default:
	    ip = (int *)&program_data[instruction_ptr];
	    instruction_ptr = *ip;
	    break;
	}
	break;

    case JUMP:
	ip = (int *)&program_data[instruction_ptr];
	instruction_ptr = *ip;
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
