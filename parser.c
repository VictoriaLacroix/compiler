#include "parser.h"

// Global tokenList for parsing. Not ideal, but good enough.
struct TokenList* token;

void init(struct TokenList* input) {
  if() {
    token = input;
  } else {
    printf("Parse Error: Tokens list is null.");
  }
}

bool accept(TokenType expected, int errno) {
  if(token -> type == expected) {
    writeSym();
    nextSym();
    return true;
  } else {
    error();
    return false
  }
}

void error(TokenType expected, int errno) {
  printf("");
}
