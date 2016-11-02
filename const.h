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
//
const char* ignoreChars          = " \n\r\t";

// list of chars that require input to be looked ahead
const char* partialTokens        = "(*<>"; // for (*, *), <= and >=

//list of chars that automatically break a token
const char* singularTokens       = "+-*=()<>[].,:;#|&";

const int LINE_BUFFER_SIZE   = 256; // Max length of the line buffer
const int TOKEN_BUFFER_SIZE  = 16;  // Max length of a token
const int NUM_RESERVED_WORDS = 59;  // Number of reserved words in the language

// TokenTypes enum allowing recognized tokens to be expressed as a data type
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
, DIV_SYM         // "DIV"
, MOD_SYM         // "MOD"
, OR_SYM          // "OR"
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
, LTE_SYM         // <=
, GTE_SYM         // >=
, PLUS_SYM        // +
, MINUS_SYM       // -
, EQUAL_SYM       // =
, OCTOTHORPE_SYM  // #
, LT_SYM          // <
, GT_SYM          // >
, STAR_SYM        // *
, AMPERSAND_SYM   // &
, LPAREN_SYM      // (
, RPAREN_SYM      // )
, LBRAKT_SYM      // [
, RBRAKT_SYM      // ]
, PIPE_SYM        // |
, DOT_SYM         // .
, COMMA_SYM       // ,
, COLON_SYM       // :
, SEMICOLON_SYM   // ;
, CMT_STR_SYM     // (*
, CMT_END_SYM     // *)
, IDENT_SYM
, NUMBER_SYM
, HEX_SYM
, UNKNOWN_SYM     // placeholder
} TokenType;

//This array is used for outputting symbol types
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
, "DIV_SYM"           // "DIV"
, "MOD_SYM"           // "MOD"
, "OR_SYM"            // "OR"
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
, "LTE_SYM"           // <=
, "GTE_SYM"           // >=
, "PLUS_SYM"          // +
, "MINUS_SYM"         // -
, "EQUALS_SYM"        // =
, "OCTOTHORPE_SYM"    // #
, "LT_SYM"            // <
, "GT_SYM"            // >
, "STAR_SYM"          // *
, "AMPERSAND_SYM"     // &
, "LPAREN_SYM"        // (
, "RPAREN_SYM"        // )
, "LBRAKT_SYM"        // [
, "RBRAKT_SYM"        // ]
, "PIPE_SYM"          // |
, "DOT_SYM"           // .
, "COMMA_SYM"         // ,
, "COLON_SYM"         // :
, "SEMICOLON_SYM"     // ;
, "CMT_STR_SYM"       // (*
, "CMT_END_SYM"       // *)
, "IDENT_SYM"
, "NUMBER_SYM"
, "HEX_SYM"
, "UNKNOWN_SYM"       // placeholder
};

// Array of syms representing reversed words and operations
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
, DIV_SYM         //"DIV"
, MOD_SYM         //"MOD"
, OR_SYM          //"OR"
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
, LTE_SYM         // <=
, GTE_SYM         // >=
, PLUS_SYM        // +
, MINUS_SYM       // -
, EQUAL_SYM       // =
, OCTOTHORPE_SYM  // #
, LT_SYM          // <
, GT_SYM          // >
, STAR_SYM        // *
, AMPERSAND_SYM   // &
, LPAREN_SYM      // (
, RPAREN_SYM      // )
, LBRAKT_SYM      // [
, RBRAKT_SYM      // ]
, PIPE_SYM        // |
, DOT_SYM         // .
, COMMA_SYM       // ,
, COLON_SYM       // :
, SEMICOLON_SYM   // ;
, CMT_STR_SYM     // (*
, CMT_END_SYM     // *)
};

// Reserved word strings to compare against.
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
