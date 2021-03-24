#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "sexp.h"


struct sexp *parse_tokens(struct tokens *t);


#endif /* PARSER_H */
