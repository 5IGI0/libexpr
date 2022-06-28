#include <libexpr/expr.h>

#include "compiler.h"
#include "../defines.h"

int	expr_compile_tokens(
	expr_token_t *tokens,
	size_t tokenlen,
	int depth,
	expr_node_t **node
) {
	size_t count = 0;
	int tmpcount;
	int needopr = 0;
	expr_node_t *firstnode = NULL;
	expr_node_t *lastptr = NULL;
	expr_node_t *nodeptr;

	if (depth == MAX_DEPTH)
		return -1;

	while (
		count < tokenlen &&
		tokens[count].type != EXPR_TKN_CLOSE_BRKTS
	) {
		if ((tmpcount = expr_compile_node(tokens+count, tokenlen-count, depth, &nodeptr)) < 0)
			return (expr_free_nodes(firstnode), -1);
		
		if (needopr && nodeptr->type != EXPR_NODE_OPR)
			return (expr_free_nodes(firstnode), -1);
		else if (!needopr && nodeptr->type == EXPR_NODE_OPR)
			return (expr_free_nodes(firstnode), -1);

		if (!firstnode)
			firstnode = nodeptr;
		if (lastptr)
			lastptr->right = nodeptr;
		lastptr = nodeptr;
		count += tmpcount;
		needopr = !needopr;
	}

	if (!firstnode || nodeptr->type == EXPR_NODE_OPR)
		return (expr_free_nodes(firstnode), -1);
	
	if (depth == 0 && count < tokenlen) /* unexcepted ')' */
		return (expr_free_nodes(firstnode), -1);

	node[0] = firstnode;
	
	return count + (count < tokenlen);
}