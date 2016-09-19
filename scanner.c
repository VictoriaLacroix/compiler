#include <stdlib.h>
#include <stdio.h>
#include "const.h"

const char*  singularTokens = " +-*/(){}[]<>:;\n\r\t";
const char** reservedTokens;

struct TokenList {
  char*              token;
  struct TokenList*  next;
};

bool              isWordEnd(char);
struct TokenList* createToken(char*);
struct TokenList* splitTokens(char*);
void              printTokens(const struct TokenList*);
void              freeTokens(struct TokenList*);

struct TokenList* createToken(char* str) {
  struct TokenList* result = malloc(sizeof(char*) + sizeof(struct TokenList*));
  result -> token = str;
  result -> next = NULL;
  return result;
}

void printTokens(const struct TokenList* tokens) {
  printf(tokens -> token);
  printf("\n");
  if(tokens -> next) {
    printTokens(tokens -> next);
  }
}

void freeTokens(struct TokenList* tokens) {
  free(tokens -> token);
  if(tokens -> next) {
    freeTokens(tokens -> next);
  }
  free(tokens);
}

bool isWordEnd(char c) {
  int idx;
  if(c == '\0') {
    return true;
  }
  for(idx = 0; singularTokens[idx] != '\0'; ++idx) {
    if(c == singularTokens[idx]) {
      return true;
    }
  }
  return false;
}

/*
 * Returns the current token based on the given input, advancing that input
 * towards the next token.
 */
struct TokenList* splitTokens(char* input) {
  char* str = malloc(TOKEN_LENGTH);

  for(; *input == ' '; ++input); //advance past whitespace

  if(*input == '\0') {
    return NULL;
  } else if(isWordEnd(*input)) {
    // assemble a single-char str
    str[0] = *input;
    str[1] = '\0';
    ++input;
  } else {
    // assemble current str
    int idx;
    for(idx = 0; !isWordEnd(*(input + idx)); ++idx) {
      str[idx] = *(input + idx);
    }
    str[idx] = '\0';
    input += idx;
  }
  
  struct TokenList* result = createToken(str);
  result -> next = splitTokens(input);
  return result;
}

/**
 * Tester for the tokenizer.
 */
int main() {
  char* input = "test TEST teeest TEEEST fwoop";

  printf("Attempting split.\n");
  struct TokenList* tokens = splitTokens(input);
  printf("Attempting print.\n");
  printTokens(tokens);
  printf("\n");
  printf("Attempting freeing.\n");
  freeTokens(tokens);

  return 0;
}

