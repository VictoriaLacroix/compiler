#ifndef __CONST_H__
#define __CONST_H__

/*
 * const.h
 *
 * by  Victoria Lacroix
 * and Alex Arbour
 */

// Defines for boolean types. C doesn't have these!
#define bool  char
#define true  1
#define false 0
#define byte  unsigned char

const int LINE_BUFFER_SIZE   = 256; // Max length of the line buffer
const int TOKEN_BUFFER_SIZE  = 16;  // Max length of a token
const int NUM_RESERVED_WORDS = 56;  // Number of reserved words in the language

//TokenTypes
typedef enum
{ MODULE_SYM
, BEGIN_SYM
, END_SYM
, CONST_SYM
, TYPE_SYM
, VAR_SYM
, PROCEDURE_SYM
, ARRAY_SYM
, OF_SYM
, RECORD_SYM
, EXIT_SYM
, RETURN_SYM
, WHILE_SYM
, DO_SYM
, THEN_SYM
, ELSIF_SYM
, ELSE_SYM
, REPEAT_SYM
, UNTIL_SYM
, FOR_SYM
, TO_SYM
, BY_SYM
, LOOP_SYM
, CASE_SYM
, DIV_SYM     // "DIV"
, MOD_SYM     // "MOD"
, OR_SYM      // "OR"
, ABS_SYM
, BOOLEAN_SYM
, FALSE_SYM
, INTEGER_SYM
, ODD_SYM
, READ_SYM
, READLN_SYM
, TRUE_SYM
, WRITE_SYM
, WRITELN_SYM
, IDENT_SYM
, NUMBER_SYM
, HEX_SYM
, lte         // <=
, gte         // >=
, plus        // +
, minus       // -
, equal       // =
, octothorp   // #
, lt          // <
, gt          // >
, star        // *
, ampersand   // &
, lparan      // (
, rparan      // )
, lbrakt      // [
, rbrakt      // ]
, pipe        // |
, comstr      // (*
, comend      // *)
, UNKNOWN     // placeholder
} TokenType;

struct TokenList {
  char*              token;
  TokenType          type;
  struct TokenList*  next;
};

#endif
