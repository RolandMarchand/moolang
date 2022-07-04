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

/** \file
    A programming language
*/

/** \mainpage
    # About Moolang
    Moolang is an educational programming language designed and written by
    [Moowool L. Summerwheat](https://moowool.info/) to teach developers C
    programming and compiler design.

    Moolang aims for code quality first and foremost, and makes use of excellent
    basic and advanced practices to optimize the educational value of the
    codebase.

    # Compiling
    Moolang uses CMake and is written for Linux with glibc.
    TODO: find the minimum compatible build.
    
    To build, run those commands in order.
    \code
    cd /path/to/moolang
    mkdir build
    cmake -S . -B build
    cd build
    make -j$(nproc)
    \endcode
    
    # License
    > Copyright (c) 2022, Roland Marchand <roland.marchand@protonmail.com>
    > 
    > Redistribution and use in source and binary forms, with or without<br />
    > modification, are permitted provided that the following conditions<br />
    > are met:
    > 
    > 1. Redistributions of source code must retain the above copyright<br />
    >    notice, this list of conditions and the following disclaimer.
    > 
    > THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND<br />
    > ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE<br />
    > IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE<br />
    > ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE<br />
    > FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL<br />
    > DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS<br />
    > OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)<br />
    > HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT<br />
    > LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY<br />
    > OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF<br />
    > SUCH DAMAGE.
 */

#include "scanner/scanner.h"
#include "parser/parser.h"

#include <assert.h>

int main(int argc, char *argv[])
{
	assert(argc == 2);

	struct scan *s = scan_init(argv[1]);
	struct statement_array *e = parse(s);

	scan_del(s);
}
