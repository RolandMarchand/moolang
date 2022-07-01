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

#include "src/macros.h"


static void statement_array_grow(struct statement_array *sa);

struct statement_array *statement_array_init()
{
	struct statement_array *sa = malloc(sizeof(struct statement_array));

	ASSERT(sa != NULL, "Unable to allocate memory for statement_array.");

	sa->count = 0;
	sa->size = STATEMENT_ARRAY_BUFFER_COUNT * sizeof(struct statement);
	sa->array = malloc(sa->size);

	ASSERT(sa->array != NULL, "Unable to allocate memory for statement_array.");

	return sa;
}

void statement_array_add(struct statement_array *sa, struct statement s)
{
	if (sa->count == (sa->size / sizeof(struct statement)))
		statement_array_grow(sa);
	sa->array[sa->count] = s;
	sa->count++;
}

static void statement_array_grow(struct statement_array *sa)
{
	sa->size += STATEMENT_ARRAY_BUFFER_COUNT * sizeof(struct statement);
	sa->array = realloc(sa->array, sa->size);

	ASSERT(sa->array != NULL, "Unable to grow statement_array.");
}

void statement_array_del(struct statement_array *sa)
{
	free(sa->array);
	free(sa);
	sa = NULL;
}
