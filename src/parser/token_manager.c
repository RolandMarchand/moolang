#include "token_manager.h"

#include <assert.h>

struct token advance()
{
	assert(parser_tokens.count > 0);

	struct token t = *CURRENT_TOKEN;

	if (parser_tokens.count == 1)
		return t;

	++(parser_tokens.array);
	--(parser_tokens.count);
	parser_tokens.size -= sizeof(struct token);

	return t;
}

int __TOKEN_IS__(struct token *tok, TokenType type[])
{
	int NOT_A_TOKEN = -1;
	for (int i = 0; type[i] != NOT_A_TOKEN; i++)
		if (tok->type == type[i]) return 1;
	return 0;
}
