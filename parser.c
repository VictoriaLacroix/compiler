#include "parser.h"

struct TokenList* sym = NULL;
int    indent = 0;

void parse(struct TokenList* tokens) {
  init(tokens);
  module();
}

bool init(struct TokenList* tokens) {
  if(tokens) {
    sym = tokens;
    return true;
  } else {
    printf("Parse Error: Tokens list is null.");
    return false;
  }
}

bool accept(TokenType expected, int errno) {
  if(sym -> type == expected) {
    //printf("Successfully accepted: ");
    writeSym();
    nextSym();
    return true;
  } else {
    error(sym -> type, expected);
    return false;
  }
}

void nextSym() {
  sym = sym -> next;
}

void writeIndent() {
  for(int i = 0; i < indent; ++i) {
    printf("  ");
  }
}

void writeSym() {
  writeIndent();
  printf(symNames[sym -> type]);
  switch(sym -> type) {
    case IDENT_SYM:
    case NUMBER_SYM:
    case HEX_SYM:
      printf(" : ");
      printf(sym -> token);
    default:
      break;
  }
  printf("\n");
}

void writeRule(const char* rule) {
  writeIndent();
  printf(rule);
  printf(" :");
  printf("\n");
}

void error(TokenType found, TokenType expected) {
  writeIndent();
  printf("Error # %d: ", expected);
  printf(symNames[found]);
  printf(" found but ");
  printf(symNames[expected]);
  printf(" was expected.");
  printf("\n");
}

void module() {
  writeRule("Module");
  ++indent;
  accept(MODULE_SYM, 0);
  accept(IDENT_SYM, 0);
  accept(SEMICOLON_SYM, 0);
  block();
  accept(DOT_SYM, 0);
  --indent;
}

void block() {
  writeRule("Block");
  ++indent;
  declSeq();
  if(sym -> type == BEGIN_SYM) {
    accept(BEGIN_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0);
  accept(IDENT_SYM, 0);
  accept(SEMICOLON_SYM, 0);
  --indent;
}

void declSeq() {
  writeRule("DeclSeq");
  ++indent;
  if(sym -> type == CONST_SYM) {
    accept(CONST_SYM, 0);
    constDecls();
  }
  if(sym -> type == TYPE_SYM) {
    accept(TYPE_SYM, 0);
    typeDecls();
  }
  if(sym -> type == VAR_SYM) {
    accept(VAR_SYM, 0);
    varDecls();
  }
  while(sym -> type == PROCEDURE_SYM) {
    procDecl();
  }
  --indent;
}
void constDecls() {
  writeRule("ConstDecls");
  ++indent;
  constDecl();
  while(sym -> type == IDENT_SYM) {
    constDecl();
  }
  --indent;
}

void constDecl() {
  writeRule("ConstDecl");
  ++indent;
  accept(IDENT_SYM, 0);
  accept(EQUALS_SYM, 0);
  constExpr();
  accept(SEMICOLON_SYM, 0);
  --indent;
}

void typeDecls() {
  writeRule("TypeDecls");
  ++indent;
  typeDecl();
  while(sym -> type == IDENT_SYM) {
    typeDecl();
  }
  --indent;
}

void typeDecl() {
  writeRule("TypeDecl");
  ++indent;
  accept(IDENT_SYM, 0);
  accept(EQUALS_SYM, 0);
  type();
  accept(SEMICOLON_SYM, 0);
  --indent;
}

void varDecls() {
  writeRule("VarDecls");
  ++indent;
  varDecl();
  while(sym -> type == IDENT_SYM) {
    varDecl();
  }
  --indent;
}

void varDecl() {
  writeRule("VarDecl");
  ++indent;
  identList();
  accept(COLON_SYM, 0);
  type();
  accept(SEMICOLON_SYM, 0);
  --indent;
}

void identList() {
  writeRule("IdentList");
  ++indent;
  accept(IDENT_SYM, 0);
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM, 0);
    accept(IDENT_SYM, 0);
  }
  --indent;
}

void procDecl() {
  writeRule("ProcDecl");
  ++indent;
  accept(PROCEDURE_SYM, 0);
  accept(IDENT_SYM, 0);
  if(sym -> type == LPAREN_SYM) {
    formalPars();
  }
  if(sym -> type == COLON_SYM) {
    accept(COLON_SYM, 0);
    accept(IDENT_SYM, 0);
  }
  accept(SEMICOLON_SYM, 0);
  procBody();
  --indent;
}

void formalPars() {
  writeRule("FormalPars");
  ++indent;
  accept(LPAREN_SYM, 0);
  if(sym -> type == VAR_SYM || sym -> type == IDENT_SYM) {
    fpSection();
    while(sym -> type == SEMICOLON_SYM) {
      accept(SEMICOLON_SYM, 0);
      fpSection();
    }
  }
  accept(RPAREN_SYM, 0);
  --indent;
}

