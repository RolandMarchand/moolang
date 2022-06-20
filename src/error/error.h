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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

enum error {
	OK = 0,
	FAILED,
	ERR_UNEXPECTED_CHARACTER,
	ERR_UNTERMINATED_STRING,
	ERR_WRONG_ARGS_COUNT,
	ERR_EXPRESSION_NOT_FOUND
};

#define ERR_UNEXPECTED_CHARACTER(line, character)			\
	{								\
		fprintf(stderr,"Unexpected character '%c' at line %d\n", \
			character, line);				\
		exit(ERR_UNEXPECTED_CHARACTER);				\
	}

#define ERR_EXPECTED_CHARACTER(line, unexpected, expected)		\
	{								\
		fprintf(stderr,"Unexpected character '%c' at line %d. \
Expected character '%c'.\n",						\
			unexpected, line, expected);			\
		exit(ERR_UNEXPECTED_CHARACTER);				\
	}

#define ERR_EXPRESSION_NOT_FOUND				\
	{							\
		fprintf(stderr, "Expression not found.\n");	\
		exit(ERR_EXPRESSION_NOT_FOUND);			\
	}

#define ERR_UNTERMINATED_STRING(line)					\
	{								\
		fprintf(stderr, "Unterminated string at line %d\n.",	\
			line);						\
		exit(ERR_UNTERMINATED_STRING);				\
	}

#define ERR_WRONG_ARGS_COUNT(count, expected)				\
	{								\
		fprintf(stderr, "Wrong amount of args. Got %d, expected %d.\n", \
			count, expected);				\
		exit(ERR_WRONG_ARGS_COUNT);				\
	}

#define FAIL_OPEN_FILE(filename)		\
	{					\
		perror(filename);		\
		exit(EXIT_FAILURE);		\
	}

#define FAIL_FREE_FILE(filename)		\
	{					\
		perror(filename);		\
		exit(EXIT_FAILURE);		\
	}

#define FAIL_CLOSE_FILE(file_desc)			\
	{						\
		char __err__[6];			\
		sprintf(__err__, "%d", file_desc);	\
		perror(__err__);			\
	}

#define FAIL_ALLOC					\
	{						\
		errno = ENOMEM;				\
		perror("Failed to allocate memory.");	\
		exit(EXIT_FAILURE);			\
	}
