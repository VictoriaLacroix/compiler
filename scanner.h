#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdlib.h>
#include <stdio.h>
#include "const.h"

// Pre-defining all functions
// tokenizing functions
bool              isIgnoreChar(const char);
bool              isDoubleToken(const char, const char);
bool              isWordEnd(const char);
struct TokenList* createToken(char*);
struct TokenList* splitTokens(const char*);
void              setPrevs(struct TokenList* tokens);

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
void              appendTokenList(struct TokenList*, struct TokenList*);
bool              removeComments(struct TokenList*);
struct TokenList* findCommentEnd(struct TokenList*);
void              printTokens(const struct TokenList*);
void              freeTokens(struct TokenList*);

#endif
