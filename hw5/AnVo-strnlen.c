/*	$OpenBSD: strnlen.c,v 1.5 2014/06/10 04:17:37 deraadt Exp $	*/

/*
 * Copyright (c) 2010 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>

#include <string.h>




/*
 * function: strnlen
 *
 * 	- strnlen takes two arguments: a pointer to a string and a maximum length
 * 		- argument 1: str
 * 			- str, a pointer to char which is also interpreted as string.
 * 			- str is labeled "const" which indicates that the function strnlen
 * 				will not modify the char pointee that is passed to it.
 * 		- argument 2: maxlen
 * 			- maxlen, an unsigned integer of type: size_t
 *
 * 	- strnlen will return an unsigned integer of type: size_t
 * 	- strnlen returns the length of the string str only if it is less than
 * 		the max length maxlen; otherwise, strnlen returns maxlen.
 * 		NOTE: the NULL terminator is not counted.
 */
size_t
strnlen(const char *str, size_t maxlen)
{




/*
 * (local) variable: cp
 *
 * 	- cp, a pointer to a char (string)
 * 	- since cp is a local variable, meaning it is available only inside
 * 		the scope of the function strnlen, it will be discarded when the
 * 		function strnlen concludes its return.
 */
	const char *cp;




/*
 * according to section 6.8.5.3 in the C11 draft standard on page 151, the
 * 	for statement is stated as:
 *  		for( clause-1 ; expression-2 ; expression-3 ) statement
 *
 * this for statement below, however, contains an empty loop body
 * 	at the "statement" field.
 * 	this means that the actions of the loop occur inside the "iteration
 * 		statement" (the three clauses).
 *
 * clause-1: cp = str
 * 	- clause-1 specifies initialization for the loop, possibly declaring
 * 		one or more variables for use in the loop.
 * 	- here, the local variable cp is initialized to be that of the local
 * 		copy of the pointer str. clause-1 is executed once, before
 * 		expression-2 is evaluated for the first time because expression-2
 * 		is dependent on the initialization of the local variable cp.
 *
 * expression-2: maxlen != 0 && *cp != '\0'
 * 	- expression-2, "the controlling expression," specifies an evaluation
 * 		made before each iteration, such that execution of the loop
 * 		continues until the expression compares equal to 0 (false).
 * 		this means if the expression becomes false, the loop breaks.
 * 	- here, if expression-2 becomes false, meaning if EITHER maxlen equals
 * 		zero OR if cp points to a null terminator '\0', the loop breaks.
 *
 * expression-3: cp++, maxlen--
 * 	- expression-3 specifies an operation that is performed after each
 * 		iteration.
 * 	- expression-3 is dependent on expression-2 for if expression-2
 * 		becomes false, expression-3 is not executed.
 * 	- here, while expression-2 is true over each iteration, expression-3
 * 		increments cp by 1 to point to the next char in the string and
 * 		maxlen is decremented by 1 after each iteration. once
 * 		expression-2 becomes false, expression-3 does not get executed
 * 		afterwards (cp is not incremented and maxlen is not decremented).
 * 	- with maxlen being decremented by 1 after each iteration, maxlen
 * 		will become zero at some point which will break the loop, unless
 * 		the loop has broken before such (cp points to a null terminator).
 * 		decrementing maxlen is used as a control to keep a finite loop
 * 			possible (infinite loop not possible).
 *
 * 	- apart from expression-3 of the C11 draft standard for statement,
 * 		which specifies AN operation that is performed after each
 * 		iteration, expression-3 of the for loop below specifies TWO
 * 		operations, separated by a comma, that are performed after
 * 		each iteration.
 * 	- according to section 6.5.17 in the C11 draft standard on page 105,
 * 		the two operations are evaluated in the sequence:
 * 			- first: cp++
 * 			- second: maxlen--
 *
 * within the "statement" field of the for statement, the semicolon
 * 	terminates "statement".
 */
	for(cp = str; maxlen != 0 && *cp != '\0'; cp++, maxlen--)
		;
/*
 * recap of the for statement:
 * 	- after each loop iteration, cp points to the next char in the string
 * 		and the max length maxlen is reduced by one each time.
 * 	- if EITHER maxlen becomes zero OR cp points to a null terminator,
 * 		the loop will end,
 * 	- if the loop ends, it means that cp will point to EITHER a char
 * 		in the string before the end of the string (maxlen must be zero)
 * 		OR cp will point to the null terminator (indicating end of
 * 		the string).
 */




/*
 * 	- cp and str are both pointers, and by the operation of (cp - str), this
 * 		is pointer subtraction (two pointers that are subtracted).
 * 		according to paragraph 9 of section 6.5.6 in the C11 draft standard
 * 		on page 93, it states:
 * 			"When two pointers are subtracted, both shall point to elements of
 * 				the same array object, or one past the last element of the
 * 				array object; the result is the difference of the subscripts of
 * 				the two array elements."
 * 		the case here is (cp - str), with cp and str both to point to the
 * 			same objects in the same array (here, string).
 *
 * 	- since cp and str are char pointers, one array element is one byte,
 * 		meaning one char. from this, the result is how chars (bytes) are
 * 		between the beginning of the string str, and cp (this is EITHER
 * 		the length of the string str, or the max length maxlen bytes).
 * 	- according to paragraph 9 in section 6.5.6 in the C11 draft standard
 * 		on page 93, it states:
 * 			"The size of the result is implementation-defined, and its
 * 				type (a signed integer type) ptrdiff_t defined in <stddef.h>
 * 				header."
 * 		thus, since ptrdiff_t is the (standard) return type, it must be
 * 			casted to match the required return type of the function strnlen
 * 			that is size_t. (which is done by parenthesizing the specified
 * 			return type and placing it to the left of the result to be returned.
 */
	return (size_t)(cp - str);
}
