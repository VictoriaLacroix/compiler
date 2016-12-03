#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"

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

  printf("\n--- Starting scan. ---\n\n");

  fgets(buf, LINE_BUFFER_SIZE, program);
  tokens = splitTokens(buf);
  //free(buf);
  while(fgets(buf, LINE_BUFFER_SIZE, program)) {
    appendTokenList(tokens, splitTokens(buf));
  }
  setPrevs(tokens);

  recognize(tokens);

  printf("-- SCANNED TOKENS (INCL. COMMENTS)--\n");
  printTokens(tokens);

  while(tokens -> type == CMT_STR_SYM) {
    tokens = findCommentEnd(tokens);
    tokens = tokens -> next;
  }

  if(!removeComments(tokens)) {
    printf("Unclosed comment found.\n");
    return 3;
  }

  printf("-- SCANNED TOKENS --\n");
  printTokens(tokens);

  printf("\n--- Starting parse. ---\n\n");

  parse(tokens);

  freeTokens(tokens);

  free(buf);
  return 0;
}

