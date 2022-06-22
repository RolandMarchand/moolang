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

#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_TOKEN(KEYWORD) (struct token)		\
	{						\
		.type = KEYWORD,			\
			.lexeme = (struct substring)	\
			{				\
				.start = start,		\
				.end = current		\
			},				\
			.line = line			\
			}
#define IS_DIGIT(d) (d >= '0' && d <= '9')
#define IS_ALPHA(c) ((c >= 'A' && c <= 'z') || c == '_')

static int line = 0;
static char *start = 0;
static char *current = 0;
static char valid_input = 1;

static void scan_tokens(const struct source *const src, struct token_array *const ta);
static struct token get_token();
static char advance();

static struct token string();
static struct token digit();
static struct token identifier();

TokenType get_keyword_type(char *str);

struct scan *scan_init(const char *filename)
{
	struct source *src = source_new(filename);
	current = src->string;
	struct token_array *ta = token_array_init();

	scan_tokens(src, ta);

	if (!valid_input) exit(1);

	struct scan *s = malloc(sizeof(struct scan));

	if (s == NULL) {
		fprintf(stderr,
			"Failed to allocate %lu bytes in scan_init.",
			sizeof(struct scan));
		exit(1);
	}

	*s = (struct scan){.source = src, .tokens = ta};

	return s;
}

void scan_del(struct scan *s)
{
	source_close(s->source);
	token_array_del(s->tokens);
	free(s);
}

static void scan_tokens(const struct source *const src, \
			struct token_array *const ta)
{
	while(current[0] != '\0') {
		start = current;
		struct token t = get_token();
		token_array_add(ta, t);
	}
}

static struct token get_token()
{
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
	case '/': advance(); return GET_TOKEN(SLASH);
	case '\0': return GET_TOKEN(END_OF_FILE);
	case '#':
		advance();
		while (current[0] != '\n' && current[0] != '\0') advance();
		return get_token();

		// One or two character tokens.
	case '!':
		advance();
		if (current[0] == '=') {
			advance();
			return GET_TOKEN(BANG_EQUAL);
		}
		return GET_TOKEN(BANG);
	case '=':
		advance();
		if (current[0] == '=') {
			advance();
			return GET_TOKEN(EQUAL_EQUAL);
		}
		return GET_TOKEN(EQUAL);
	case '>':
		advance();
		if (current[0] == '=') {
			advance();
			return GET_TOKEN(GREATER_EQUAL);
		}
		return GET_TOKEN(GREATER);
	case '<':
		advance();
		if (current[0] == '=') {
			advance();
			return GET_TOKEN(LESS_EQUAL);
		}
		return GET_TOKEN(LESS);

		// Ignored.
	case '\n':
		line++;
	case ' ':
	case '\t':
	case '\f':
	case '\v':
	case '\r':
		advance();
		start = current;
		return get_token();

		// Literals.
	case '"': advance(); return string();

	default:
		if (IS_DIGIT(current[0])) return digit();
		if (IS_ALPHA(current[0])) return identifier();

		// Invalid.
		valid_input = 0;
		fprintf(stderr,
			"Unexpected character %c at line %d",
			current[0], line);
		return GET_TOKEN(INVALID);
	}
}

static char advance()
{
	if (current[0] == '\0') return '\0';

	current++;

	return current[-1];
}

static struct token string()
{
	int line_begin = line; // Used to print error message.

	while (current[0] != '"') {
		if (current[0] == '\n') line++;
		if (current[0] == '\\') advance();

		if (current[0] != '\0') {
			advance();
			continue;
		}

		fprintf(stderr, "Unterminated string at line %d.", line_begin);
		return GET_TOKEN(INVALID);
	}

	advance();
	return GET_TOKEN(STRING);
}

static struct token digit()
{
	while (IS_DIGIT(current[0])) advance();

	if (current[0] != '.') goto return_digit;

	if (!IS_DIGIT(current[1])) goto return_invalid;

	while (IS_DIGIT(current[0])) advance();

return_digit:
	return GET_TOKEN(NUMBER);

return_invalid:
	fprintf(stderr,
		"Expected %c at line %d to be a number after the '.'.",
		current[1], line);
	while (IS_ALPHA(current[0]) || IS_DIGIT(current[0])) advance();
	return GET_TOKEN(INVALID);
}

static struct token identifier()
{
	while (IS_ALPHA(current[0]) || IS_DIGIT(current[0])) advance();

	struct substring *sbstr = &(struct substring){.start=start, .end=current};
	int s = SUBSTRING_LENGTH((*sbstr)) + 1; // +1 for '\0'
	char str[s];
	sbstrcpy(sbstr, str);

	TokenType t = get_keyword_type(str);
	return GET_TOKEN(t);
}

// TODO: Replace this by a hash table
TokenType get_keyword_type(char *str)
{
#define GET(kwstr, tk) if (strcmp(str, kwstr) == 0) return tk;

	GET("blueprint", BLUEPRINT);
	GET("con", CON);
	GET("dis", DIS);
	GET("else", ELSE);
	GET("fluid", FLUID);
	GET("if", IF);
	GET("me", ME);
	GET("nil", NIL);
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
