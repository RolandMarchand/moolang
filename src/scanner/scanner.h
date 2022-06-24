/** \file */

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

/*
 * The scan struct is the final output of the scanner. It contains the
 * source code of the program and a dynamic array of tokens.
 *
 * To get the scan struct, call the `scan_init` function.
 * To delete the scan, call the `scan_del` function. Not doing so will
 * result in a memory leak.
 *
 * The source is a structure composed of a `string` character array, a
 * `size` integer measuring physical size, and a `file_descriptor`. The source is managed
 * automatically.
 *
 * The dynamic array of tokens is similar to the source, composed of an
 * `array` of tokens, a `size` integer measuring the physical size of
 * the array, and a `count` integer, counting the number of tokens. The
 * array of tokens is managed automatically.
 *
 * Tokens of the dynamic array are structures being made of their
 * `type` (TypeToken), the `line` on which they appear in the source
 * code, and a `lexeme`. The `lexeme` is a custom structure called a
 * substring.
 *
 * Substrings are composed of two char pointers `start` and `end`
 * pointing to a section of a string. You can use `sbstrcpy` to
 * write the contents of a substring in a user-defined character array
 * of size `SUBSTRING_LENGTH(sbstr) + 1`. There is a
 * `PRINT_SUBSTRING(sbstr)` macro, to print the substring to STDOUT.
 */

#pragma once

#include "token_array.h"
#include "substring.h"
#include "token.h"
#include "source.h"

/** Final output of the scanner.
    This should not be created manually, and should only be retreived from
    scan_init().
    \sa scan_init
*/
struct scan {
	struct source *source;	/**< Pointer to the original source file. */
	struct token_array *tokens; /**< Resulting  */
};

/** Scan a Moolang source code file to identify the tokens that will be parsed
    by the parser.<br />This command performs manual memory allocations,
    and the resulting pointer must be passed to scan_del().
   \sa scan_del()
   \sa scan
   \param filename an absolute or relative path to the source code.
   \return \ref scan, a structure containing an array of the scanned tokens and the input
   file's source code.
 */
extern struct scan *scan_init(const char *filename);

/** Safely deletes the scan returned from scan_init.<br />Not doing so will result in a memory leak.
    \sa scan_init
    \param s is the scan returned from scan_init() that will be freed.
 */
extern void scan_del(struct scan *s);
