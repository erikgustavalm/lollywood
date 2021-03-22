#ifndef LEXER_H
#define LEXER_H

struct token;
enum tag;

char *lexer_next(struct token **t);

struct token *lexer_tokenizer(char *str);

void lexer_free(struct token *t);

#endif /* LEXER_H */
