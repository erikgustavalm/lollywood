#include "vm.h"
#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PUSH_SI 1 // Push a signed int to stack
#define ADD_SI 2 // Add two two upmost signed int on stack and push result to stack
#define MULT_SI 3 // Multiply two two upmost signed int on stack and push result to stack
#define SUB_SI 4 // Subtract two two upmost signed int on stack and push result to stack
#define PRINT_SI 10 // Print topmost signed int on stack
#define STORE_SI 20 // Store a signed integer in the environment
#define LOAD_SI 21 // Load a signed integer from the environment
#define PUSH_STR 30 // Push a string to the stack

#define STK_SIZE 255

uint8_t *stack;
size_t stack_ptr;

char tmp_buf[100];

const char int_size = sizeof(int);

uint8_t program[] = {
    PUSH_SI,
    0,
    3,
    0,
    0,
    PUSH_SI,
    0,
    3,
    0,
    0,
    MULT_SI,
    PUSH_STR,
    'x',
    '0',
    STORE_SI,
    PUSH_STR,
    'x',
    '0',
    LOAD_SI
};
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
	*a = *(int *)&program[instruction_ptr];
	stack_ptr += int_size;
	instruction_ptr += int_size;
	printf("Pushed signed integer: %d to stack, sp: %d\n", *a, stack_ptr);
	break;

    case PUSH_STR:
	// Saving start only for testing
	start = (char *)(stack + stack_ptr);
	while (program[instruction_ptr]) {
	    c = (char *)(stack + stack_ptr);
	    *c = *(char *)&program[instruction_ptr++];
	    stack_ptr++;
	}
	printf("Pushed string: \"%s\" to stack, sp: %d\n", start, stack_ptr);
	break;

    case ADD_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) + (*b);
	printf("Added two signed integers, result: %d, sp: %d\n", *a, stack_ptr);
	break;

    case MULT_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) * (*b);
	printf("Multiplied two signed integers, result: %d, sp: %d\n", *a, stack_ptr);
	break;

    case SUB_SI:
	a = (int *)(stack + stack_ptr - 2*int_size);
	b = (int *)(stack + stack_ptr - int_size);
	stack_ptr -= int_size;
	*a = (*a) - (*b);
	printf("Subtract two signed integers, result: %d, sp: %d\n", *a, stack_ptr);
	break;

    case PRINT_SI:
	a = (int *)(stack + stack_ptr - int_size);
	printf("%d\n", *a);
	break;

    case STORE_SI:
	*a = 1;
	c = (char *)(stack + stack_ptr - (*a));
	while (*c) {
	    tmp_buf[(*a) - 1] = c;
	    (*a)++;
	    c = (char *)(stack + stack_ptr - (*a));
	}
	tmp_buf[*a] = 0;
	stack_ptr -= (*a);
	b = (int *)(stack + stack_ptr);
	env_add(e, tmp_buf, (uint8_t *)b, int_size);
	stack_ptr -= (*b);
	printf("Stored signed integer \"%s\": %d in environment\n", tmp_buf, *b);
	break;

    case LOAD_SI:
	*a = 1;
	c = (char *)(stack + stack_ptr - (*a));
	while (*c) {
	    tmp_buf[(*a) - 1] = c;
	    (*a)++;
	    c = (char *)(stack + stack_ptr - (*a));
	}
	tmp_buf[*a] = 0;
	env_lookup(e, tmp_buf, (stack + stack_ptr), int_size);
	printf("Loaded signed integer \"%s\": %d from environment\n", tmp_buf, *(int *)(stack + stack_ptr));
	stack_ptr += int_size;
	break;

    default:
	break;
    }
}

int main(int argc, char *argv[])
{
    init();
    struct env *e = env_create();

    size_t len = sizeof(program) / sizeof(program[0]);

    while (instruction_ptr < len) {
	run_opcode(program[instruction_ptr], e);
    }

    env_destroy(e);
    free(stack);
    return 0;
}
