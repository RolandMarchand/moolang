/*
 * Copyright (c) 2022, Roland Marchand <roland.marchand@protonmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

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
	struct expression *left = comparison();

	while (CURRENT_TOKEN_IS(BANG_EQUAL, EQUAL_EQUAL)) {
		struct token operator = advance();
		struct expression *right = comparison();

		struct expression *expr = malloc(sizeof(struct expression));

		ASSERT(expr != NULL, "Failed to allocate memory.");

		*expr = (struct expression){
			.operator = operator,
			.left = left,
			.right = right
		};
		left = expr;
	}

	return left;
}

static struct expression *comparison()
{
	struct expression *left = term();

	while (CURRENT_TOKEN_IS(\
			GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
		struct token operator = advance();
		struct expression *right = term();

		struct expression *expr = malloc(sizeof(struct expression));

		ASSERT(expr != NULL, "Failed to allocate memory.");

		*expr = (struct expression){
			.operator = operator,
			.left = left,
			.right = right
		};
		left = expr;
	}

	return left;
}

static struct expression *term()
{
	struct expression *left = factor();

	while (CURRENT_TOKEN_IS(PLUS, MINUS)) {
		struct token operator = advance();
		struct expression *right = factor();

		struct expression *expr = malloc(sizeof(struct expression));

		ASSERT(expr != NULL, "Failed to allocate memory.");

		*expr = (struct expression){
			.operator = operator,
			.left = left,
			.right = right
		};
		left = expr;
	}

	return left;
}

static struct expression *factor()
{
	struct expression *left = unary();

	while (CURRENT_TOKEN_IS(STAR, SLASH)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *expr = malloc(sizeof(struct expression));

		ASSERT(expr != NULL, "Failed to allocate memory.");

		*expr = (struct expression){
			.operator = operator,
			.left = left,
			.right = right
		};
		left = expr;
	}

	return left;
}

static struct expression *unary()
{
	while (CURRENT_TOKEN_IS(BANG, MINUS)) {
		struct token operator = advance();
		struct expression *right = unary();

		struct expression *expr = malloc(sizeof(struct expression));

		ASSERT(expr != NULL, "Failed to allocate memory.");

		*expr = (struct expression){
			.operator = operator,
			.right = right
		};
		return expr;
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
