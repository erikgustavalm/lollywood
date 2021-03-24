#include "parser.h"


struct sexp *parse_tokens(struct token *t)
{

    struct token *root = t;
    struct token *cursor = lexer_next(&t);
    while (cursor) {

	cursor = lexer_next(&t);
    }

    lexer_free(root);

}
