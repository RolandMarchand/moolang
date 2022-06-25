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
