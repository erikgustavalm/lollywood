#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>


#define ACC 100
#define MAX_BUF 255

#define UNACC(last, c, line) fprintf(stderr, "syntax error at line:%d, unexpected '%c'\n", line, c)

extern char *strdup(const char *);

uint16_t line_nr;

struct token {
    char *data;
    struct token *next;
    uint16_t line;
    char type;
};

static
void cut(struct token **last, char *buf, int *i, char type)
{
    buf[*i] = '\0';

    (*last)->data = strdup(buf);
    (*last)->type = type;
    (*last)->line = line_nr;
    struct token *new = malloc(sizeof(struct token));
    *new = (struct token) {
	.data = NULL,
	.next = NULL,
	.line = 0,
	.type = 0
    };

    (*last)->next = new;
    *last = new;

    *i = 0;
}

static
char next_state(char curr_state, char c, struct token **last, char *buf, int *i)
{
    switch (curr_state) {
    case 0: // Initial state
	switch (c) {
	case '(':
	    buf[(*i)++] = '(';
	    cut(last, buf, i, T_OPAR);
	    return 1;
	case ')':
	    buf[(*i)++] = ')';
	    cut(last, buf, i, T_CPAR);
	    return 0;
	case '\n':
	    ++line_nr;
	    return 0;
	case ' ':
	    return 0;
	case '\'':
	    buf[(*i)++] = '\'';
	    cut(last, buf, i, T_TICK);
	    return 0;
	case '\0':
	    (*last)->data = strdup("ACC");
	    (*last)->type = T_EOF;
	    return ACC;
	default:
	    if (isdigit(c)) {
		buf[(*i)++] = c;
		return 7;
	    }
	    if (c == '-') {
		buf[(*i)++] = c;
		return 8;
	    }
	    if (c == '\"') {
		return 11;
	    }

	    UNACC(last, c, line_nr);
	    return -1;
	}

    case 1: // Start of a sexp
	if (c == '(') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_OPAR);
	    return 0;
	}
	if (c == '\'') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_TICK);
	    return 0;
	}
	if (c == ')') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	if (islower(c)) {
	    buf[(*i)++] = c;
	    return 2;
	}
	if (isupper(c)) {
	    buf[(*i)++] = c;
	    return 4;
	}
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_SYMB);
	    return 1;
	}
	if (c == '\n') {
	    ++line_nr;
	    cut(last, buf, i, T_SYMB);
	    return 1;
	}
	if (c == '\'') {
	    cut(last, buf, i, T_TICK);
	    return 1;
	}
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 7;
	}
	if (c == '-') {
	    buf[(*i)++] = c;
	    return 8;
	}
	if (c == '\"') {
	    return 11;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 2: // Midst of symbol name
	if (islower(c)) {
	    buf[(*i)++] = c;
	    return 2;
	}
	if (c == '-') {
	    buf[(*i)++] = c;
	    return 3;
	}
	if (c == ')') {
	    cut(last, buf, i, T_SYMB);
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_SYMB);
	    return 5;
	}
	if (c == '\n') {
	    ++line_nr;
	    cut(last, buf, i, T_SYMB);
	    return 5;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 3: // symbol name but can't end
	if (islower(c)) {
	    buf[(*i)++] = c;
	    return 2;
	}
	if (c == '-') {
	    buf[(*i)++] = c;
	    return 3;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 4: // Atom
	if (isupper(c)) {
	    buf[(*i)++] = c;
	    return 4;
	}
	if (c == ')') {
	    cut(last, buf, i, T_ATOM);
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	if (c == '\n') {
	    ++line_nr;
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 5: // after whitespace in list
	if (c == '(') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_OPAR);
	    return 1;
	}
	if (c == '\'') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_TICK);
	    return 1;
	}
	if (c == ')') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 1;
	}
	if (c == '.') {
	    buf[(*i)++] = c;
	    return 6;
	}
	if (islower(c)) {
	    buf[(*i)++] = c;
	    return 2;
	}
	if (isupper(c)) {
	    buf[(*i)++] = c;
	    return 4;
	}
	if (c == '\t' || c == ' ') {
	    return 5;
	}
	if (c == '\n') {
	    ++line_nr;
	    return 5;
	}
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 7;
	}
	if (c == '-') {
	    buf[(*i)++] = c;
	    return 8;
	}
	if (c == '\"') {
	    return 11;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 6: // punctuation
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_PUNC);
	    return 1;
	}
	if (c == '\n') {
	    ++line_nr;
	    return 1;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 7: // Midst of number
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 7;
	}
	if (c == '.') {
	    buf[(*i)++] = c;
	    return 9;
	}
	if (c == ')') {
	    cut(last, buf, i, T_ATOM);
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	if (c == '\n') {
	    ++line_nr;
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 8: // Negative number
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 7;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 9: // Number that contains a '.'
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 10;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 10: // Negative number with at least one decimal digit
	if (isdigit(c)) {
	    buf[(*i)++] = c;
	    return 10;
	}
	if (c == ')') {
	    cut(last, buf, i, T_ATOM);
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	if (c == '\t' || c == ' ') {
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	if (c == '\n') {
	    ++line_nr;
	    cut(last, buf, i, T_ATOM);
	    return 5;
	}
	UNACC(last, c, line_nr);
	return -1;

    case 11:
	if (c == '\0') {
	    UNACC(last, '!', line_nr);
	    return -1;
	}
	if (c == '\"') {
	    cut(last, buf, i, T_ATOM);
	    return 12;
	}
	buf[(*i)++] = c;
	return 11;

    case 12:
	if (c == '\n' || c == '\t' || c == ' ') {
	    return 5;
	}
	if (c == ')') {
	    buf[(*i)++] = c;
	    cut(last, buf, i, T_CPAR);
	    return 0;
	}
	UNACC(last, c, line_nr);
	return -1;
    }


    return -1;
}

struct token *lexer_next(struct token **t)
{
    if (!(*t)) return NULL;

    struct token *tmp = *t;
    *t = (*t)->next;
    return tmp;
}

char *lexer_token_data(struct token *t)
{
    if (!t) return NULL;
    return t->data;
}

char lexer_token_tag(struct token *t)
{
    if (!t) return T_EOF;
    return t->type;
}

struct token *lexer_tokenizer(char *str)
{
    struct token *root = malloc(sizeof(struct token));
    struct token *last = root;

    int i = 0;
    line_nr = 1;

    char *buf = malloc(sizeof(char) * MAX_BUF);

    char state = next_state(0, *str, &last, buf, &i);
    root->next = last;
    while (state >= 0) {
	if (state == ACC) {
	    free(buf);
	    return root;
	}

	str++;
	state = next_state(state, *str, &last, buf, &i);
    }

    free(buf);
    lexer_free(root);

    return NULL;
}

void lexer_free(struct token *t)
{
    struct token *cursor = t;
    while (cursor) {
	struct token *tmp = cursor->next;
	free(cursor->data);
	free(cursor);
	cursor = tmp;
    }
}
