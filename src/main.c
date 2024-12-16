#include "lexer.h"
#include <stdio.h>

int main(void) {
  Tokens tokens;
	init_tokens(&tokens,TOKENS_DEFAULT_SIZE);
  tokenize(&tokens, "<hello name=\"value\"> blab la bla </hello>");
  for (int i = 0; i < tokens.size; i++) {
		printf("%i - %s\n",tokens.list[i].type,tokens.list[i].content);
  }
  return 0;
}
