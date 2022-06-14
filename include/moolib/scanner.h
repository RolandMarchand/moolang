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

#define TOKEN_ARRAY_BUFFER_COUNT 64
#define SUBSTRING_LENGTH(sbstr) ((int)(sbstr.end - sbstr.start))

typedef enum {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS,
	PLUS, SEMICOLON, STAR, SLASH,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	CON, DIS, BLUEPRINT, IF, ELSE, YES, NO, PROCEDURE, WRITE, PRODUCE, ME,
	WHILE, FLUID, SOLID,

	END_OF_FILE
} TokenType;

typedef struct {
	char* start;
	char* end;
} substring;

typedef struct {
	TokenType type;
	substring lexeme;
	int line;
	int column;
} Token;

typedef struct {
	Token* array;
	int size;
	int count;
} TokenArray;

typedef struct {
	char* string;
	int size;
	int file_descriptor;
} Source;

void scanner(const char* filename);
void source_close(Source* sf);
Source* source_new(const char* file);
void token_array_add(TokenArray* const ta, const Token t);
TokenArray* token_array_init();
void token_array_del(TokenArray* ta);
void get_substring(char* str, substring sbstr);
TokenType get_keyword_type(char* str);
