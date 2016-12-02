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

void module() {
  accept(MODULE_SYM, 0);
  accept(IDENT_SYM, 0);
  accept(SEMICOLON_SYM, 0);
  block();
  accept(DOT_SYM, 0);
}

void block() {
  declSeq();
  if(token -> type == BEGIN_SYM) {
    accept(BEGIN_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0);
  accept(IDENT_SYM, 0);
}

void declSeq() {
  if(token -> type == CONST_SYM) {
    accept(CONST_SYM, 0);
    constDecls();
  }
  if(token -> type == TYPE_SYM) {
    accept(TYPE_SYM, 0);
    typeDecls();
  }
  if(token -> type == VAR_SYM) {
    accept(VAR_SYM, 0);
    varDecls();
  }
  while(token -> type == PROCEDURE_SYM) {
    procDecl();
  }
}
void constDecls() {
  constDecl();
  while(token -> type == IDENT_SYM) {
    constDecl();
  }
}

void constDecl() {
  accept(IDENT_SYM, 0);
  accept(EQUALS_SYM, 0);
  constExpr();
  accept(SEMICOLON_SYM, 0);
}

void typeDecls() {
  typeDecl();
  while(token -> type == IDENT_SYM) {
    typeDecl();
  }
}

void typeDecl() {
  accept(IDENT_SYM, 0);
  accept(EQUALS_SYM, 0);
  type();
  accept(SEMICOLON_SYM, 0);
}

void varDecls() {
  varDecl();
  while(token -> type == IDENT_SYM) {
    varDecl();
  }
}

void varDecl() {
  identList();
  accept(COLON_SYM, 0);
  type();
  accept(SEMICOLON_SYM, 0);
}

void identList() {
  accept(IDENT_SYM, 0);
  while(token -> type == COMMA_SYM) {
    accept(COMMA_SYM, 0);
    accept(IDENT_SYM, 0);
  }
}

void procDecl() {
  accept(PROCEDURE_SYM, 0);
  accept(IDENT_SYM, 0);
  if(token -> type == LPAREN_SYM) {
    formalPars();
  }
  if(token -> type == COLON_SYM) {
    accept(COLON_SYM, 0);
    accept(IDENT_SYM, 0);
  }
  accept(SEMICOLON_SYM, 0);
  procBody();
}

void formalPars() {
  accept(LPAREN_SYM, 0);
  if(token -> type == VAR_SYM) {
    fpSection();
    while(token -> type == SEMICOLON_SYM) {
      accept(SEMICOLON_SYM, 0);
      fpSection();
    }
  }
  accept(RPAREN_SYM);
}

void fpSection() {
  if(token -> type == VAR_SYM) {
    accept(VAR_SYM, 0);
  }
  identList();
  accept(COLON_SYM, 0);
  formalType();
}

void formalType() {
  accept(IDENT_SYM, 0);
}

void procBody() {
  block();
}

void type() {
  switch(token -> type) {
    case IDENT_SYM:
      accept(IDENT_SYM, 0);
      break;
    case ARRAY_SYM:
      arrayType();
      break;
    case RECORD_SYM:
      recordType();
      break;
    case LPAREN_SYM:
      enumType();
      break;
    default:
      error(token -> type, IDENT_SYM);
  }
}

void arrayType() {
  accept(ARRAY_SYM, 0);
  length();
  while(token -> type == COMMA_SYM) {
    length();
  }
  accept(OF_SYM, 0);
  type();
}

void length() {
  constExpr();
}

void recordType() {
  accept(RECORD_SYM, 0);
  fieldList();
  while(token -> type == SEMICOLON_SYM) {
    accept(SEMICOLON, 0);
    fieldList();
  }
}

void enumType() {
  accept(LPAREN_SYM, 0);
  accept(IDENT_SYM, 0);
  while(token -> type == COMMA_SYM) {
    accept(COMMA_SYM, 0);
    accept(IDENT_SYM, 0);
  }
  accept(RPAREN_SYM, 0);
}

void fieldList() {
  if(token -> type == IDENT_SYM) {
    identList();
    accept(COLON_SYM, 0);
    type();
  }
}

void statSeq() {
  stat();
  while(token -> type == SEMICOLON_SYM) {
    accept(SEMICOLON_SYM, 0);
    stat();
  }
}

void stat() {
  switch(token -> type) {
    case IDENT_SYM:
      if(token -> next -> type == DOT_SYM || token -> next -> type == ASSIGN_SYM) {
        assignStat();
      } else {
        procCall();
      }
      break;
    case IF_SYM:
      ifStat();
      break;
    case WHILE_SYM:
      whileStat();
      break;
    case CASE_SYM:
      caseStat();
      break;
    case REPEAT_SYM:
      repeatStat();
      break;
    case FOR_SYM:
      forStat();
      break;
    case LOOP_SYM:
      loopStat();
      break;
    case EXIT_SYM:
      accept(EXIT_SYM, 0);
      break;
    case RETURN_SYM:
      accept(RETURN_SYM, 0);
      expr();
      break;
    default:
      error(token -> type, IDENT_SYM);
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
