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
