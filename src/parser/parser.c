#include "scanner/scanner.h"
#include "error_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CURRENT_TOKEN &tokens.array[0]
#define TOKEN_IS(s_token, ...) __TOKEN_IS__(s_token,			\
					    (TokenType[]){__VA_ARGS__, -1})


struct expression {
	struct token operator;
	struct expression* left;
	struct expression* right;
};

static int __TOKEN_IS__(struct token *tok, TokenType type[]);
static struct token advance();
static struct expression *expression();
static struct expression *equality();
static struct expression *comparison();
static struct expression *term();
static struct expression *factor();
static struct expression *unary();
static struct expression *primary();
void error(struct token t, char* message);

static struct token_array tokens;
#include <string.h>
void treecpy(char* str, int *offset, struct expression *e)
{
#define WRITE_LEXEME							\
	{								\
		strncpy(str+*offset, e->operator.lexeme.start,		\
			SUBSTRING_LENGTH(e->operator.lexeme));		\
		*offset += SUBSTRING_LENGTH(e->operator.lexeme);	\
	}
#define ADD(c)					\
	{					\
		str[*offset] = c;		\
		(*offset)++;			\
	}

	if (e->left != NULL) {
		ADD('(');
		WRITE_LEXEME;
		ADD(' ');
		treecpy(str, offset, e->left);
		ADD(' ');
		treecpy(str, offset, e->right);
		ADD(')');
	} else if (e->right != NULL) {
		ADD('(');
		WRITE_LEXEME;
		treecpy(str, offset, e->right);
		ADD(')');
	} else {
		WRITE_LEXEME;
	}
#undef WRITE_LEXEME
#undef SPACE
}

void parse(struct scan *s)
{
	tokens = *(s->tokens);

	struct expression *e = expression();
	char t[1024];
	int i = 0;
	for (int _i = 0; _i < 1024; _i++)
		t[_i] = '\0';
	treecpy(t, &i, e);
	printf("%s\n", t);
}

static struct token advance()
{
	assert(tokens.count > 0);
	
	struct token t = tokens.array[0];
	
	if (tokens.count == 1)
		return t;
	
	++(tokens.array);
	--(tokens.count);
	tokens.size -= sizeof(struct token);

	return t;
}

static struct expression *expression()
{
	return equality();
}

static struct expression *equality()
{
	struct expression *expr = comparison();

	while (TOKEN_IS(CURRENT_TOKEN, BANG_EQUAL, EQUAL_EQUAL)) {
		struct token operator = advance();
		struct expression *right = comparison();

		struct expression *new_expr = malloc(sizeof(struct expression));
		*new_expr = (struct expression) {
			.operator = operator,
			.left = expr,
			.right = right
		};
		expr = new_expr;
	}

	return expr;
}

static struct expression *comparison()
{
	struct expression *expr = term();

	while (TOKEN_IS(CURRENT_TOKEN, \
			GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
		struct token operator = advance();
		struct expression *right = term();

		struct expression *new_expr = malloc(sizeof(struct expression));
		*new_expr = (struct expression) {
			.operator = operator,
			.left = expr,
			.right = right
		};
		expr = new_expr;
	}

	return expr;
}

static struct expression *term()
{
	struct expression *expr = factor();

	while (TOKEN_IS(CURRENT_TOKEN, PLUS, MINUS)) {
		struct token operator = advance();
		struct expression *right = factor();

		struct expression *new_expr = malloc(sizeof(struct expression));
		*new_expr = (struct expression) {
			.operator = operator,
			.left = expr,
			.right = right
		};
		expr = new_expr;
	}

	return expr;
}

static struct expression *factor()
{
	struct expression *expr = unary();

	while (TOKEN_IS(CURRENT_TOKEN, STAR, SLASH)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *new_expr = malloc(sizeof(struct expression));
		*new_expr = (struct expression) {
			.operator = operator,
			.left = expr,
			.right = right
		};
		expr = new_expr;
	}

	return expr;
}

static struct expression *unary()
{
	while (TOKEN_IS(CURRENT_TOKEN, BANG, MINUS)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *expr = malloc(sizeof(struct expression));
		*expr = (struct expression) {
			.operator = operator,
			.right = right
		};
		return expr;
	}

	return primary();
}

static struct expression *primary()
{
	struct expression *lit = malloc(sizeof(struct expression));
	if (TOKEN_IS(CURRENT_TOKEN, YES, NO, NIL, NUMBER, STRING)) {
		lit->operator = advance();
		return lit;
	}

	if (TOKEN_IS(CURRENT_TOKEN, LEFT_PAREN)) {
		advance();
		struct expression *expr = expression();
		CHECK_ERROR_AND_PERFORM(!TOKEN_IS(CURRENT_TOKEN, RIGHT_PAREN),
					perror("Expected ')' after expression."););
		advance();
		return expr;
	}

	CHECK_ERROR_AND_PERFORM(1, perror("Expression not found."););
}

void error(struct token t, char* message)
{
	
}	

static int __TOKEN_IS__(struct token *tok, TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
