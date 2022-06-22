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

int __TOKEN_IS__(struct token *tok, TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
