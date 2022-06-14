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
#include "moolib/error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_TOKEN(KEYWORD) (Token){.type=KEYWORD, .lexeme=(substring){.start=start, .end=current}, .line=line}

static int line = 0;
static int column = 0; // To remove?
static char* start = 0;
static char* current = 0;

void scan_tokens(const Source* const src, TokenArray* const ta);
Token get_token();
char advance();
Token string();
Token digit();
Token identifier();
TokenType get_keyword_type(char* str);
int is_digit(char c);
int is_alpha(char c);

void scanner(const char* filename)
{
	Source* src = source_new(filename);
	TokenArray* ta = token_array_init();
	current = src->string;
	scan_tokens(src, ta);

	for (int i = 0; i < ta->count; i++) {
		substring sbstr = ta->array[i].lexeme;
		char str[SUBSTRING_LENGTH(sbstr) + 1];
		get_substring(str, sbstr);
		printf("%d - %s\n", ta->array[i].line, str);
	}

	token_array_del(ta);
	source_close(src);	
}

void scan_tokens(const Source* const src, TokenArray* const ta)
{
	while(current[0] != '\0') {
		start = current;
		Token t = get_token();
		token_array_add(ta, t);
	}
}

Token get_token()
{
	char error[64];

	switch (current[0]) {
		// Single-character tokens.
	case '(': advance(); return GET_TOKEN(LEFT_PAREN);
	case ')': advance(); return GET_TOKEN(RIGHT_PAREN);
	case '{': advance(); return GET_TOKEN(LEFT_BRACE);
	case '}': advance(); return GET_TOKEN(RIGHT_PAREN);
	case ',': advance(); return GET_TOKEN(COMMA);
	case '.': advance(); return GET_TOKEN(DOT);
	case '-': advance(); return GET_TOKEN(MINUS);
	case '+': advance(); return GET_TOKEN(PLUS);
	case ';': advance(); return GET_TOKEN(SEMICOLON);
	case '*': advance(); return GET_TOKEN(STAR);
	case '\0': return GET_TOKEN(END_OF_FILE);

		// One or two character tokens.
	case '!':
		advance();
		return advance() == '=' ? GET_TOKEN(BANG_EQUAL) : GET_TOKEN(BANG);
	case '=':
		advance();
		return advance() == '=' ? GET_TOKEN(EQUAL_EQUAL) : GET_TOKEN(EQUAL);
	case '>':
		advance();
		return advance() == '=' ? GET_TOKEN(GREATER_EQUAL) : GET_TOKEN(GREATER);
	case '<':
		advance();
		return advance() ? GET_TOKEN(LESS_EQUAL) : GET_TOKEN(LESS);
	case '/':
		advance();
		if (current[0] != '/') return GET_TOKEN(SLASH);
		while (current[0] != '\n' && current[0] != '\0') advance();
		return get_token();

		// Ignored.
	case ' ':
	case '\t':
	case '\f':
	case '\v':
	case '\r':
		advance();
		start = current;
		return get_token();
	case '\n':
		advance();
		start = current;
		column = 0;
		line++;
		return get_token();

		// Literals.
	case '"': advance(); return string();

	default:
		if (is_digit(current[0])) return digit();
		if (is_alpha(current[0])) return identifier();
		sprintf(error, "Line %d: Unexpected character '%c'.", line, current[0]);
		CHECK_ERROR_AND_PERFORM(1, perror(error););
	}
}

char advance()
{
	if (current[0] == '\0') return '\0';

	current++;
	
	return current[-1];
}

Token string()
{
	while (current[0] != '"' && current[0] != '\0') {
		if (current[0] == '\n') line++;
		if (current[0] == '\\') advance();
		advance();
	}
	CHECK_ERROR_AND_PERFORM(current[0] == '\0',
				perror("Unterminated string."););
	advance();
	return GET_TOKEN(STRING);
}

Token digit()
{
	while (is_digit(current[0])) advance();

	if (current[0] == '.') {
		CHECK_ERROR_AND_PERFORM(!is_digit(current[1]), {
				char err[128];
				sprintf(err, "Line %d: Unexpected character '%c'. Digit expected.", line + 1, current[1]);
				perror(err);
			});
		while (is_digit(current[0])) advance();
	}

	return GET_TOKEN(NUMBER);
}

Token identifier()
{
	while (is_alpha(current[0]) || is_digit(current[0])) advance();

	substring* sbstr = &(substring){.start=start, .end=current};
	int s = SUBSTRING_LENGTH((*sbstr)) + 1; // +1 for '\0'
	char str[s];
	get_substring(str, *sbstr);

	TokenType t = get_keyword_type(str);
	return GET_TOKEN(t);
}

TokenType get_keyword_type(char* str)
{
#define GET(kwstr, tk) if (strcmp(str, kwstr) == 0) return tk;

	GET("blueprint", BLUEPRINT);
	GET("con", CON);
	GET("dis", DIS);
	GET("else", ELSE);
	GET("fluid", FLUID);
	GET("if", IF);
	GET("me", ME);
	GET("no", NO);
	GET("procedure", PROCEDURE);
	GET("produce", PRODUCE);
	GET("solid", SOLID);
	GET("while", WHILE);
	GET("write", WRITE);
	GET("yes", YES);

	return IDENTIFIER;
		
#undef GET
}

int is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int is_alpha(char c)
{
	return (c >= 'A' && c <= 'z') || c == '_';
}

