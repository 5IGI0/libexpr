#include <libexpr/expr.h>

int expr_easy_compile(const char *str, expr_node_t **nodes) {
	expr_token_t *tokens = NULL;
	size_t tokenlen;

	if (expr_tokenize(str, &tokens, &tokenlen) < 0)
		return -1;

	if (expr_compile_tokens(tokens, tokenlen, 0, nodes) < 0)
		return (expr_free_tokens(tokens, tokenlen), -1);

	return 0;
}

int expr_easy_eval(const char *str, expr_variable_t *vars, size_t varlen,
		expr_literal_t *ret) {
	expr_node_t *nodes;

	if (expr_easy_compile(str, &nodes) < 0)
		return -1;
	
	if (expr_evaluate_node(nodes, vars, varlen, ret) < 0)
		return (expr_free_nodes(nodes), -1);

	expr_free_nodes(nodes);

	return (0);
}

int expr_easy_eval_int(const char *str, expr_variable_t *vars, size_t varlen,
		expr_int_t *ret) {
	expr_literal_t literal;

	if (expr_easy_eval(str, vars, varlen, &literal) < 0)
		return (-1);

	if (literal.type == EXPR_LIT_FLT)
		*ret = (expr_int_t)literal.lit.flt_val;
	else
		*ret = literal.lit.int_val;
	return 0;
}

int expr_easy_eval_float(const char *str, expr_variable_t *vars, size_t varlen,
		expr_flt_t *ret) {
	expr_literal_t literal;

	if (expr_easy_eval(str, vars, varlen, &literal) < 0)
		return (-1);

	if (literal.type == EXPR_LIT_FLT)
		*ret = literal.lit.flt_val;
	else
		*ret = (expr_flt_t)literal.lit.int_val;
	return 0;
}