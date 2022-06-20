#include "../scanner/scanner.h"
#include "../error_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TOKEN_IS(s_token, ...) __TOKEN_IS__(s_token, \
					    (TokenType[]){__VA_ARGS__, -1})
#define CURRENT_TOKEN &tokens.array[0]

struct expression {
	struct token operator;
	struct expression* left;
	struct expression* right;
};

int __TOKEN_IS__(struct token *tok, TokenType type[]);
struct token advance();
struct expression *expression();
struct expression *equality();
struct expression *comparison();
struct expression *term();
struct expression *factor();
struct expression *unary();
struct expression *primary();
void error(struct token t, char* message);

static struct token_array tokens;

void parse(struct scan *s)
{
	tokens = *(s->tokens);

	/* for (int i = 0; i < s->tokens->count; i++) { */
	/* 	struct token *t = &(s->tokens->array[i]); */
	/* 	if (MATCH(t, PLUS, MINUS, BLUEPRINT)) { */
	/* 		PRINT_SUBSTRING(t->lexeme); */
	/* 		printf(" matches.\n"); */
	/* 	} else { */
	/* 		PRINT_SUBSTRING(t->lexeme); */
	/* 		printf(" does not match.\n"); */
	/* 	} */
	/* } */
}

struct token advance()
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

struct expression *expression()
{
	return equality();
}

struct expression *equality()
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

struct expression *comparison()
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

struct expression *term()
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

struct expression *factor()
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

struct expression *unary()
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

struct expression *primary()
{
	struct expression *lit = malloc(sizeof(struct expression));
	if (TOKEN_IS(CURRENT_TOKEN, YES, NO, NIL, NUMBER, STRING)) {
		lit->operator = advance();
		return lit;
	}

	if (TOKEN_IS(CURRENT_TOKEN, LEFT_PAREN)) {
		struct expression *expr = expression();
		CHECK_ERROR_AND_PERFORM(TOKEN_IS(CURRENT_TOKEN, RIGHT_PAREN),
					perror("Expected ')' after expression.");)
		return expr;
	}
}

void error(struct token t, char* message)
{
	
}	

int __TOKEN_IS__(struct token *tok, TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
