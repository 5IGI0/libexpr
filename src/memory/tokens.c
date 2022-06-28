#include <stdlib.h>

#include <libexpr/expr.h>

void expr_free_token(expr_token_t *token) {
	if (token->type == EXPR_TKN_VAR)
		free(token->data.variable);
}

void expr_free_tokens(expr_token_t *tokens, size_t len) {
	size_t i;

	for (i = 0; i < len; i++)
		expr_free_token(tokens + i);
	
	free(tokens);
}