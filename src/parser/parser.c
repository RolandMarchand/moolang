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
#include "expression_printer.h"

#include "macros.h"

struct expression *parse(struct scan *s)
{
	parser_tokens = *(s->tokens);
	struct expression *e = get_next_expr();
	PRINT_EXPRESSION(e);
	return e;
}

struct token advance()
{
	ASSERT(parser_tokens.count > 0, "Not enough tokens.");

	struct token t = *CURRENT_TOKEN;

	if (parser_tokens.count == 1)
		return t;

	++(parser_tokens.array);
	--(parser_tokens.count);
	parser_tokens.size -= sizeof(struct token);

	return t;
}

int __TOKEN_IS__(const struct token *tok, const TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
