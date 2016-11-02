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

// For tokenizing
const char* ignoreChars          = " \n\r\t";
const char* singularTokens       = "+-*=()<>[].,:;#|&";
const char* partialTokens        = "(*<>"; // for (*, *), <= and >=

const int LINE_BUFFER_SIZE   = 256; // Max length of the line buffer
const int TOKEN_BUFFER_SIZE  = 16;  // Max length of a token
const int NUM_RESERVED_WORDS = 59;  // Number of reserved words in the language

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
, IF_SYM
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
, dot         // .
, comma       // ,
, colon       // :
, semicolon   // ;
, cmtstr      // (*
, cmtend      // *)
, IDENT_SYM
, NUMBER_SYM
, HEX_SYM
, UNKNOWN     // placeholder
} TokenType;

//This array is used for output
const char* symNames[] =
{ "MODULE_SYM"
, "BEGIN_SYM"
, "END_SYM"
, "CONST_SYM"
, "TYPE_SYM"
, "VAR_SYM"
, "PROCEDURE_SYM"
, "ARRAY_SYM"
, "OF_SYM"
, "RECORD_SYM"
, "EXIT_SYM"
, "RETURN_SYM"
, "WHILE_SYM"
, "DO_SYM"
, "IF_SYM"
, "THEN_SYM"
, "ELSIF_SYM"
, "ELSE_SYM"
, "REPEAT_SYM"
, "UNTIL_SYM"
, "FOR_SYM"
, "TO_SYM"
, "BY_SYM"
, "LOOP_SYM"
, "CASE_SYM"
, "DIV_SYM"    // "DIV"
, "MOD_SYM"    // "MOD"
, "OR_SYM"     // "OR"
, "ABS_SYM"
, "BOOLEAN_SYM"
, "FALSE_SYM"
, "INTEGER_SYM"
, "ODD_SYM"
, "READ_SYM"
, "READLN_SYM"
, "TRUE_SYM"
, "WRITE_SYM"
, "WRITELN_SYM"
, "<="         // <=
, ">="         // >=
, "+"          // +
, "-"          // -
, "="          // =
, "#"          // #
, "<"          // <
, ">"          // >
, "*"          // *
, "&"          // &
, "("          // (
, ")"          // )
, "["          // [
, "]"          // ]
, "|"          // |
, "."          // .
, ","          // ,
, ":"          // :
, ";"          // ;
, "(*"         // (*
, "*)"         // *)
, "IDENT_SYM"
, "NUMBER_SYM"
, "HEX_SYM"
, "UNKNOWN"    // placeholder
};

// NOTE: this is not a duplicate definition, this instance is an array.
const TokenType reservedWordsSymbols[] =
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
, IF_SYM
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
, DIV_SYM     //"DIV"
, MOD_SYM     //"MOD"
, OR_SYM      //"OR"
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
, dot         // .
, comma       // ,
, colon       // :
, semicolon   // ;
, cmtstr      // (*
, cmtend      // *)
};

//This array is used for logic.
const char* reservedWords[] =
{ "MODULE"
, "BEGIN"
, "END"
, "CONST"
, "TYPE"
, "VAR"
, "PROCEDURE"
, "ARRAY"
, "OF"
, "RECORD"
, "EXIT"
, "RETURN"
, "WHILE"
, "DO"
, "IF"
, "THEN"
, "ELSIF"
, "ELSE"
, "REPEAT"
, "UNTIL"
, "FOR"
, "TO"
, "BY"
, "LOOP"
, "CASE"
, "OR"
, "DIV"
, "MOD"
, "ABS"
, "BOOLEAN"
, "FALSE"
, "INTEGER"
, "ODD"
, "READ"
, "READLN"
, "TRUE"
, "WRITE"
, "WRITELN"
, "<="
, ">="
, "+"
, "-"
, "="
, "#"
, "<"
, ">"
, "*"
, "&"
, "("
, ")"
, "["
, "]"
, "|"
, "."
, ","
, ":"
, ";"
, "(*"
, "*)"
};


struct TokenList {
  char*              token;
  TokenType          type;
  struct TokenList*  next;
};

#endif
