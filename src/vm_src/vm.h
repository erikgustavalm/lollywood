#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdlib.h>

#define PUSH_NUM 1 // Push a double to stack
#define ADD_NUM 2 // Add two two upmost double on stack and push result to stack
#define MULT_NUM 3 // Multiply two two upmost double on stack and push result to stack
#define SUB_NUM 4 // Subtract two two upmost double on stack and push result to stack

#define PRINT_NUM 10 // Print topmost double on stack

#define STORE_NUM 20 // Store a double in the environment
#define LOAD_NUM 21 // Load a double from the environment
#define STORE_PTR 22 // Store a pointer in the environment
#define LOAD_PTR 23 // Load a pointer from the environment
#define LOAD_CAR 24 // Load car from list in the environment
#define LOAD_CDR 25 // Load cdr from environment and add it to stack
#define LOAD_OFF 26 // Load from an offset
#define STORE_CONS 27 // Store a cons and push its address to the stack

#define PUSH_STR 30 // Push a string to the stack

#define BRAT 40 // Read topmost byte, and branch if it is 1
#define CONUM_ZERO 41 // Read topmost doubleeger and if it zero, add 1 to stack. Otherwise add 0
#define CONUM_NOT_ZERO 42 // Read topmost doubleeger and if it is not zero, add 1 to stack. Otherwise add 0
#define CONUM_NEQ 43 // Read two topmost doubleegers and if they are not the same, add 1 to stack, otherwise 0
#define CONUM_EQ 44 // Read two topmost doubleegers and if they are the same, add 1 to stack, otherwise 0

#define JUMP 50 // Jump to address in program

#define READ_NUM 100 // Read a double and put it onto the stack

int vm_run(uint8_t *program, size_t len);

#endif /* VM_H */
