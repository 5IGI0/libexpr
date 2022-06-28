#ifndef __LIBEXPR_EVAL_H__
#define __LIBEXPR_EVAL_H__

#include <libexpr/expr.h>

typedef union {
	expr_opr_t opr;
	expr_literal_t lit;
} expr_opr_lst_t;


#endif