#include "lexer.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// general func
char *strccat(char *string, char chr);

// lexer funcs
char peek(Lexer *l);
bool advance(Lexer *l);
char* get_next_string(Lexer *l, bool alpha_only, char *banned_chars);
bool match(Lexer *l, char chr);
Token get_next_token(Lexer *l);


Token get_next_token(Lexer *l) {
  Token t;
  init_token(&t);

  const Ctx ctx = l->ctx;
  while(ctx == l->ctx && advance(l)) {
    switch(ctx) {
      case CTX_TEXT:
        switch(l->txt[l->i]) {
          case '<':
            l->ctx = CTX_TAG;
            break;
          case '\n':
            break;
          default:
            strccat(t.content, l->txt[l->i]);
            break;
        }
        break;
      case CTX_TAG:
        if(match(l, '/')) {
          t.type = TOKEN_CLOSE_TAG;
        }
        else {
          t.type = TOKEN_OPEN_TAG;
        }
        t.content = get_next_string(l, true, "");
        l->ctx = CTX_ATTR_NAME;
      case CTX_ATTR_NAME:
        switch(l->txt[l->i]) {
          case ' ':
            break;
          case '=':
            l->ctx = CTX_ATTR_NAME;
            break;
          case '>':
            l->ctx = CTX_TEXT;
            break;
          default:
            t.content = get_next_string(l, true, "");
            break;
        }
        break;
      case CTX_ATTR_VALUE:
        switch(l->txt[l->i]) {
          case ' ':
            l->ctx = CTX_ATTR_NAME;
            break;
          default:
            t.content = get_next_string(l, true, "");
            break;
        }
        break;
    }
  }
  return t;
}

void init_tokens(Tokens *tl, size_t initial_capacity) {
  tl->size = 0;
  tl->capacity = initial_capacity;
  tl->list = (Token*)malloc(tl->capacity * sizeof(Token));
}

void add_token(Tokens *tl, Token t) {
  if (tl->size >= tl->capacity) {
    tl->capacity += 10;
    tl->list = reallocarray(tl->list, tl->capacity + 10, sizeof(Token));
  }
  tl->list[tl->size] = t;
  tl->size++;
}

void init_token(Token *t) {
  t->content = (char*) malloc(0);
  t->type = TOKEN_TEXT;
}

bool tokenize(Tokens *tl, const char *text) {
  Lexer l;
  l.txt = text;
  l.txt_len = strlen(text);
  l.i = 0;
  l.ctx = CTX_TEXT;

  while (peek(&l) != EOF) {
    Token t = get_next_token(&l);

    // check for token validity
    if (t.type == TOKEN_EOF) {
      break;
    }
    else if (t.type == TOKEN_TEXT && t.content == "") {
      continue;
    }

    add_token(tl, t);
  }
}


char* strccat(char *string, char chr) {
  char tmp[1];
  tmp[0] = chr;
  return strcat(string, tmp);
}

bool advance(Lexer *l) {
  if (l->i +1 < l->txt_len) {
    l->i++;
    return true;
  }
  else {
    return false;
  }
}

char peek(Lexer *l) {
  if(l->i + 1 < l->txt_len)
    return l->txt[l->i+1];
  return EOF;
}

char* get_next_string(Lexer *l, bool alpha_only, char *banned_chars) {
  char* string;
  char n = peek(l);
  if (alpha_only) {
    while(isalpha(n) && !strchr(banned_chars, (int)n)) {
      strccat(string, n);
      l->i++;
      n = peek(l);
    }
  }
  else {
    while(!strchr(banned_chars, n)) {
      strccat(string, n);
      l->i++;
      n = peek(l);
    }
  }
  return string;
}

bool match(Lexer *l, char chr) {
  return (peek(l) == chr);
}
