#include <stdlib.h>
#include <stdio.h>
#include "const.h"

/*
 * scanner.c
 *
 * by  Victoria Lacroix
 * and Alex Arbour
 */

bool              isWordEnd(char);
struct TokenList* createToken(char*);
void              freeTokens(struct TokenList*);
void              printTokens(const struct TokenList*);
struct TokenList* splitTokens(char*);
TokenType         verifyToken(const char*);
TokenType         scanIdent(const char*);
TokenType         scanDigit(const char*);
bool              isLetter(const char c);
bool              isDigit(const char c);
bool              isHex(const char c);
int               strcmp(const char*, const char*);
void              recognize(struct TokenList*);

// ----- TOKENIZING CODE ------

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

bool isPartialToken(char c, char n) {
  int idx;
  if(c == '\0') {
    return false;
  }
  for(idx = 0; partialTokens[idx] != '\0'; ++idx) {
    if(c == partialTokens[idx]) {
      switch(idx) {
        case 0: return (n == '*') ? true: false; //comment start
        case 1: return (n == ')') ? true: false; //comment end
        case 2: return (n == '=') ? true: false; //less than or equal
        case 3: return (n == '=') ? true: false; //greater than or equal
        default: return false;
      }
    }
  }
  return false;
}

/*
 * Returns true if the end of a token has been reached.
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
  result -> type = UNKNOWN_SYM; //will be found later
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
  printf(" : ");
  printf(symNames[tokens -> type]);
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
  } else if(isPartialToken(*input, *(input + 1))) {
    //token is one of "(*", "*)", "<=", ">="
    str[0] = *input++;
    str[1] = *input++;
    str[2] = '\0';
  } else if(isWordEnd(*input)) {
    // assemble a single-char str
    str[0] = *input++;
    str[1] = '\0';
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

// ----- ANALYSIS/RECOGNIZING CODE -----

TokenType verifyToken(const char* token) {
  for(int i = 0; i < NUM_RESERVED_WORDS; ++i) {
    //check to see if token matches reserved word
    if(strcmp(reservedWords[i], token) == 0) {
      return reservedWordsSymbols[i];
    }
  }
  if(isLetter(*token)) {
    return scanIdent(token);
  }
  if(isDigit(*token)) {
    return scanDigit(token);
  }
  return UNKNOWN_SYM;
}

TokenType scanIdent(const char* token) {

  int idx = 1; //we already checked first char

  while(*(token+idx) != '\0') {
    if(isLetter(*(token+idx)) || isDigit(*(token+idx))) {
      ++idx;
    } else {
      return UNKNOWN_SYM;
    }
  }
  return IDENT_SYM;
}

TokenType scanDigit(const char* token) {
  int idx = 1; //we already checked first char
  bool hex = false;

  while(*(token+idx) != '\0') {
    if(isHex(*(token+idx)) || isDigit(*(token+idx))) {
      if(isHex(*(token+idx))) {
        hex = true;
      }
      ++idx;
    } else {
      return UNKNOWN_SYM;
    }
  }
  if(hex) {
    return HEX_SYM;
  }
  return NUMBER_SYM;
}


bool isLetter(const char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(const char c) {
  return c >= '0' && c <= '9';
}

bool isHex(const char c) {
  return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int strcmp(const char *s1, const char *s2) {
  for (; *s1 == *s2; s1++, s2++) {
    if (*s1 == '\0') {
      return 0;
    }
  }
  return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1);
}

void recognize(struct TokenList* tokens) {
  do {
    tokens -> type = verifyToken(tokens -> token);
    tokens = tokens -> next;
  } while(tokens != NULL);
}

// ----- MAIN METHOD -----

int main(int argc, char** argv) {
  char* buf = malloc(LINE_BUFFER_SIZE);

  if(argc != 2) {
    printf("Usage: a.out <file>");
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

  recognize(tokens);

  printTokens(tokens);
  freeTokens(tokens);

  free(buf);
  return 0;
}

