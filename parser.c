#include "parser.h"

// Global tokenList for parsing. Not ideal, but good enough.
struct TokenList* token;

bool init(struct TokenList* input) {
  if(input) {
    token = input;
    return true;
  } else {
    printf("Parse Error: Tokens list is null.");
    return false;
  }
}

bool accept(TokenType expected, int errno) {
  if(token -> type == expected) {
    writeSym();
    nextSym();
    return true;
  } else {
    error(token -> type, expected);
    return false
  }
}

void nextSym() {
  token = token -> next;
}

void writeSym() {
  printf(symNames[token -> type]);
  switch(token -> type) {
    case IDENT_SYM:
    case NUMBER_SYM:
    case HEX_SYM:
      printf(" : ");
      printf(token -> token);
    default:
      printf("\n");
      break;
  }
}

void error(TokenType found, TokenType expected) {
  printf("Error # %d: ", expected);
  printf(symNames[found]);
  printf(" found but ");
  printf(symNames[expected]);
  printf(" was expected.");
  printf("\n");
}
