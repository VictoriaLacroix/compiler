#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include "const.h"

extern struct TokenList* sym;
extern int    indent;

void parse(struct TokenList*);
bool init(struct TokenList*);
bool accept(TokenType);
void nextSym();
void writeSym();
void writeRule(const char*);
void error(TokenType, TokenType);

void module();
void block();
void declSeq();
void constDecls();
void constDecl();
void typeDecls();
void typeDecl();
void varDecls();
void varDecl();
void identList();
void procDecl();
void formalPars();
void fpSection();
void formalType();
void procBody();
void type();
void arrayType();
void length();
void recordType();
void enumType();
void fieldList();
void statSeq();
void stat();
void assignOrProc();
void assignStat();
void whileStat();
void ifStat();
void procCall();
void actParams();
void exprList();
void repeatStat();
void forStat();
void loopStat();
void caseStat();
void caseOpt();
void caseLabs();
void constExpr();
void expr();
void simplExpr();
void term();
void factor();
void addOp();
void relation();
void mulOp();
void designator();
void selector();

bool isAddOp(TokenType);
bool isRelation(TokenType);
bool isMulOp(TokenType);
bool isNumber(TokenType);

#endif
