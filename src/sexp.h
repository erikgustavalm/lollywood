#ifndef SEXP_H
#define SEXP_H

#include <stdlib.h>

struct sexp {
    unsigned char *car;
    struct sexp *cdr;
    unsigned char type;
};

struct sexp sexp_add_atom(struct sexp **last, char type, unsigned char *data);

#endif /* SEXP_H */
