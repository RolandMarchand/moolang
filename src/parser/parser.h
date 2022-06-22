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

#include "error.h"
#include "expression.h"

#include "scanner/scanner.h"

#define CURRENT_LINE *CURRENT_TOKEN.line
#define CURRENT_TOKEN &parser_tokens.array[0]
#define CURRENT_TOKEN_IS(...) __TOKEN_IS__(CURRENT_TOKEN,		\
					   (TokenType[]){__VA_ARGS__, -1})

extern struct token_array parser_tokens;

struct expression *parse(struct scan *s);
struct token advance();
int __TOKEN_IS__(struct token *tok, TokenType type[]);
