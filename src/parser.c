#include "parser.h"

#include <stdio.h>
#include <string.h>

struct sexp *parse_tokens(struct token *t)
{

    struct token *root = t;
    struct token *cursor = lexer_next(&t);
    while (cursor) {

	cursor = lexer_next(&t);
    }

    lexer_free(root);

}

int parse_output_to_file(char *filename, struct sexp *s)
{
    FILE *fp = fopen(filename, "wb");

}
