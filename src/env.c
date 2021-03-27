#include "env.h"
#include <stdint.h>


#define DEF_SIZE 113
#define DATA_SIZE 512

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

void env_add(struct env *e, char *key, uint8_t *data, size_t size)
{
    uint64_t index = hash(e, key);
    e->data_size++;
    e->buckets[index] = (struct entry) {
	.key = key,
	.offset = e->data_size
    };
    for (int i = 0; i < size; ++i) {
	e->data[e->data_size++] = data[i];
    }
}

void env_lookup(struct env *e, char *key, uint8_t *data, size_t size)
{
    uint64_t index = hash(e, key);
    uint64_t offset = e->buckets[index].offset;

    for (int i = 0; i < size; ++i) {
	data[i] = *(uint8_t *)(e->data + offset + i);
    }
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
