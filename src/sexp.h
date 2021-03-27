#ifndef SEXP_H
#define SEXP_H

#include <stdlib.h>
#include "common.h"

struct ccell {
    unsigned char *car;
    struct sexp *cdr;
};

struct ccell sexp_add_atom(struct ccell **last, unsigned char *data);

#endif /* SEXP_H */
