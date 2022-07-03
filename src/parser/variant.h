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
    The interface for the \ref variant struct, which consists of setters and getters.
*/

#pragma once

#define VARIANT_TYPE_NUMBER 'n'
#define VARIANT_TYPE_STRING 's'
#define VARIANT_TYPE_BOOL 'b'

#define BOOL_YES 'y'
#define BOOL_NO 'n'

/** The variant structure.<br/>
    A dynamic data container supporting all of Moolang's types.<br/>
    Variants are allocated using variant_new() and freed with variant_del().
*/
struct variant {
	/** The value union is the variant's payload.<br/>
	    It holds all variant data types.*/
	union value {
		double number; /**< All numbers in moolang are double precision floating points.*/
		char *string; /**< Manually allocated string from variant_new().*/
		char bool; /**< Can either be BOOL_YES or BOOL_NO.*/
	} value;
	char type; /**< The variant's type. Possible values are VARIANT_TYPE_{NUMBER,STRING,BOOL}.*/
};

/** Instance a manually allocated variant of the desired type.
    <br/>This command performs manual memory allocations, and the
    resulting pointer must be passed to variant_del().
    <br/>variant.value is initialized to 0.

    \sa variant_del().
    \sa variant

    \param Type, the desired type of the variant.
    <br>possible values are VARIANT_TYPE_[NUMBER STRING BOOL].

    \return Variant: Pointer to a manually allocated \ref variant.
*/
struct variant *variant_new(char type);

/** Cleanly delete the variant returned by variant_new().

    \sa variant_new().
    \sa variant

    \param var, a pointer to the variant to delete.
*/
void variant_del(struct variant *var);

/** Override the current value of the variant with a number.<br/>
    Changes the type of the variant to VARIANT_TYPE_NUMBER.

    \sa variant

    \param var: A pointer to the variant to set the value.
    \param num: Number to set variant.value to.
*/
void variant_set_number(struct variant *var, double num);

/** Override the current value of the variant with a string.<br/>
    Changes the type of the variant to VARIANT_TYPE_STRING.

    \sa variant

    \param var: A pointer to the variant to set the value.
    \param num: char* to set variant.value to.
*/
void variant_set_string(struct variant *var, char *str);

/** Override the current value of the variant with a boolean.<br/>
    Changes the type of the variant to VARIANT_TYPE_BOOLEAN.

    \sa variant

    \param var: A pointer to the variant to set the value.
    \param num: Boolean to set variant.value to.
*/
void variant_set_bool(struct variant *var, char b);

/** Return the value of the variant.

    \sa variant

    \param var: A pointer to the variant holding the returned value.
    \return variant::value.number
*/
double variant_get_number(struct variant *var);

/** Override the current value of the variant with a boolean.<br/>
    Changes the type of the variant to VARIANT_TYPE_BOOLEAN.

    \sa variant

    \param var: A pointer to the variant holding the returned value.
    \return variant::value.string
*/
char *variant_get_string(struct variant *var);

/** Override the current value of the variant with a boolean.<br/>
    Changes the type of the variant to VARIANT_TYPE_BOOLEAN.

    \sa variant

    \param var: A pointer to the variant holding the returned value.
    \return variant::value.bool
*/
char variant_get_bool(struct variant *var);
