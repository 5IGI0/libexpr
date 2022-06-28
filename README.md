# LIBEXPR

**libexpr** - A library for expression evaluation written in ANSI C.

## Features

- variables
- floats and integers
- all operators supported by the C language (except for `++` and `--`)
- parentheses

## Usage

There is 2 ways to use this library.

first one is the low-level way using \
``expr_tokenize``, ``expr_compile_tokens``, ``expr_free_tokens``
``expr_evaluate_node`` and ``expr_free_nodes`` 

the other one is the high-level way using \
``expr_easy_eval(_float/_int)`` or
``expr_easy_compile`` with ``expr_evaluate_node`` and ``expr_free_nodes``

(see [expr.h](./include/libexpr/expr.h) and [easy.h](./include/libexpr/easy.h) for prototypes)

Here's an example of the high-level way

```c
#include <stdio.h>

#include <libexpr/expr.h> /* EXPR_VAR_FLT() */
#include <libexpr/easy.h> /* expr_easy_eval() */

expr_variable_t vars[] = {
	/* this macro doesn't work in ANSI C */
	EXPR_VAR_FLT("pi", 3.1415926)
};

int main(int argc, char **argv) {
	int i;
	expr_literal_t literal;

	for (i = 1; i < argc; i++) {
		if (expr_easy_eval(argv[i], vars, sizeof(vars)/sizeof(vars[0]), &literal) < 0)
			fprintf(stderr, "%s: an error occured!\n", argv[i]);
		else if (literal.type == EXPR_LIT_FLT)
			printf("%s: %f\n", argv[i], literal.lit.flt_val);
		else
			printf("%s: %ld\n", argv[i], literal.lit.int_val);
	}
	return 0;
}
```