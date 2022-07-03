/*
 * Copyright (c) 2022, Roland Marchand <roland.marchand@protonmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include "token_array.h"
#include "substring.h"
#include "token.h"
#include "source.h"

/** \file */

/*
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
    \sa scan_init()
    \param s is the scan returned from scan_init() that will be freed.
 */
extern void scan_del(struct scan *s);
