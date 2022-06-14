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

#include "moolib/scanner.h"
#include <stdlib.h>

void token_array_grow(TokenArray* const ta);

TokenArray* token_array_init()
{
	TokenArray* ta = malloc(sizeof(TokenArray));
	ta->count = 0;
	ta->size = TOKEN_ARRAY_BUFFER_COUNT * sizeof(Token);
	ta->array = malloc(ta->size);

	return ta;
}

void token_array_add(TokenArray* const ta, const Token t)
{
	if (ta->count == (ta->size / sizeof(Token)))
		token_array_grow(ta);
	ta->array[ta->count] = t;
	ta->count++;
}

void token_array_grow(TokenArray* const ta)
{
	ta->size += TOKEN_ARRAY_BUFFER_COUNT * sizeof(Token);
	ta->array = realloc(ta->array, ta->size);
}

void token_array_del(TokenArray* ta)
{
	free(ta->array);
	free(ta);
	ta = NULL;
}
