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

void assignStat(){
  designator();
  accept(ASSIGN_SYM, 0);
  expr();
}

void whileStat(){
  accept(WHILE_SYM, 0);
  expr();
  accept(DO_SYM, 0);
  statSeq();
  accept(END_SYM, 0);
}

void ifStat(){
  accept(IF_SYM, 0);
  expr();
  accept(THEN_SYM, 0);
  statSeq();
  while(token -> type == ELSIF_SYM){
     accept(ELSIF_SYM, 0);
     expr();
     accept(THEN_SYM, 0);
     statSeq();
  }
  if(token -> type == ELSIF_SYM){
     accept(ELSE_SYM, 0);
     statSeq();
  }
  accept(END_SYM, 0);
}

void procCall(){
  accept(IDENT_SYM, 0);
  if(token -> type == LPAREN_SYM, 0)
    actParams();
}

void actParams(){
  accept(LPAREN_SYM, 0);
  if(token -> type == PLUS_SYM || token -> type == MINUS_SYM || 
      token -> type == IDENT_SYM)
    exprList();
  accept(RPAREN_SYM, 0);
}

void exprList(){
  expr();
  while(token -> type -> COMMA_SYM){
    accept(COMMA_SYM, 0);
    expr();
  }
    
}

void repeatStat(){
  accept(REPEAT_SYM, 0);
  statSeq();
  accept(UNTIL_SYM, 0);
  expr();
}

void forStat(){
  accept(FOR_SYM, 0);
  accept(IDENT_SYM, 0);
  accept(ASSIGN_SYM, 0);
  expr();
  accept(TO_SYM, 0);
  expr();
  if(token -> type == BY_SYM){
    accept(TO_SYM, 0);
    constExpr();
  }
  accept(END_SYM, 0);
    
}

void loopStat(){
  accept(LOOP_SYM, 0);
  statSeq();
  accept(END_SYM, 0);
}

void caseStat(){
  
  accept(CASE_SYM, 0);
  expr();
  accept(OF_SYM, 0);
  caseOpt();

  while(token -> type == PIPE_SYM){
    accept(PIPE_SYM, 0);
    caseOpt();
  }

  if(token -> type == ELSE_SYM){
    accept(ELSE_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0); 
}

void caseOpt(){
  if(token -> type == PLUS_SYM || token -> type == MINUS_SYM ||
     token -> type == IDENT_SYM) {

     caseLabs();
     while(token -> type == COMMA_SYM){
       accept(COMMA_SYM, 0);
       caseLabs();
     }
     accept(COLON_SYM, 0);
     statSeq();
  }
}

void caseLabs(){
  constExpr();
  if(token -> type == DOT_SYM){
     accept(DOT_SYM, 0);
     accept(DOT_SYM, 0);
     constExpr();
  }
}

void constExpr(){
  expr(); //Are you fucking kidding me. 
}

void expr(){
  simplExpr();
  if(token -> type == EQUAL_SYM || token -> type == OCTOTHORPE_SYM ||
     token -> type == LT_SYM || token -> type == LTE_SYM ||
     token -> type == RT_SYM || token -> type == RTE_SYM){
    relation();
    simplExpr();
  }  
}

void simplExpr(){
  if(token -> type == PLUS_SYM)
    accept(PLUS_SYM, 0);
  else if(token -> type == MINUS_SYM)
    accept(MINUS_SYM, 0);

  term();

  while(token -> type == PLUS_SYM || token -> type == MINUS_SYM ||
        token -> type == OR_SYM){
     addOp();
     term();
  } 
}

void term(){
  Factor();
  while(token -> type == STAR_SYM || token -> type == MOD_SYM ||
        token -> type == DIV_SYM || token -> type == AMPERSAND_SYM) {
     mulOp();
     factor();
  }
}

void factor(){
 designator();
 switch(token -> type){
  case IDENT_SYM :
     if(token -> TYPE == LPAREN_SYM)
     actParams();
  case INTEGER_SYM :
   accept(INTERGER_SYM, 0);
  case LPAREN_SYM
   accept(LPAREN_SYM, 0);
   expr();
   accept(RPAREN_SYM, 0);
  case TILDE_SYM
   accept(TILDE_SYM, 0);
  default:
   error(token -> type, IDENT_SYM);
}

void addOp(){
 switch(token -> type){
  case OR_SYM :
    accept(OR_SYM, 0);
    break;
  case MINUS_SYM :
    accept(MINUS_SYM, 0);
    break;
  case PLUS_SYM :
    accept(PLUS_SYM, 0);
    break;
  default :
    error(token -> type, IDENT_SYM);
 } 
}

void relation(){
 switch(token -> type){
  case EQUAL_SYM :
    accept(EQUAL_SYM, 0);
    break;
  case OCTOTHORPE_SYM :
    accept(OCTOTHORPE_SYM, 0);
    break;
  case LT_SYM :
    accept(LT_SYM, 0);
    break;
  case RT_SYM :
    accept(RT_SYM, 0);
    break;
  case LTE_SYM :
    accept(LTE_SYM. 0);
    break;
  case RTE_SYM :
    accept(RTE_SYM, 0);
    break;
  default :
    error(token -> type, IDENT_SYM);
 }
}

void mulOp(){ //GOD DAMN I SHOULD HAVE MADE THIS A SWITCH
  if(token -> type == STAR_SYM)
    accept(STAR_SYM, 0);
  else if(token -> type == DIV_SYM)
    accept(MUL_SYM, 0);
  else if(token -> type == MOD_SYM)
    accept(MOD_SYM, 0);
  else if(token -> type == AMPERSAND_SYM)
    accept(AMPERSAND_SYM);
  else
    error(token -> type, IDENT_SYM);
}

void designator(){
  accept(IDENT_SYM, 0);
  while(token -> type == DOT_SYM)
    selector();
}

void selector(){
  accept(DOT_SYM, 0);
  if(token -> type == IDENT_SYM)
    accept(IDENT_SYM, 0);
  } else if(token -> type == LBRAKT_SYM){
    accept(LBRAKT_SYM, 0);
    exprList();
    accept(RBRAKT_SYM, 0);
  } else
   error(token -> type, IDENT_SYM);   
}


