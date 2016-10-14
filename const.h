#ifndef __CONST_H__
#define __CONST_H__

// Defines for boolean types. C doesn't have these!
#define bool  char
#define true  1
#define false 0

const int LINE_BUFFER_SIZE  = 256; // Max length of the line buffer
const int TOKEN_BUFFER_SIZE = 16;  // Max length of a token

enum TokenTypes {
  //TODO: consult oberon-s grammar and finish this.
  UNKNOWN,
  MODULE,
  IF_START,
  IF_END,
  IDENTIFIER
};

struct TokenList {
  char*              token;
  enum TokenTypes    type;
  struct TokenList*  next;
};


#endif
