#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdlib.h>

#define PUSH_SI 1 // Push a signed int to stack
#define ADD_SI 2 // Add two two upmost signed int on stack and push result to stack
#define MULT_SI 3 // Multiply two two upmost signed int on stack and push result to stack
#define SUB_SI 4 // Subtract two two upmost signed int on stack and push result to stack

#define PRINT_SI 10 // Print topmost signed int on stack

#define STORE_SI 20 // Store a signed integer in the environment
#define LOAD_SI 21 // Load a signed integer from the environment

#define PUSH_STR 30 // Push a string to the stack

#define BRAT 40 // Read topmost byte, and branch if it is 1
#define COSI_ZERO 41 // Read topmost signed integer and if it zero, add 1 to stack. Otherwise add 0
#define COSI_NOT_ZERO 42 // Read topmost signed integer and if it is not zero, add 1 to stack. Otherwise add 0
#define COSI_NEQ 43 // Read two topmost signed integers and if they are not the same, add 1 to stack, otherwise 0
#define COSI_EQ 44 // Read two topmost signed integers and if they are the same, add 1 to stack, otherwise 0

#define JUMP 50 // Jump to address in program

#define READ_SI 100 // Read a signed integer and put it onto the stack

int vm_run(uint8_t *program, size_t len);

#endif /* VM_H */
