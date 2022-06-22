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

#include "source.h"
#include "macros.h" // for ASSERT

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct source *source_new(const char *file)
{
	struct stat sb;
	ASSERT(stat(file, &sb) != -1, "Invalid input file.");

	int fd = open(file, O_RDONLY);
	char *code = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	struct source *sf = malloc(sizeof(struct source));

	ASSERT(fd != -1 && code != MAP_FAILED && sf != NULL,
	       "Failed to allocate source file.");

	*sf = (struct source){
		.string = code,
		.file_name = file,
		.size = sb.st_size,
		.file_descriptor = fd
	};

	return sf;
}

void source_close(struct source *sf)
{
	ASSERT(munmap(sf->string, sf->size) != -1 &&
	       close(sf->file_descriptor) != -1, "Unable to close source file.");

	free(sf);
	sf = NULL;
}
