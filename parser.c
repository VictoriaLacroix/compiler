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

bool accept(TokenType expected) {
  if(sym -> type == expected) {
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
    if(i < indent - 1) {
      printf("| ");
    } else {
      printf("+ ");
    }
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
  printf(" (accepted)\n");
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
  accept(MODULE_SYM);
  accept(IDENT_SYM);
  accept(SEMICOLON_SYM);
  block();
  accept(DOT_SYM);
  --indent;
}

void block() {
  writeRule("Block");
  ++indent;
  declSeq();
  if(sym -> type == BEGIN_SYM) {
    accept(BEGIN_SYM);
    statSeq();
  }
  accept(END_SYM);
  accept(IDENT_SYM);
  accept(SEMICOLON_SYM);
  --indent;
}

void declSeq() {
  writeRule("DeclSeq");
  ++indent;
  if(sym -> type == CONST_SYM) {
    accept(CONST_SYM);
    constDecls();
  }
  if(sym -> type == TYPE_SYM) {
    accept(TYPE_SYM);
    typeDecls();
  }
  if(sym -> type == VAR_SYM) {
    accept(VAR_SYM);
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
  accept(IDENT_SYM);
  accept(EQUALS_SYM);
  constExpr();
  accept(SEMICOLON_SYM);
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
  accept(IDENT_SYM);
  accept(EQUALS_SYM);
  type();
  accept(SEMICOLON_SYM);
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
  accept(COLON_SYM);
  type();
  accept(SEMICOLON_SYM);
  --indent;
}

void identList() {
  writeRule("IdentList");
  ++indent;
  accept(IDENT_SYM);
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM);
    accept(IDENT_SYM);
  }
  --indent;
}

void procDecl() {
  writeRule("ProcDecl");
  ++indent;
  accept(PROCEDURE_SYM);
  accept(IDENT_SYM);
  if(sym -> type == LPAREN_SYM) {
    formalPars();
  }
  if(sym -> type == COLON_SYM) {
    accept(COLON_SYM);
    accept(IDENT_SYM);
  }
  accept(SEMICOLON_SYM);
  procBody();
  --indent;
}

void formalPars() {
  writeRule("FormalPars");
  ++indent;
  accept(LPAREN_SYM);
  if(sym -> type == VAR_SYM || sym -> type == IDENT_SYM) {
    fpSection();
    while(sym -> type == SEMICOLON_SYM) {
      accept(SEMICOLON_SYM);
      fpSection();
    }
  }
  accept(RPAREN_SYM);
  --indent;
}

void fpSection() {
  writeRule("FPSection");
  ++indent;
  if(sym -> type == VAR_SYM) {
    accept(VAR_SYM);
  }
  identList();
  accept(COLON_SYM);
  formalType();
  --indent;
}

void formalType() {
  writeRule("FormalType");
  ++indent;
  accept(IDENT_SYM);
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
      accept(IDENT_SYM);
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
  accept(ARRAY_SYM);
  length();
  while(sym -> type == COMMA_SYM) {
    length();
  }
  accept(OF_SYM);
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
  accept(RECORD_SYM);
  fieldList();
  while(sym -> type == SEMICOLON_SYM) {
    accept(SEMICOLON_SYM);
    fieldList();
  }
  --indent;
}

void enumType() {
  writeRule("EnumType");
  ++indent;
  accept(LPAREN_SYM);
  accept(IDENT_SYM);
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM);
    accept(IDENT_SYM);
  }
  accept(RPAREN_SYM);
  --indent;
}

void fieldList() {
  writeRule("FieldList");
  ++indent;
  if(sym -> type == IDENT_SYM) {
    identList();
    accept(COLON_SYM);
    type();
  }
  --indent;
}

