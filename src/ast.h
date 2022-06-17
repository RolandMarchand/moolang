#include "scanner/scanner.h"

typedef struct AstNode {
	TokenType type;
	union {
		double value;
		char *string;
		struct {
			struct AstNode *left;
			struct AstNode *right;
		} op;
	};
} AstNode;
