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
#include "../error_handling.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct source *source_new(const char *file)
{
	struct stat sb;
	CHECK_ERROR_AND_PERFORM(stat(file, &sb) == -1,
				perror(file);
		);

	int fd = open(file, O_RDONLY);
	char *code = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	struct source *sf = malloc(sizeof(struct source));
	*sf = (struct source){
		.string = code,
		.size = sb.st_size,
		.file_descriptor = fd
	};

	return sf;
}

void source_close(struct source *sf)
{
	CHECK_ERROR_AND_PERFORM(munmap(sf->string, sf->size) == -1,
				char i[25];
				sprintf(i, "%p", sf->string);
				perror(i);
		);
	CHECK_ERROR_AND_PERFORM(close(sf->file_descriptor) == -1,
				char i[25];
				sprintf(i, "%d", sf->file_descriptor);
				perror(i);
		);
	free(sf);
	sf = NULL;
}
