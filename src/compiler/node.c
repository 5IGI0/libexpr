#include <stdlib.h>
#include <string.h>

#include <libexpr/expr.h>

#include "compiler.h"

/* not in standard libc specifications */
static char *my_strdup(const char *str) {
	size_t len = strlen(str) + 1;
	char *ret = malloc(len);

	if (ret)
		memcpy(ret, str, len);

	return ret;
}

static int compile_literal(expr_token_t *token, expr_node_t **nodeptr) {
	expr_node_t *node = calloc(1, sizeof(expr_node_t));

	if (!node) return -1;

	node->type = EXPR_NODE_LIT;
	node->data.lit = token->data.literal;
	nodeptr[0] = node;
	return 1;
}

static int compile_opr(
	expr_token_t *token,
	expr_node_t **nodeptr
) {
	expr_node_t *node = calloc(1, sizeof(expr_node_t));

	if (!node) return -1;

	node->type = EXPR_NODE_OPR;
	node->data.opr = token->data.opr;
	nodeptr[0] = node;
	return 1;
}

static int compile_var(
	expr_token_t *token,
	expr_node_t **nodeptr
) {
	expr_node_t *node = calloc(1, sizeof(expr_node_t));

	if (!node) return -1;

	node->type = EXPR_NODE_VAR;
	node->data.var = my_strdup(token->data.variable);

	if (!node->data.var)
		return (free(node), -1);
	
	nodeptr[0] = node;
	return 1;
}

static int compile_sub(
	expr_token_t *tokens,
	size_t tokenlen,
	int depth,
	expr_node_t **nodeptr
) {
	expr_node_t *node = calloc(1, sizeof(expr_node_t));
	int tmp;

	if (!node) return -1;
	if ((tmp = expr_compile_tokens(tokens+1, tokenlen-1, depth+1, &node->data.sub)) < 0)
		return (free(node), -1);
	
	node->type = EXPR_NODE_SUB;
	*nodeptr = node;
	return tmp + 1;
}

int expr_compile_node(
	expr_token_t *tokens,
	size_t tokenlen,
	int depth,
	expr_node_t **nodeptr
) {
	expr_token_type_t type = tokens[0].type;

	switch (type) {
		case EXPR_TKN_LITERAL:    return compile_literal(tokens, nodeptr);
		case EXPR_TKN_OPR:        return compile_opr(tokens, nodeptr);
		case EXPR_TKN_VAR:        return compile_var(tokens, nodeptr);
		case EXPR_TKN_OPEN_BRKTS: return compile_sub(tokens, tokenlen, depth, nodeptr);
		default: break;
	}
	return -1;
}