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

#include <stdlib.h>

static void *PARSER_ERROR = (int[]){0};

static struct statement *declaration();
static struct statement *variable_declaration();
static struct statement *statement();
static struct statement *expression_statement();
static struct statement *write_statement();

struct statement *get_next_stmt()
{
	return declaration();
}

void free_statement(struct statement *s)
{
	free_expression(s->expression);
	free(s);
	s = NULL;
}

static struct statement *declaration()
{
	struct statement *stmt;

	if (CURRENT_TOKEN_IS(END_OF_FILE))
		return NULL;

	if (CURRENT_TOKEN_IS(FLUID))
		stmt = variable_declaration();
	else
		stmt = statement();

	if (stmt == PARSER_ERROR) {
		synchronize();
		return NULL;
	}

	return stmt;
}

/* Only called if the current token is FLUID. */
struct statement *variable_declaration()
{
	struct statement *stmt = malloc(sizeof(struct statement));

	stmt->l_token = parser_advance(); /* FLUID token. */
	stmt->expression = get_next_expr();

	if (!CURRENT_TOKEN_IS(SEMICOLON)) {
		report(CURRENT_LINE, "; expected.");
		free(stmt);
		return PARSER_ERROR;
	}
	
	stmt->r_token = parser_advance();

	return stmt;}

struct statement *statement()
{
	if (CURRENT_TOKEN_IS(WRITE))
		return write_statement();
	return expression_statement();
}

/* Only called if the current token is WRITE. */
static struct statement *write_statement()
{
	struct statement *stmt = malloc(sizeof(struct statement));

	stmt->l_token = parser_advance(); /* WRITE token. */
	stmt->expression = get_next_expr();

	if (!CURRENT_TOKEN_IS(SEMICOLON)) {
		report(CURRENT_LINE, "; expected.");
		free(stmt);
		return PARSER_ERROR;
	}
	
	stmt->r_token = parser_advance();

	return stmt;
}

static struct statement *expression_statement()
{
	struct statement *stmt = malloc(sizeof(struct statement));

	stmt->l_token = NULL;
	stmt->expression = get_next_expr();

	if (!CURRENT_TOKEN_IS(SEMICOLON)) {
		report(CURRENT_LINE, "; expected.");
		free(stmt);
		return PARSER_ERROR;
	}

	stmt->r_token = parser_advance();

	return stmt;
}
