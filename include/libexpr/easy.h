#ifndef __LIBEXPR_EASY_H__
#define __LIBEXPR_EASY_H__

#include <stddef.h>

#include "structs.h"

/**
 * @brief compile a string (see expr_free_nodes and expr_evaluate_node)
 * 
 * @param str string to compile
 * @param nodes output node pointer
 * @return int 0 on success, -1 on error
 */
int expr_easy_compile(const char *str, expr_node_t **nodes);

/**
 * @brief evaluate expression from `str`
 * 
 * @param str expression to evaluate
 * @param vars variables
 * @param varlen variable count
 * @param ret expression result
 * @return int 0 on success, -1 on error
 */
int expr_easy_eval(const char *str, expr_variable_t *vars, size_t varlen,
		expr_literal_t *ret);

/**
 * @brief evaluate expression from `str`
 * 
 * @param str expression to evaluate
 * @param vars variables
 * @param varlen variable count
 * @param ret expression result
 * @return int 0 on success, -1 on error
 */
int expr_easy_eval_int(const char *str, expr_variable_t *vars, size_t varlen,
		expr_int_t *ret);

/**
 * @brief evaluate an expression from a node
 * 
 * @param node node to evaluate
 * @param variables variables
 * @param varlen variable count
 * @param ret return value
 * @return int 0 on success, -1 on error
 */
int expr_easy_eval_float(const char *str, expr_variable_t *vars, size_t varlen,
		expr_flt_t *ret);

#endif