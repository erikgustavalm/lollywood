#ifndef LEXER_H
#define LEXER_H

#define T_SYMB 1
#define T_OPAR 2
#define T_CPAR 3
#define T_PUNC 4
#define T_ATOM 5
#define T_TICK 6
#define T_EOF 10

struct token;

struct token *lexer_next(struct token **t);

char *lexer_token_data(struct token *t);

char lexer_token_tag(struct token *t);

struct token *lexer_tokenizer(char *str);

void lexer_free(struct token *t);

#endif /* LEXER_H */
