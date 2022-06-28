#ifndef __LIBEXPR_STRUCTS_H__
#define __LIBEXPR_STRUCTS_H__

typedef long expr_int_t;
typedef double expr_flt_t;

typedef enum {
	EXPR_TKN_OPEN_BRKTS,
	EXPR_TKN_CLOSE_BRKTS,
	EXPR_TKN_LITERAL,
	EXPR_TKN_VAR,
	EXPR_TKN_OPR
} expr_token_type_t;

typedef enum {
	EXPR_OPR_ADD = 0,
	EXPR_OPR_SUB = 1,
	EXPR_OPR_MULT = 2,
	EXPR_OPR_DIV = 3,
	EXPR_OPR_MOD = 4,
	EXPR_OPR_EQ = 5,
	EXPR_OPR_NEQ = 6,
	EXPR_OPR_GRTR = 7,
	EXPR_OPR_LESS = 8,
	EXPR_OPR_GRTR_EQ = 9,
	EXPR_OPR_LESS_EQ = 10,
	EXPR_OPR_BIT_LEFT = 11,
	EXPR_OPR_BIT_RIGHT = 12,
	EXPR_OPR_AND = 13,
	EXPR_OPR_OR = 14,
	EXPR_OPR_BIT_OR = 15,
	EXPR_OPR_BIT_AND = 16,
	EXPR_OPR_BIT_XOR = 17
} expr_opr_t;

typedef enum {
	EXPR_LIT_INT,
	EXPR_LIT_FLT
} expr_literal_type_t;

typedef struct {
	expr_literal_type_t type;
	union {
		expr_int_t int_val;
		expr_flt_t flt_val;
	} lit;
} expr_literal_t;

typedef struct {
	expr_token_type_t type;
	union {
		expr_opr_t opr;
		char *variable;
		expr_literal_t literal;
	} data;
} expr_token_t;

typedef enum {
	EXPR_NODE_OPR,
	EXPR_NODE_SUB,
	EXPR_NODE_VAR,
	EXPR_NODE_LIT
} expr_node_type_t;

typedef struct expr_node expr_node_t;

struct expr_node {
	expr_node_type_t type;
	expr_node_t *right;
	union {
		expr_node_t *sub;
		char *var;
		expr_opr_t opr;
		expr_literal_t lit;
	} data;
};

typedef struct {
	char *name;
	expr_literal_t value;
} expr_variable_t;

#endif