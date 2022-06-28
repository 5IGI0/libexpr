
/***
 * Add an operator:
 *   src/operators.h:
 *     1) modify the IS_OPR_CHAR()
 *   include/libexpr/structs.h:
 *     1) add your operator to `expr_opr_t`
 *   src/operators.c
 *     1) modify expr_parse_opr
 *     2) modify get_priority_lvl
 *     3) create a function for the operator
 *     4) modify expr_operate
 */

#include <string.h>

#include <libexpr/structs.h>

#define OPR_COMP(OPR_STR, OPR) \
	if (!memcmp(OPR_STR, str, sizeof(OPR_STR)-1)) \
	{ \
		if (opr) *opr = OPR; \
		return str + sizeof(OPR_STR)-1; \
	}

const char *expr_parse_opr(const char *str, expr_opr_t *opr) {
	OPR_COMP("+", EXPR_OPR_ADD);
	OPR_COMP("-", EXPR_OPR_SUB);
	OPR_COMP("*", EXPR_OPR_MULT);
	OPR_COMP("/", EXPR_OPR_DIV);
	OPR_COMP("==", EXPR_OPR_EQ);
	OPR_COMP("!=", EXPR_OPR_NEQ);
	OPR_COMP("<<", EXPR_OPR_BIT_LEFT);
	OPR_COMP(">>", EXPR_OPR_BIT_RIGHT);
	OPR_COMP(">=", EXPR_OPR_GRTR_EQ);
	OPR_COMP("<=", EXPR_OPR_LESS_EQ);
	OPR_COMP(">", EXPR_OPR_GRTR);
	OPR_COMP("<", EXPR_OPR_LESS);
	OPR_COMP("&&", EXPR_OPR_AND);
	OPR_COMP("%", EXPR_OPR_MOD);
	OPR_COMP("||", EXPR_OPR_OR);
	OPR_COMP("|", EXPR_OPR_BIT_OR);
	OPR_COMP("&", EXPR_OPR_BIT_AND);
	OPR_COMP("^", EXPR_OPR_BIT_XOR);
	return NULL;
}

int expr_get_priority_lvl(expr_opr_t opr) {
	switch (opr) {
		case EXPR_OPR_DIV :
		case EXPR_OPR_MOD :
		case EXPR_OPR_MULT: return 1;

		case EXPR_OPR_ADD:
		case EXPR_OPR_SUB: return 2;

		case EXPR_OPR_BIT_LEFT :
		case EXPR_OPR_BIT_RIGHT: return 3;

		case EXPR_OPR_LESS   :
		case EXPR_OPR_LESS_EQ:
		case EXPR_OPR_GRTR   :
		case EXPR_OPR_GRTR_EQ: return 4;

		case EXPR_OPR_EQ:
		case EXPR_OPR_NEQ: return 5;

		case EXPR_OPR_BIT_AND: return 6;
		case EXPR_OPR_BIT_XOR: return 7;
		case EXPR_OPR_BIT_OR: return 8;
		case EXPR_OPR_AND: return 9;
		case EXPR_OPR_OR: return 10;
		default: return -1;
	}
}

static expr_flt_t get_as_float(expr_literal_t a) {
	if (a.type == EXPR_LIT_FLT)
		return a.lit.flt_val;
	return (expr_flt_t) a.lit.int_val;
}

static expr_int_t get_as_int(expr_literal_t a) {
	if (a.type == EXPR_LIT_INT)
		return a.lit.int_val;
	return (expr_int_t)a.lit.flt_val;
}

#define OPR_FUNC_FROM_BUILTIN(func_name, oprator) \
static expr_literal_t func_name(expr_literal_t a, expr_literal_t b) { \
	expr_literal_t ret;\
	if (a.type == EXPR_LIT_FLT || b.type == EXPR_LIT_FLT) { \
		ret.type = EXPR_LIT_FLT; \
		ret.lit.flt_val = get_as_float(a) oprator get_as_float(b); \
	} else {\
		ret.type = EXPR_LIT_INT; \
		ret.lit.int_val = a.lit.int_val oprator b.lit.int_val; \
	} \
	return ret; \
}

OPR_FUNC_FROM_BUILTIN(opr_add,+)
OPR_FUNC_FROM_BUILTIN(opr_sub,-)
OPR_FUNC_FROM_BUILTIN(opr_div,/)
OPR_FUNC_FROM_BUILTIN(opr_mult,*)
OPR_FUNC_FROM_BUILTIN(opr_eq,==)
OPR_FUNC_FROM_BUILTIN(opr_neq,!=)
OPR_FUNC_FROM_BUILTIN(opr_less,<)
OPR_FUNC_FROM_BUILTIN(opr_less_eq,<=)
OPR_FUNC_FROM_BUILTIN(opr_greater,>)
OPR_FUNC_FROM_BUILTIN(opr_greater_eq,>=)
OPR_FUNC_FROM_BUILTIN(opr_and,&&)
OPR_FUNC_FROM_BUILTIN(opr_or,||)

#define OPR_FUNC_BUILTIN_FRC_INT(func_name, operator) \
static expr_literal_t func_name(expr_literal_t a, expr_literal_t b) { \
	expr_literal_t ret;\
	ret.type = EXPR_LIT_INT; \
	ret.lit.int_val = get_as_int(a) operator get_as_int(b); \
	return ret; \
}

OPR_FUNC_BUILTIN_FRC_INT(opr_bitwise_left,<<)
OPR_FUNC_BUILTIN_FRC_INT(opr_bitwise_right,>>)
OPR_FUNC_BUILTIN_FRC_INT(opr_bitwise_or,|)
OPR_FUNC_BUILTIN_FRC_INT(opr_bitwise_and,&)
OPR_FUNC_BUILTIN_FRC_INT(opr_bitwise_xor,^)
OPR_FUNC_BUILTIN_FRC_INT(opr_mod,%)

expr_literal_t expr_operate(expr_opr_t opr, expr_literal_t a, expr_literal_t b) {
	expr_literal_t ((*operators[])(expr_literal_t, expr_literal_t)) = {
		opr_add, opr_sub, opr_mult, opr_div, opr_mod,
		opr_eq, opr_neq, opr_greater, opr_less,
		opr_greater_eq, opr_less_eq, opr_bitwise_left,
		opr_bitwise_right, opr_and, opr_or, opr_bitwise_or,
		opr_bitwise_and, opr_bitwise_xor
	};
	return operators[opr](a,b);
}