void fpSection() {
  writeRule("FPSection");
  ++indent;
  if(sym -> type == VAR_SYM) {
    accept(VAR_SYM, 0);
  }
  identList();
  accept(COLON_SYM, 0);
  formalType();
  --indent;
}

void formalType() {
  writeRule("FormalType");
  ++indent;
  accept(IDENT_SYM, 0);
  --indent;
}

void procBody() {
  writeRule("ProcBody");
  ++indent;
  block();
  --indent;
}

void type() {
  writeRule("Type");
  ++indent;
  switch(sym -> type) {
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
      error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void arrayType() {
  writeRule("ArrayType");
  ++indent;
  accept(ARRAY_SYM, 0);
  length();
  while(sym -> type == COMMA_SYM) {
    length();
  }
  accept(OF_SYM, 0);
  type();
  --indent;
}

void length() {
  writeRule("Length");
  ++indent;
  constExpr();
  --indent;
}

void recordType() {
  writeRule("RecordType");
  ++indent;
  accept(RECORD_SYM, 0);
  fieldList();
  while(sym -> type == SEMICOLON_SYM) {
    accept(SEMICOLON_SYM, 0);
    fieldList();
  }
  --indent;
}

void enumType() {
  writeRule("EnumType");
  ++indent;
  accept(LPAREN_SYM, 0);
  accept(IDENT_SYM, 0);
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM, 0);
    accept(IDENT_SYM, 0);
  }
  accept(RPAREN_SYM, 0);
  --indent;
}

void fieldList() {
  writeRule("FieldList");
  ++indent;
  if(sym -> type == IDENT_SYM) {
    identList();
    accept(COLON_SYM, 0);
    type();
  }
  --indent;
}

void statSeq() {
  writeRule("StatSeq");
  ++indent;
  stat();
  while(sym -> type == SEMICOLON_SYM) {
    accept(SEMICOLON_SYM, 0);
    stat();
  }
  --indent;
}

