#ifndef __LIBEXPR_LEXER_H__
#define __LIBEXPR_LEXER_H__

#include <libexpr/structs.h>

int	expr_tokenize(const char *str, expr_token_t **tokens, size_t *lenptr);
const char *expr_parse_float(const char *str, expr_literal_t *literal);
const char *expr_parser_num(const char *str, expr_literal_t *literal);
const char *expr_parse_variable(const char *str, char **varname);

#endif