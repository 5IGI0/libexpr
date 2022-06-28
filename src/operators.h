#ifndef __LIBEXPR_OPERATORS_H__
#define __LIBEXPR_OPERATORS_H__

#include <libexpr/structs.h>

const char *expr_parse_opr(const char *str, expr_opr_t *opr);
int expr_get_priority_lvl(expr_opr_t opr);
expr_literal_t expr_operate(expr_opr_t opr, expr_literal_t a, expr_literal_t b);

/* WARNING: it is a macro! */
#define IS_OPR_CHAR(x) ( \
		(str[0] >= '<' && str[0] <= '>') || \
		( \
			str[0] >= '*' && \
			str[0] <= '-' && \
			str[0] != ',' \
		) || \
		str[0] == '/' || \
		str[0] == '%' || \
		str[0] == '&' || \
		str[0] == '|' || \
		str[0] == '^' || \
		str[0] == '!' \
	)

#endif