void stat() {
  writeRule("Stat");
  ++indent;
  switch(sym -> type) {
    case IDENT_SYM:
      assignOrProc();
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
      error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void assignOrProc() {
  writeRule("AssignOrProc");
  ++indent;
  designator();
  if(sym -> type == ASSIGN_SYM) {
    assignStat();
  } else {
    procCall();
  }
  --indent;
}

void assignStat() {
  writeRule("AssignStat");
  ++indent;
  accept(ASSIGN_SYM, 0);
  expr();
  --indent;
}

void procCall() {
  writeRule("ProcCall");
  ++indent;
  if(sym -> type == LPAREN_SYM) {
    actParams();
  }
  --indent;
}

void whileStat() {
  writeRule("WhileStat");
  ++indent;
  accept(WHILE_SYM, 0);
  expr();
  accept(DO_SYM, 0);
  statSeq();
  while(sym -> type == ELSIF_SYM) {
    accept(ELSIF_SYM, 0);
    expr();
    accept(DO_SYM, 0);
    statSeq();
  }
  if(sym -> type == ELSE_SYM){
    accept(ELSE_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0);
  --indent;
}

void ifStat() {
  writeRule("IfStat");
  ++indent;
  accept(IF_SYM, 0);
  expr();
  accept(THEN_SYM, 0);
  statSeq();
  while(sym -> type == ELSIF_SYM) {
    accept(ELSIF_SYM, 0);
    expr();
    accept(THEN_SYM, 0);
    statSeq();
  }
  if(sym -> type == ELSE_SYM){
    accept(ELSE_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0);
  --indent;
}

void actParams() {
  writeRule("ActParams");
  ++indent;
  accept(LPAREN_SYM, 0);
  if(sym -> type == PLUS_SYM || sym -> type == MINUS_SYM
      || sym -> type == IDENT_SYM) {
    exprList();
  }
  accept(RPAREN_SYM, 0);
  --indent;
}

void exprList() {
  writeRule("ExprList");
  ++indent;
  expr();
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM, 0);
    expr();
  }
  --indent;
}

void repeatStat() {
  writeRule("RepeatStat");
  ++indent;
  accept(REPEAT_SYM, 0);
  statSeq();
  accept(UNTIL_SYM, 0);
  expr();
  --indent;
}

void forStat() {
  writeRule("ForStat");
  ++indent;
  accept(FOR_SYM, 0);
  accept(IDENT_SYM, 0);
  accept(ASSIGN_SYM, 0);
  expr();
  accept(TO_SYM, 0);
  expr();
  if(sym -> type == BY_SYM) {
    accept(BY_SYM, 0);
    constExpr();
  }
  accept(DO_SYM, 0);
  statSeq();
  accept(END_SYM, 0);
  --indent;
}

void loopStat() {
  writeRule("LoopStat");
  ++indent;
  accept(LOOP_SYM, 0);
  statSeq();
  accept(END_SYM, 0);
  --indent;
}

void caseStat() {
  writeRule("CaseStat");
  ++indent;
  accept(CASE_SYM, 0);
  expr();
  accept(OF_SYM, 0);
  caseOpt();
  while(sym -> type == PIPE_SYM) {
    accept(PIPE_SYM, 0);
    caseOpt();
  }
  if(sym -> type == ELSE_SYM) {
    accept(ELSE_SYM, 0);
    statSeq();
  }
  accept(END_SYM, 0);
  --indent;
}

void caseOpt() {
  writeRule("Case");
  ++indent;
  if(sym -> type == PLUS_SYM || sym -> type == MINUS_SYM
      || sym -> type == IDENT_SYM) {
    caseLabs();
    while(sym -> type == COMMA_SYM) {
      accept(COMMA_SYM, 0);
      caseLabs();
    }
    accept(COLON_SYM, 0);
    statSeq();
  }
  --indent;
}

void caseLabs() {
  writeRule("CaseLabs");
  ++indent;
  constExpr();
  if(sym -> type == DOT_SYM) {
    accept(DOT_DOT_SYM, 0);
    constExpr();
  }
  --indent;
}

void constExpr() {
  writeRule("ConstExpr");
  ++indent;
  expr(); // Wow, so complicated.
  --indent;
}

void expr() {
  writeRule("Expr");
  ++indent;
  simplExpr();
  if(sym -> type == EQUALS_SYM || sym -> type == OCTOTHORPE_SYM
      || sym -> type == LT_SYM || sym -> type == LTE_SYM
      || sym -> type == GT_SYM || sym -> type == GTE_SYM) {
    relation();
    simplExpr();
  }
  --indent;
}

void simplExpr() {
  writeRule("SimplExpr");
  ++indent;
  if(sym -> type == PLUS_SYM) {
    accept(PLUS_SYM, 0);
  } else if(sym -> type == MINUS_SYM) {
    accept(MINUS_SYM, 0);
  }
  term();
  while(sym -> type == PLUS_SYM || sym -> type == MINUS_SYM
      || sym -> type == OR_SYM) {
    addOp();
    term();
  }
  --indent;
}

void term() {
  writeRule("Term");
  ++indent;
  factor();
  while(sym -> type == STAR_SYM || sym -> type == MOD_SYM
      || sym -> type == DIV_SYM || sym -> type == AMPERSAND_SYM) {
    mulOp();
    factor();
  }
  --indent;
}

void factor() {
  writeRule("Factor");
  ++indent;
  designator();
  switch(sym -> type) {
    case IDENT_SYM:
      if(sym -> type == LPAREN_SYM) {
        actParams();
      }
      break;
    case NUMBER_SYM:
      accept(NUMBER_SYM, 0);
      break;
    case HEX_SYM:
      accept(HEX_SYM, 0);
      break;
    case LPAREN_SYM:
      accept(LPAREN_SYM, 0);
      expr();
      accept(RPAREN_SYM, 0);
      break;
    case TILDE_SYM:
      accept(TILDE_SYM, 0);
      break;
    default:
      break;
  }
  --indent;
}

void addOp() {
  writeRule("AddOp");
  ++indent;
  switch(sym -> type) {
    case OR_SYM :
      accept(OR_SYM, 0);
      break;
    case MINUS_SYM :
      accept(MINUS_SYM, 0);
      break;
    case PLUS_SYM :
      accept(PLUS_SYM, 0);
      break;
    default:
      error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void relation() {
  writeRule("Relation");
  ++indent;
  switch(sym -> type) {
    case EQUALS_SYM :
      accept(EQUALS_SYM, 0);
      break;
    case OCTOTHORPE_SYM :
      accept(OCTOTHORPE_SYM, 0);
      break;
    case LT_SYM :
      accept(LT_SYM, 0);
      break;
    case GT_SYM :
      accept(GT_SYM, 0);
      break;
    case LTE_SYM :
      accept(LTE_SYM, 0);
      break;
    case GTE_SYM :
      accept(GTE_SYM, 0);
      break;
    default :
      error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void mulOp() {
  writeRule("MulOp");
  ++indent;
  //TODO make a switch
  if(sym -> type == STAR_SYM) {
    accept(STAR_SYM, 0);
  } else if(sym -> type == DIV_SYM) {
    accept(DIV_SYM, 0);
  } else if(sym -> type == MOD_SYM) {
    accept(MOD_SYM, 0);
  } else if(sym -> type == AMPERSAND_SYM) {
    accept(AMPERSAND_SYM, 0);
  } else {
    error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void designator() {
  writeRule("Designator");
  ++indent;
  accept(IDENT_SYM, 0);
  while(sym -> type == DOT_SYM || sym -> type == LBRAKT_SYM) {
    selector();
  }
  --indent;
}

void selector() {
  writeRule("Selector");
  ++indent;
  if(sym -> type == DOT_SYM) {
    accept(DOT_SYM, 0);
    accept(IDENT_SYM, 0);
  } else if(sym -> type == LBRAKT_SYM) {
    accept(LBRAKT_SYM, 0);
    exprList();
    accept(RBRAKT_SYM, 0);
  } else {
    error(sym -> type, IDENT_SYM);
  }
  --indent;
}

