#ifndef ENV_H
#define ENV_H

#include <stdlib.h>
#include <stdint.h>

struct env;

struct env *env_create();

void env_destroy(struct env *e);

void env_add(struct env *e, char *key, uint8_t *data, size_t size);

void env_lookup(struct env *e, char *key, uint8_t *data, size_t size);

#endif /* ENV_H */
