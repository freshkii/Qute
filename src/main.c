#include <stdio.h>

#include "lexer.h"

int main(void) {
  Tokens tokens;
  tokenize(&tokens, "<hello> blab la bla </hello>");
  for (int i = 0; i < tokens.size; i++) {
    printf("%s -- ", tokens.list[i].content);
  }
  return 0;
}
