#ifndef __LIBEXRP_EXPR_H__
#define __LIBEXRP_EXPR_H__

#include <libexpr/structs.h>

int	expr_compile_tokens(expr_token_t *tokens, size_t tokenlen, int depth,
	expr_node_t **node);
int expr_compile_node(expr_token_t *tokens, size_t tokenlen, int depth,
	expr_node_t **nodeptr);

#endif