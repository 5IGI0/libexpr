#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <libexpr/structs.h>

#include "lexer.h"
#include "../operators.h"

static const char *expr_skipspaces(const char *str) {
	while (isspace(str[0]))
		str++;
	return str;
}

static const char *expr_tokenize_one(const char *str, expr_token_t *token) {
	void *dataptr = (token) ? &token->data : NULL;

	if (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]))) {
		if (token) token->type = EXPR_TKN_LITERAL;
		str = expr_parser_num(str, dataptr);
	} else if (IS_OPR_CHAR(str[0])) {
		if (token) token->type = EXPR_TKN_OPR;
		str = expr_parse_opr(str, dataptr);
	} else if (isalpha(str[0])) {
		if (token) token->type = EXPR_TKN_VAR;
		str = expr_parse_variable(str, dataptr);
	} else if (str[0] == '(' || str[0] == ')') {
		if (token)
			token->type = (str[0] == '(') ? 
				EXPR_TKN_OPEN_BRKTS : EXPR_TKN_CLOSE_BRKTS;
		str++;
	} else
		return NULL;
	return str;
}

int expr_count_token(const char *str, size_t *lenptr) {
	const char *ptr;
	size_t len = 0;

	ptr = expr_skipspaces(str);
	while (ptr[0]) {
		if (!(ptr = expr_tokenize_one(ptr, NULL)))
			return -1;
		len++;
		ptr = expr_skipspaces(ptr);
	}
	*lenptr = len;
	return 0;
}

int	expr_tokenize(const char *str, expr_token_t **tokenptr, size_t *lenptr) {
	expr_token_t *tokens;
	size_t i = 0;
	const char *ptr;

	/* validate / set length */
	if (expr_count_token(str, lenptr) < 0)
		return -1;
	
	if (!*tokenptr) {
		tokens = malloc(sizeof(expr_token_t) * (*lenptr));
		if (!tokens)
			return -1;
		*tokenptr = tokens;
	} else
		tokens = *tokenptr;

	ptr = expr_skipspaces(str);
	while (ptr[0]) {
		if (!(ptr = expr_tokenize_one(ptr, tokens + i)))
			return -1;
		i++;
		ptr = expr_skipspaces(ptr);
	}
	
	return 0;
}