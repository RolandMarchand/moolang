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

#include "expression.h"
#include "token_manager.h"

#include "macros.h"

#include <stdio.h>
#include <stdlib.h>

#define expression() get_next_expr()

struct token_array parser_tokens;

static struct expression *equality();
static struct expression *comparison();
static struct expression *term();
static struct expression *factor();
static struct expression *unary();
static struct expression *primary();

void free_expression(struct expression *e)
{
	if (e->left != NULL) {
		free_expression(e->left);
	}

	if (e->right != NULL) {
		free_expression(e->right);
	}

	free(e);
	e = NULL;
}

struct expression *get_next_expr()
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

		ASSERT(new_expr != NULL, "Failed to allocate memory.");

		*new_expr = (struct expression){
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

		ASSERT(new_expr != NULL, "Failed to allocate memory.");

		*new_expr = (struct expression){
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

		ASSERT(new_expr != NULL, "Failed to allocate memory.");

		*new_expr = (struct expression){
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

		ASSERT(new_expr != NULL, "Failed to allocate memory.");

		*new_expr = (struct expression){
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

		ASSERT(new_expr != NULL, "Failed to allocate memory.");

		*new_expr = (struct expression){
			.operator = operator,
			.right = right
		};
		return new_expr;
	}

	return primary();
}

static struct expression *primary()
{
	if (CURRENT_TOKEN_IS(YES, NO, NIL, NUMBER, STRING)) {
		struct expression *lit = malloc(sizeof(struct expression));
		ASSERT(lit != NULL, "Failed to allocate memory.");
		
		lit->operator = advance();
		lit->left = NULL;
		lit->right = NULL;
		
		return lit;
	}

	if (!CURRENT_TOKEN_IS(LEFT_PAREN)) {
		// To change.
		fprintf(stderr, "No expression found.");
		exit(1);
	}

	advance();
	struct expression *expr = expression();

	if (!CURRENT_TOKEN_IS(RIGHT_PAREN)) {
		// Synchronize.
		fprintf(stderr, "No right parenthesis found.");
		exit(1);
	}

	advance();
	return expr;
}
