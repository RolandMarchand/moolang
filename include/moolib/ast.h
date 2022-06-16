#include "moolib/scanner/scanner.h"

typedef struct AstNode {
	TokenType type;
	union { double value;
		struct {
			struct AstNode* left;
			struct AstNode* right;
		} op;
	};
} AstNode;
