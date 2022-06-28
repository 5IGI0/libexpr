#ifndef __LIBEXPR_EXPR_H__
#define __LIBEXPR_EXPR_H__

#include <stddef.h>

#include "structs.h"

/**
 * @brief tokenize a string
 * 
 * @param str string to tokenize (if you just want to count tokens, see expr_count_token)
 * @param tokenptr pointer used for returning tokens (if NULL, it will be allocated)
 * @param lenptr token count (might be not modified on error)
 * @return int 0 on succes, -1 on error
 */
int	expr_tokenize(const char *str, expr_token_t **tokenptr, size_t *lenptr);

/**
 * @brief count token
 * 
 * @param str str to count tokens
 * @param lenptr token count (not modified on error)
 * @return int 0 on succes, -1 on error
 */
int expr_count_token(const char *str, size_t *lenptr);

/**
 * @brief free a token list
 * 
 * @param tokens token list
 * @param len token list length
 */
void expr_free_tokens(expr_token_t *tokens, size_t len);

/**
 * @brief compile from tokens
 * 
 * @param tokens tokens
 * @param tokenlen token count
 * @param depth depth of expression (used to know if we should expect a bracket)
 * @param node first node found
 * @return int number of token used, or -1 on error
 */
int	expr_compile_tokens(expr_token_t *tokens, size_t tokenlen, int depth,
	expr_node_t **node);

/**
 * @brief free nodes
 * 
 * @param nodes nodes
 */
void expr_free_nodes(expr_node_t *nodes);

/**
 * @brief evaluate an expression from a node
 * 
 * @param node node to evaluate
 * @param variables variables
 * @param varlen variable count
 * @param ret return value
 * @return int 0 on success, -1 on error
 */
int expr_evaluate_node(expr_node_t *node, expr_variable_t *variables,
		size_t varlen, expr_literal_t *ret);

#define EXPR_VAR_INT(__name, __x) { \
		.name = __name, \
		(expr_literal_t){ \
			.type = EXPR_LIT_INT, \
			.lit.int_val = __x \
		} \
	}

#define EXPR_VAR_FLT(__name, __x) { \
		.name = __name, \
		(expr_literal_t){ \
			.type = EXPR_LIT_FLT, \
			.lit.flt_val = __x \
		} \
	}

#endif