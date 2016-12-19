#ifndef CONST_H_
#define CONST_H_

// Defines for boolean types. C doesn't have these!
typedef char  bool;
#define true  1
#define false 0

extern const char* ignoreChars;

extern const char* singularTokens;

extern const char* doubleTokens;

extern const int LINE_BUFFER_SIZE;
extern const int TOKEN_BUFFER_SIZE;
extern const int RULE_BUFFER_SIZE;
extern const int NUM_RESERVED_WORDS;

// TokenType enum allowing recognized tokens to be expressed as a data type
// Constant lists use haskell-style syntax as they are very easy to expand and
// edit in bulk using vi commands
typedef enum
{ MODULE_SYM
, IMPORT_SYM
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
, LTE_SYM         // <=
, GTE_SYM         // >=
, PLUS_SYM        // +
, MINUS_SYM       // -
, EQUALS_SYM       // =
, ASSIGN_SYM      // :=
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
, DOT_DOT_SYM     // .
, COMMA_SYM       // ,
, COLON_SYM       // :
, SEMICOLON_SYM   // ;
, TILDE_SYM       // ~
, CMT_STR_SYM     // (*
, CMT_END_SYM     // *)
, IDENT_SYM
, NUMBER_SYM
, HEX_SYM
, UNKNOWN_SYM     // placeholder/error
} TokenType;

extern const char*      symNames[];
extern const TokenType  reservedWordsSymbols[];
extern const char*      reservedWords[];
extern const char*      predefIdents[];

/*
 * Ultimately unnecessary given how errors were handled.
typedef enum
{ ERR_UNKNOWN
} ErrTypes;

extern const char* errmsgs[];
*/

struct TokenList {
  char*              token;
  TokenType          type;
  //TODO implement line numbers for better error handling
  //int                linenumber;
  struct TokenList*  prev;
  struct TokenList*  next;
};

#endif
