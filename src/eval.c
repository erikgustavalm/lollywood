#include "eval.h"
#include "lexer.h"

struct sexp *eval_atom(struct sexp *s)
{

}

struct sexp *eval_list(struct sexp *s)
{

}

struct sexp *eval_symb(struct sexp *s)
{

}

struct sexp *eval_sexp(struct sexp *s)
{
    switch (s->type) {
    case T_ATOM: return eval_atom(s);
    case T_LIST: return eval_list(s);
    case T_SYMB: return eval_symb(s);
    }
    return NULL;
}
