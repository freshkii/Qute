#ifndef __lexer_h__
#define __lexer_h__

#include <stdlib.h>
#include <stdbool.h>

#define TOKENS_DEFAULT_SIZE 100
#define TOKENS_ADDING_SIZE 50


typedef enum {
  CTX_TEXT,       // reading text ~ default
  CTX_TAG,        // after we read the "<" or "</" opening token
  CTX_ATTR_NAME,  // after CTX_TAG, reading the attr name
  CTX_ATTR_VALUE, // after CTX_ATTR_NAME, reading the attr value if there is "="
	CTX_EOF         // end of file
} Ctx;

typedef enum {
  TOKEN_OPEN_TAG,
  TOKEN_CLOSE_TAG,
  TOKEN_SELF_CLOSE_TAG,
  TOKEN_ATTR_NAME,
  TOKEN_ATTR_VALUE,
  TOKEN_TEXT,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  char *content;
} Token;

void init_token(Token *t);

typedef struct {
  Token *list;
  size_t size;
  size_t capacity;
} Tokens;

void init_tokens(Tokens *tl, size_t initial_capacity);
void add_token(Tokens *tl, Token t); // tl for token list

typedef struct {
  const char *txt;
  int txt_len;
  size_t i; // cursor index
  Ctx ctx;
} Lexer;

void init_lexer(Lexer *l, const char *txt);

void tokenize(Tokens *tl, const char *text);
Token get_next_token(Lexer *l);


char peek(Lexer *l);
bool advance(Lexer *l);
char* get_next_string(Lexer *l, const char *banned_chars);
bool match(Lexer *l, char chr);

#endif // __lexer_h__
