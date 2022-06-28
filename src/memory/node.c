#include <stdlib.h>

#include <libexpr/expr.h>

void expr_free_nodes(expr_node_t *nodes);

static void expr_free_node(expr_node_t *node) {
	if (node->type == EXPR_NODE_SUB)
		expr_free_nodes(node->data.sub);
	else if (node->type == EXPR_NODE_VAR)
		free(node->data.var);
	free(node);
}

void expr_free_nodes(expr_node_t *nodes) {
	expr_node_t *next;

	while (nodes) {
		next = nodes->right;
		expr_free_node(nodes);
		nodes = next;
	}
}