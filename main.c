#include "src/lexer.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    struct token *tok = lexer_tokenizer("(input)");
    struct token *cursor = tok;
    while (cursor){
	char *s = lexer_next(&cursor);
	printf("%s ", s);
	free(s);
    }
    printf("\n");
    lexer_free(tok);

    return 0;
}
