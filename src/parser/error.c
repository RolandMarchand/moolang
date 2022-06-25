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

static char error_status = 0;

char get_error_status() {
	return error_status;
}

void report(int line, char *message)
{
	error_status = 1;

	if (CURRENT_TOKEN_IS(END_OF_FILE)) {
		fprintf(stderr, "%d at end: %s", line, message);
		return;
	}

	char lexeme[SUBSTRING_LENGTH(*CURRENT_TOKEN.lexeme) + 1];
	sbstrcpy(CURRENT_TOKEN.lexeme, lexeme);

	fprintf(stderr, "%d at %s: %s", line, lexeme, message);
}

void synchronize()
{
	advance();

	while (!CURRENT_TOKEN_IS(END_OF_FILE)) {
		switch (*CURRENT_TOKEN.type) {
		case SEMICOLON:
			advance();
		case BLUEPRINT:
		case PROCEDURE:
		case IF:
		case WHILE:
		case FLUID:
		case WRITE:
		case PRODUCE:
			return;
		default:
			advance();
		}
	}
}
