#include <stdlib.h>
#include <stdio.h>
#include "const.h"

const char* ignoreChars          = " \n\r\t";
const char* singularTokens       = "+-*/(){}[]<>:;";

bool              isWordEnd(char);
struct TokenList* createToken(char*);
void              freeTokens(struct TokenList*);
void              printTokens(const struct TokenList*);
struct TokenList* splitTokens(char*);

bool isIgnoreChar(char c){
  int idx;
  if(c == '\0') {
    return false;
  }
  for(idx = 0; ignoreChars[idx] != '\0'; ++idx) {
    if(c == ignoreChars[idx]) {
      return true;
    }
  }
  return false;
}

/*
 * Returns true if the end of a token has been reached.
 *
 * TODO: implement for look-aheads
 */
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
  return isIgnoreChar(c);
}

struct TokenList* createToken(char* str) {
  struct TokenList* result = malloc(sizeof(char*) + sizeof(struct TokenList*));
  result -> token = str;
  result -> type = UNKNOWN; //will be found later
  result -> next = NULL;
  return result;
}

void appendTokenList(struct TokenList* head, struct TokenList* tail) {
  while(head -> next != NULL) {
    head = head -> next;
  }
  head -> next = tail;
}

void freeTokens(struct TokenList* tokens) {
  free(tokens -> token);
  if(tokens -> next) {
    freeTokens(tokens -> next);
  }
  free(tokens);
}

void printTokens(const struct TokenList* tokens) {
  printf("- ");
  printf(tokens -> token);
  printf("\n");
  if(tokens -> next) {
    printTokens(tokens -> next);
  }
}

struct TokenList* splitTokens(char* input) {
  char* str = malloc(TOKEN_BUFFER_SIZE);

  for(; isIgnoreChar(*input); ++input); //advance past whitespace

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

int main(int argc, char** argv) {
  char* buf = malloc(LINE_BUFFER_SIZE);

  if(argc != 2) {
    printf("Usage: obcompile <program file>");
    printf("\n");
    return 1;
  }

  FILE* program = fopen(argv[1], "r");
  if(!program) {
    printf("Error opening specified file.");
    printf("\n");
    return 2;
  }

  struct TokenList* tokens = NULL;

  fgets(buf, LINE_BUFFER_SIZE, program);
  tokens = splitTokens(buf);
  //free(buf);
  while(fgets(buf, LINE_BUFFER_SIZE, program)) {
    appendTokenList(tokens, splitTokens(buf));
  }
  printTokens(tokens);
  freeTokens(tokens);

  free(buf);
  return 0;
}

