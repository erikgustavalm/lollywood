#ifndef ENV_H
#define ENV_H

#include <stdlib.h>
#include <stdint.h>

struct env;

struct env *env_create();

void env_destroy(struct env *e);

void env_add_atom(struct env *e, char *key, uint8_t *data, size_t size);

void env_lookup(struct env *e, char *key, uint8_t *data, size_t size);

int env_lookup_car(struct env *e, char *key, uint8_t *data);

int env_lookup_cdr(struct env *e, char *key, uint8_t *data);

uint64_t env_add_cons(struct env *e, uint8_t *data, size_t size, uint64_t address);

int env_lookup_ptr(struct env *e, uint64_t address, uint8_t *data);

#endif /* ENV_H */
