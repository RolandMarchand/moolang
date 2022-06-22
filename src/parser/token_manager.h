#pragma once

#include "scanner/token.h"
#include "scanner/token_array.h"

#define CURRENT_TOKEN &parser_tokens.array[0]
#define CURRENT_TOKEN_IS(...) __TOKEN_IS__(CURRENT_TOKEN,		\
					   (TokenType[]){__VA_ARGS__, -1})
#define CURRENT_LINE *CURRENT_TOKEN.line

extern struct token_array parser_tokens;

struct token advance();
int __TOKEN_IS__(struct token *tok, TokenType type[]);
