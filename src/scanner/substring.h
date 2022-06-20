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

#include <stdio.h>

#define SUBSTRING_LENGTH(sbstr) ((int)(sbstr.end - sbstr.start))
#define PRINT_SUBSTRING(sbstr)					\
	{							\
		char __STR__[SUBSTRING_LENGTH(sbstr) + 1];	\
		get_substring(__STR__, sbstr);			\
		printf("%s", __STR__);				\
	}

struct substring {
	char *start;
	char *end;
};

void get_substring(char *str, struct substring sbstr);
