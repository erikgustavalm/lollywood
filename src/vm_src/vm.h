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
#define READ_SI 100 // Read a signed integer and put it onto the stack

int vm_run(uint8_t *program, size_t len);

#endif /* VM_H */
