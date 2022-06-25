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

#include "token_array.h"

#include <stdlib.h>
#include <assert.h>

static void token_array_grow(struct token_array *const ta);

struct token_array *token_array_init()
{
	struct token_array *ta = malloc(sizeof(struct token_array));

	assert(ta != NULL);

	ta->count = 0;
	ta->size = TOKEN_ARRAY_BUFFER_COUNT * sizeof(struct token);
	ta->array = malloc(ta->size);

	assert(ta->array != NULL);

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

	assert(ta->array != NULL);
}

void token_array_del(struct token_array *ta)
{
	free(ta->array);
	free(ta);
	ta = NULL;
}
