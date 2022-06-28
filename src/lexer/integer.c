#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <libexpr/structs.h>

const char *expr_parse_float(const char *str, expr_literal_t *literal) {
	expr_flt_t flt;
	const char *ptr;

	errno = 0;
	flt = strtod(str, (char **)&ptr);
	if (errno == 0 && ptr != str) {
		if (literal) {
			literal->lit.flt_val = flt;
			literal->type = EXPR_LIT_FLT;
		}
		return ptr;
	}
	return NULL;
}

const char *expr_parser_num(const char *str, expr_literal_t *literal) {
	const char *ptr;
	expr_int_t num;

	errno = 0;
	num = strtol(str, (char **)&ptr, 0);
	if (errno)
		return (NULL);

	if (ptr[0] == '.')
		return expr_parse_float(str, literal);
	if (!isspace(ptr[0]) && !ispunct(ptr[0]) && ptr[0] != 0)
		return NULL;

	if (literal) {
		literal->lit.int_val = num;
		literal->type = EXPR_LIT_INT;
	}

	return ptr;
}