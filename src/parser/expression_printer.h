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

/*
 * This header file will be deleted soon.
 */

#pragma once

#include "expression.h"

#include <string.h>
#include <stdio.h>

#define PRINT_EXPRESSION(expr)			\
	{					\
		char out[4096];			\
		for (int i = 0; i < 4096; i++)	\
			out[i] = '\0';		\
		treecpy(out, expr);		\
		printf("%s\n", out);		\
	}

void treecpy(char* str, struct expression *e)
{
#define WRITE_LEXEME						\
	{							\
		strncpy(str+offset, e->operator.lexeme.start,	\
			SUBSTRING_LENGTH(e->operator.lexeme));	\
		offset += SUBSTRING_LENGTH(e->operator.lexeme);	\
	}
#define ADD(c)					\
	{					\
		str[offset] = c;		\
		offset++;			\
	}
	static int offset = 0;

	if (e->left != NULL) {
		ADD('(');
		WRITE_LEXEME;
		ADD(' ');
		treecpy(str, e->left);
		ADD(' ');
		treecpy(str, e->right);
		ADD(')');
	} else if (e->right != NULL) {
		ADD('(');
		WRITE_LEXEME;
		treecpy(str, e->right);
		ADD(')');
	} else {
		WRITE_LEXEME;
	}
#undef WRITE_LEXEME
#undef SPACE
}
