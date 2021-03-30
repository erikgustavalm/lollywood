#include "env.h"
#include <stdint.h>
#include <string.h>


#define DEF_SIZE 113
#define DATA_SIZE 512

struct memblock {
    uint64_t cdr;
    size_t size;
    uint8_t car;
};

const int MBSIZE = sizeof(struct memblock *) + sizeof(size_t);

struct entry {
    char *key;
    uint64_t offset;
};

struct env {
    struct entry *buckets;
    uint8_t *data;
    size_t data_size;
    size_t nr_buckets;
};

static uint64_t hash(struct env *e, char *key)
{
    size_t i = 0;
    uint64_t index = key[i];
    while (key[i] != '\0') {
	index += key[i++];
    }
    return index % e->nr_buckets;
}

void env_add_atom(struct env *e, char *key, uint8_t *data, size_t size)
{
    uint64_t index = hash(e, key);
    e->buckets[index] = (struct entry) {
	.key = key,
	.offset = e->data_size
    };
    int start = e->data_size;

    struct memblock *tmp = (struct memblock *)(e->data + e->data_size);
    tmp->cdr = (double) 0.0;
    tmp->size = size;
    memcpy(&tmp->car, data, size);
    e->data_size += MBSIZE + size;
}

uint64_t env_add_cons(struct env *e, uint8_t *data, size_t size, uint64_t address)
{
    struct memblock *tmp = (struct memblock *)(e->data + e->data_size);
    tmp->cdr = address;
    tmp->size = size;
    memcpy(&tmp->car, data, size);
    uint64_t retval = e->data_size;
    e->data_size += MBSIZE + size;
    return retval;
}

int env_lookup_car(struct env *e, char *key, uint8_t *data)
{
    uint64_t index = hash(e, key);
    uint64_t offset = e->buckets[index].offset;

    memcpy(data, &offset, 8);
    return 8;
}

int env_lookup_cdr(struct env *e, char *key, uint8_t *data)
{
    uint64_t index = hash(e, key);
    uint64_t offset = e->buckets[index].offset;

    memcpy(data, e->data + offset, 8);
    return 8;
}

int env_lookup_ptr(struct env *e, uint64_t address, uint8_t *data)
{
    struct memblock *tmp = (struct memblock *)(e->data + address);
    memcpy(data, &tmp->car, tmp->size);
    return tmp->size;
}

struct env *env_create()
{
    struct env *new = malloc(sizeof(struct env));
    new->nr_buckets = DEF_SIZE;
    new->data_size = 0;
    new->buckets = malloc(sizeof(struct entry) * new->nr_buckets);
    new->data = calloc(DATA_SIZE, sizeof(uint8_t));

    return new;
}

void env_destroy(struct env *e)
{
    free(e->data);
    free(e->buckets);
    free(e);
}
