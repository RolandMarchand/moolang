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

#pragma once
#include "token.h"

#define TOKEN_ARRAY_BUFFER_COUNT 8

struct token_array {
	struct token *array;
	int size;
	int count;
};

struct token_array *token_array_init();
void token_array_add(struct token_array *const ta, const struct token t);
void token_array_del(struct token_array *ta);
