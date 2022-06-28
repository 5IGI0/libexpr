#include <stdlib.h>
#include <string.h>

#include <libexpr/expr.h>

#include "eval.h"
#include "../operators.h"

int expr_evaluate_node(
	expr_node_t *node,
	expr_variable_t *variables,
	size_t varlen,
	expr_literal_t *ret
);

static size_t expr_nodelen(expr_node_t *node) {
	size_t i = 0;
	while (node) {
		i++;
		node = node->right;
	}
	return i;
}

static expr_literal_t expr_get_var(const char *varname, expr_variable_t *var, size_t varlen) {
	expr_literal_t ret;

	while (varlen > 0) {
		varlen--;
		if (!strcmp(varname, var[varlen].name))
			return var[varlen].value;
	}

	ret.type = EXPR_LIT_INT;
	ret.lit.int_val = 0;
	return ret;
}

static expr_literal_t expr_resolv_node(expr_node_t *node, expr_variable_t *var, size_t varlen) {
	expr_literal_t lit;

	if (node->type == EXPR_NODE_LIT)
		return node->data.lit;
	else if (node->type == EXPR_NODE_SUB)
		return (expr_evaluate_node(node->data.sub, var, varlen, &lit), lit);
	else if (node->type == EXPR_NODE_VAR)
		return expr_get_var(node->data.var, var, varlen);

	lit.type = EXPR_LIT_INT;
	lit.lit.int_val = 0;
	return lit;
}

static expr_opr_lst_t *expr_node2opr_lst(
	expr_node_t *node,
	size_t *oprlenptr,
	expr_variable_t *var,
	size_t varlen
) {
	expr_opr_lst_t *oprlst;
	size_t oprlen;
	size_t i = 0;

	oprlen = expr_nodelen(node);
	oprlst = malloc(oprlen * sizeof(*oprlst));
	
	if (!oprlst)
		return NULL;

	while (node) {
		if (i % 2)
			oprlst[i].opr = node->data.opr;
		else
			oprlst[i].lit = expr_resolv_node(node, var, varlen);
		node = node->right;
		i++;
	}

	*oprlenptr = oprlen;
	return oprlst;
}

int expr_evaluate_node(
	expr_node_t *node,
	expr_variable_t *variables,
	size_t varlen,
	expr_literal_t *ret
) {
	expr_opr_lst_t *oprlst;
	size_t oprlen;
	size_t i;
	int	priority_lvl = 0;

	oprlst = expr_node2opr_lst(node, &oprlen, variables, varlen);
	if (!oprlst) return -1;

	while (oprlen >= 3) {
		priority_lvl++;
		i = 1;
		while (i < oprlen) {
			if (expr_get_priority_lvl(oprlst[i].opr) == priority_lvl) {
				oprlst[i - 1].lit = expr_operate(oprlst[i].opr, oprlst[i - 1].lit, oprlst[i + 1].lit);
				memmove(oprlst + i, oprlst + i + 2, (oprlen - i - 1) * sizeof(*oprlst));
				oprlen -= 2;
				i -= 2;
			}
			i += 2;
		}
	}

	*ret = oprlst[0].lit;
	free(oprlst);
	return 0;
}