void statSeq() {
  writeRule("StatSeq");
  ++indent;
  stat();
  while(sym -> type == SEMICOLON_SYM) {
    accept(SEMICOLON_SYM);
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
      accept(EXIT_SYM);
      break;
    case RETURN_SYM:
      accept(RETURN_SYM);
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
  accept(ASSIGN_SYM);
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
  accept(WHILE_SYM);
  expr();
  accept(DO_SYM);
  statSeq();
  while(sym -> type == ELSIF_SYM) {
    accept(ELSIF_SYM);
    expr();
    accept(DO_SYM);
    statSeq();
  }
  if(sym -> type == ELSE_SYM){
    accept(ELSE_SYM);
    statSeq();
  }
  accept(END_SYM);
  --indent;
}

void ifStat() {
  writeRule("IfStat");
  ++indent;
  accept(IF_SYM);
  expr();
  accept(THEN_SYM);
  statSeq();
  while(sym -> type == ELSIF_SYM) {
    accept(ELSIF_SYM);
    expr();
    accept(THEN_SYM);
    statSeq();
  }
  if(sym -> type == ELSE_SYM){
    accept(ELSE_SYM);
    statSeq();
  }
  accept(END_SYM);
  --indent;
}

void actParams() {
  writeRule("ActParams");
  ++indent;
  accept(LPAREN_SYM);
  if(sym -> type == PLUS_SYM || sym -> type == MINUS_SYM
      || sym -> type == IDENT_SYM) {
    exprList();
  }
  accept(RPAREN_SYM);
  --indent;
}

void exprList() {
  writeRule("ExprList");
  ++indent;
  expr();
  while(sym -> type == COMMA_SYM) {
    accept(COMMA_SYM);
    expr();
  }
  --indent;
}

void repeatStat() {
  writeRule("RepeatStat");
  ++indent;
  accept(REPEAT_SYM);
  statSeq();
  accept(UNTIL_SYM);
  expr();
  --indent;
}

void forStat() {
  writeRule("ForStat");
  ++indent;
  accept(FOR_SYM);
  accept(IDENT_SYM);
  accept(ASSIGN_SYM);
  expr();
  accept(TO_SYM);
  expr();
  if(sym -> type == BY_SYM) {
    accept(BY_SYM);
    constExpr();
  }
  accept(DO_SYM);
  statSeq();
  accept(END_SYM);
  --indent;
}

void loopStat() {
  writeRule("LoopStat");
  ++indent;
  accept(LOOP_SYM);
  statSeq();
  accept(END_SYM);
  --indent;
}

void caseStat() {
  writeRule("CaseStat");
  ++indent;
  accept(CASE_SYM);
  expr();
  accept(OF_SYM);
  caseOpt();
  while(sym -> type == PIPE_SYM) {
    accept(PIPE_SYM);
    caseOpt();
  }
  if(sym -> type == ELSE_SYM) {
    accept(ELSE_SYM);
    statSeq();
  }
  accept(END_SYM);
  --indent;
}

void caseOpt() {
  writeRule("Case");
  ++indent;
  if(sym -> type == PLUS_SYM || sym -> type == MINUS_SYM
      || sym -> type == IDENT_SYM) {
    caseLabs();
    while(sym -> type == COMMA_SYM) {
      accept(COMMA_SYM);
      caseLabs();
    }
    accept(COLON_SYM);
    statSeq();
  }
  --indent;
}

void caseLabs() {
  writeRule("CaseLabs");
  ++indent;
  constExpr();
  if(sym -> type == DOT_SYM) {
    accept(DOT_DOT_SYM);
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
    accept(PLUS_SYM);
  } else if(sym -> type == MINUS_SYM) {
    accept(MINUS_SYM);
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
      accept(NUMBER_SYM);
      break;
    case HEX_SYM:
      accept(HEX_SYM);
      break;
    case LPAREN_SYM:
      accept(LPAREN_SYM);
      expr();
      accept(RPAREN_SYM);
      break;
    case TILDE_SYM:
      accept(TILDE_SYM);
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
      accept(OR_SYM);
      break;
    case MINUS_SYM :
      accept(MINUS_SYM);
      break;
    case PLUS_SYM :
      accept(PLUS_SYM);
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
      accept(EQUALS_SYM);
      break;
    case OCTOTHORPE_SYM :
      accept(OCTOTHORPE_SYM);
      break;
    case LT_SYM :
      accept(LT_SYM);
      break;
    case GT_SYM :
      accept(GT_SYM);
      break;
    case LTE_SYM :
      accept(LTE_SYM);
      break;
    case GTE_SYM :
      accept(GTE_SYM);
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
    accept(STAR_SYM);
  } else if(sym -> type == DIV_SYM) {
    accept(DIV_SYM);
  } else if(sym -> type == MOD_SYM) {
    accept(MOD_SYM);
  } else if(sym -> type == AMPERSAND_SYM) {
    accept(AMPERSAND_SYM);
  } else {
    error(sym -> type, IDENT_SYM);
  }
  --indent;
}

void designator() {
  writeRule("Designator");
  ++indent;
  accept(IDENT_SYM);
  while(sym -> type == DOT_SYM || sym -> type == LBRAKT_SYM) {
    selector();
  }
  --indent;
}

void selector() {
  writeRule("Selector");
  ++indent;
  if(sym -> type == DOT_SYM) {
    accept(DOT_SYM);
    accept(IDENT_SYM);
  } else if(sym -> type == LBRAKT_SYM) {
    accept(LBRAKT_SYM);
    exprList();
    accept(RBRAKT_SYM);
  } else {
    error(sym -> type, IDENT_SYM);
  }
  --indent;
}

