#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PUSH_SI 1 // Push a signed int to stack
#define ADD_SI 2 // Add two two upmost signed int on stack and push result to stack
#define MULT_SI 3 // Multiply two two upmost signed int on stack and push result to stack
#define SUB_SI 4 // Subtract two two upmost signed int on stack and push result to stack
#define PRINT_SI 10 // Print topmost signed int on stack

#define STK_SIZE 255

uint8_t *stack;
size_t stack_ptr;

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
    MULT_SI
};
size_t instruction_ptr;

void init()
{
    stack = malloc(sizeof(uint8_t) * STK_SIZE);
    stack_ptr = 0;
    instruction_ptr = 0;
}

void run_opcode(uint8_t opcode)
{
    int *a, *b;
    ++instruction_ptr;
    switch (opcode) {
    case PUSH_SI:
	a = (int *)(stack + stack_ptr);
	*a = *(int *)&program[instruction_ptr];
	stack_ptr += int_size;
	instruction_ptr += int_size;
	printf("Pushed signed integer: %d to stack, sp: %d\n", *a, stack_ptr);
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

    default:
	break;
    }
}

int main(int argc, char *argv[])
{
    init();
    size_t len = sizeof(program) / sizeof(program[0]);

    while (instruction_ptr < len) {
	run_opcode(program[instruction_ptr]);
    }

    free(stack);
    return 0;
}
