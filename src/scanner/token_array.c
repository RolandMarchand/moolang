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

#include "token_array.h"
#include "error/error.h"

#include <stdlib.h>

static void token_array_grow(struct token_array *const ta);

struct token_array *token_array_init()
{
	struct token_array *ta = malloc(sizeof(struct token_array));

	if (ta == NULL) {
		FAIL_ALLOC;
	}

	ta->count = 0;
	ta->size = TOKEN_ARRAY_BUFFER_COUNT * sizeof(struct token);
	ta->array = malloc(ta->size);

	if (ta->array == NULL) {
		FAIL_ALLOC;
	}

	return ta;
}

void token_array_add(struct token_array *const ta, const struct token t)
{
	if (ta->count == (ta->size / sizeof(struct token)))
		token_array_grow(ta);
	ta->array[ta->count] = t;
	ta->count++;
}

static void token_array_grow(struct token_array *const ta)
{
	ta->size += TOKEN_ARRAY_BUFFER_COUNT * sizeof(struct token);
	ta->array = realloc(ta->array, ta->size);

	if (ta->array == NULL) {
		FAIL_ALLOC;
	}
}

void token_array_del(struct token_array *ta)
{
	free(ta->array);
	free(ta);
	ta = NULL;
}
