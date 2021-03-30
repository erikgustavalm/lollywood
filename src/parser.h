#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "sexp.h"


struct sexp *parse_tokens(struct tokens *t);

int parse_output_to_file(char *filename, struct sexp *s);

#endif /* PARSER_H */
