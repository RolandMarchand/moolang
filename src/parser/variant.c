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

#include "variant.h"
#include "parser.h"
#include "src/macros.h"

#include <stdlib.h>
#include <string.h>

static void free_string(struct variant *var);

struct variant *variant_new(char type)
{
	ASSERT(type == VARIANT_TYPE_NUMBER || type == VARIANT_TYPE_STRING \
	       || type == VARIANT_TYPE_BOOL,
	       "Variant of this type is not \
allowed. Please choose a variant of type VARIANT_TYPE_{NUMBER,STRING,BOOL}.");

	struct variant* var = malloc(sizeof(struct variant));
	*var = (struct variant){.type = type, .value.number = 0};

	return var;
}

void variant_set_number(struct variant *var, double num)
{
	free_string(var);

	var->type = VARIANT_TYPE_NUMBER;
	var->value.number = num;
}

void variant_set_string(struct variant *var, char* str)
{
	free_string(var);

	char *s = malloc(strlen(str) + 1);

	var->type = VARIANT_TYPE_STRING;
	var->value.string = s;
}


void variant_set_bool(struct variant *var, char b)
{
  ASSERT(b == BOOL_YES || b == BOOL_NO, "The bool value must either be \
BOOL_YES or BOOL_NO");

	free_string(var);

	var->type = VARIANT_TYPE_BOOL;
	var->value.number = b;
}

double variant_get_number(struct variant* var)
{
	ASSERT(var->type == VARIANT_TYPE_NUMBER, "The variant's type must be \
VARIANT_TYPE_NUMBER.");
	return var->value.number;
}

char *variant_get_string(struct variant *var)
{
	ASSERT(var->type == VARIANT_TYPE_STRING, "The variant's type must be \
VARIANT_TYPE_STRING.");
	return var->value.string;
}

char variant_get_bool(struct variant* var)
{
	ASSERT(var->type == VARIANT_TYPE_BOOL, "The variant's type must be \
VARIANT_TYPE_BOOL.");
	return var->value.bool;
}

static void free_string(struct variant *var)
{
	if (var->type != VARIANT_TYPE_STRING)
		return;

	free(var->value.string);
}
