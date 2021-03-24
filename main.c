#include "src/lexer.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    struct token *tok = lexer_tokenizer("(\"hello world\")");
    struct token *cursor = tok;
    while (cursor) {
	struct token *tmp = lexer_next(&cursor);
	char *data = lexer_token_data(tmp);
	char type = lexer_token_tag(tmp);
	printf("%d:%s\n", type, data);
    }

    printf("\n");
    lexer_free(tok);

    return 0;
}
