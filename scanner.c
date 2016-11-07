#include <stdlib.h>
#include <stdio.h>
#include "const.h"

/*
 * scanner.c
 *
 * by  Victoria Lacroix
 * and Alex Arbour
 */

// Pre-defining all functions
// tokenizing functions
bool              isIgnoreChar(const char);
bool              isDoubleToken(const char, const char);
bool              isWordEnd(const char);
struct TokenList* createToken(char*);
struct TokenList* splitTokens(const char*);

// recognizing functions
TokenType         verifyToken(const char*);
TokenType         scanIdent(const char*);
TokenType         scanDigit(const char*);
bool              isLetter(const char c);
bool              isDigit(const char c);
bool              isHex(const char c);
int               strcmp(const char*, const char*);
void              recognize(struct TokenList*);

// TokenList manipulation
void              appentTokenList(struct TokenList*, struct TokenList*);
bool              removeComments(struct TokenList*);
struct TokenList* findCommentEnd(struct TokenList*);
void              printTokens(const struct TokenList*);
void              freeTokens(struct TokenList*);

// ----- TOKENIZING CODE ------

bool isIgnoreChar(const char c){
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

// Verifies if input is at a double token, i.e (*, *), <=, >=
bool isDoubleToken(const char c, const char l) {
  int idx;
  if(c == '\0') {
    return false;
  }
  if(l == '\0') {
    return false;
  }
  for(idx = 0; doubleTokens[idx] != '\0'; idx += 2) {
    if(c == doubleTokens[idx] && l == doubleTokens[idx + 1]) {
      return true;
    }
  }
  return false;
}

/*
 * Returns true if the end of a token has been reached.
 */
bool isWordEnd(const char c) {
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
//sizeof(char*) + 
struct TokenList* createToken(char* str) {
  struct TokenList* result = malloc(sizeof(struct TokenList));
  result -> token = str;
  result -> type = UNKNOWN_SYM; //will be found later
  result -> prev = NULL;
  result -> next = NULL;
  return result;
}

void appendTokenList(struct TokenList* head, struct TokenList* tail) {
  while(head -> next != NULL) {
    head = head -> next;
  }
  head -> next = tail;
}

void setPrevs(struct TokenList* tokens) {
  while(tokens -> next != NULL) {
    tokens -> next -> prev = tokens;
    tokens = tokens -> next;
  }
}

void freeTokens(struct TokenList* tokens) {
  free(tokens -> token);
  if(tokens -> next != NULL) {
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
  if(tokens -> next != NULL) {
    printTokens(tokens -> next);
  }
}

struct TokenList* splitTokens(const char* input) {
  char* str = malloc(TOKEN_BUFFER_SIZE);

  for(; isIgnoreChar(*input); ++input); //advance past whitespace

  if(*input == '\0') {
    return NULL;
  } else if(isDoubleToken(*input, *(input + 1))) {
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

bool removeComments(struct TokenList* tokens) {
  do {
    if(tokens -> type == CMT_STR_SYM) {
      struct TokenList* end = findCommentEnd(tokens);
      if(end == NULL) {
        return false;
      } else {
        // unlink everything between the comment syms
        tokens -> prev -> next = end -> next;
        end -> next -> prev = tokens -> prev;
        tokens -> prev == NULL;
        end -> next == NULL;
        //free them to prevent memory leaks
        //TODO fix this
        //freeTokens(tokens);
      }
    }
    tokens = tokens -> next;
  } while(tokens);
  return true;
}

// Returns the correct end-of-comment token, or null if it does not exist
struct TokenList* findCommentEnd(struct TokenList* tokens) {
  int depth = 0;
  do {
    if(tokens -> type == CMT_STR_SYM) {
      depth++;
    } else if(tokens -> type == CMT_END_SYM) {
      depth--;
    }
    if(depth > 0) {
      tokens = tokens -> next;
    }
  } while(depth > 0 && tokens != NULL);
  return tokens;
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

// copied from the standard library
int strcmp(const char *s1, const char *s2) {
  for (; *s1 == *s2; s1++, s2++) {
    if (*s1 == '\0') {
      return 0;
    }
  }
  return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1);
}

//Sets the TokenType field of every token in the given list
void recognize(struct TokenList* tokens) {
  do {
    tokens -> type = verifyToken(tokens -> token);
    tokens = tokens -> next;
  } while(tokens != NULL);
}

// ----- MAIN FUNCTION -----

#ifndef __MAIN_FUNCTION_DEFINED__
#define __MAIN_FUNCTION_DEFINED__

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
  setPrevs(tokens);

  recognize(tokens);

  if(!removeComments(tokens)) {
    printf("Unclosed comment found.\n");
    return 3;
  }

  printTokens(tokens);
  freeTokens(tokens);

  free(buf);
  return 0;
}

#endif

