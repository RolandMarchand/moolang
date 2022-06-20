/* 
 * Moolang, a programming language.
 * Copyright (C) 2022 moowool195@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "parser.h"
#include "scanner/scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <error/error.h>
#include <assert.h>

#define CURRENT_TOKEN &tokens.array[0]
#define CURRENT_TOKEN_IS(...) __TOKEN_IS__(CURRENT_TOKEN,			\
					    (TokenType[]){__VA_ARGS__, -1})

static struct token_array tokens;

static struct token advance();

static struct expression *expression();
static struct expression *equality();
static struct expression *comparison();
static struct expression *term();
static struct expression *factor();
static struct expression *unary();
static struct expression *primary();

static int __TOKEN_IS__(struct token *tok, TokenType type[]);

// TODO: remove
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

	while (CURRENT_TOKEN_IS(BANG_EQUAL, EQUAL_EQUAL)) {
		struct token operator = advance();
		struct expression *right = comparison();

		struct expression *new_expr = malloc(sizeof(struct expression));

		if (new_expr == NULL) {
			FAIL_ALLOC;
		}

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

	while (CURRENT_TOKEN_IS(\
			GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
		struct token operator = advance();
		struct expression *right = term();

		struct expression *new_expr = malloc(sizeof(struct expression));

		if (new_expr == NULL) {
			FAIL_ALLOC;
		}

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

	while (CURRENT_TOKEN_IS(PLUS, MINUS)) {
		struct token operator = advance();
		struct expression *right = factor();

		struct expression *new_expr = malloc(sizeof(struct expression));

		if (new_expr == NULL) {
			FAIL_ALLOC;
		}

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

	while (CURRENT_TOKEN_IS(STAR, SLASH)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *new_expr = malloc(sizeof(struct expression));

		if (new_expr == NULL) {
			FAIL_ALLOC;
		}

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
	while (CURRENT_TOKEN_IS(BANG, MINUS)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *new_expr = malloc(sizeof(struct expression));

		if (new_expr == NULL) {
			FAIL_ALLOC;
		}

		*new_expr = (struct expression) {
			.operator = operator,
			.right = right
		};
		return new_expr;
	}

	return primary();
}

static struct expression *primary()
{
	struct expression *lit = malloc(sizeof(struct expression));

	if (lit == NULL) {
		FAIL_ALLOC;
	}

	if (CURRENT_TOKEN_IS(YES, NO, NIL, NUMBER, STRING)) {
		lit->operator = advance();
		return lit;
	}

	if (CURRENT_TOKEN_IS(LEFT_PAREN)) {
		advance();
		struct expression *expr = expression();

		if (!CURRENT_TOKEN_IS(RIGHT_PAREN)) {
			ERR_EXPECTED_CHARACTER(*CURRENT_TOKEN.line,
					       *CURRENT_TOKEN.lexeme.start[0],
					       ')');
		}

		advance();
		return expr;
	}

	ERR_EXPRESSION_NOT_FOUND;
}

static int __TOKEN_IS__(struct token *tok, TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
