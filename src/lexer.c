#include "lexer.h"
#include <string.h>


void init_token(Token *t) {
	t->type = TOKEN_EOF;
	t->content = (char*)malloc(0);
}

void init_tokens(Tokens *tl, size_t initial_capacity) {
  tl->list = (Token*)malloc(sizeof(Token)*initial_capacity);
	tl->size = 0;
	tl->capacity = initial_capacity;
}

void add_token(Tokens *tl, Token t) {
	if (tl->size == tl->capacity) {
		tl->capacity += TOKENS_ADDING_SIZE;
		tl->list = realloc(tl->list, sizeof(Token) * tl->capacity);
	}
	tl->list[tl->size++] = t;
}

void init_lexer(Lexer *l,const char *txt) {
	l->txt = txt;
	l->txt_len = strlen(txt);
	l->i = 0;
	l->ctx = CTX_TEXT;
}


char* strccat(char *string, char chr) {
	size_t len = strlen(string);
	char *output = (char*)malloc(len+2);
	if (!output) { exit(1); }

	strcpy(output,string);
	output[len] = chr;
	output[len+1] = '\0';

	return output;
}

bool advance(Lexer *l) {
  if (l->i +1 < l->txt_len) {
    l->i++;
    return true;
  }
	return false;
}

char peek(Lexer *l) {
  if(l->i + 1 < l->txt_len)
    return l->txt[l->i+1];
  return '\0';
}

char* get_next_string(Lexer *l, const char *banned_chars) {
  char* string = "";
  char n = l->txt[l->i];
	while(!strchr(banned_chars, (int)n) && n != ' ' && advance(l)) {
		string = strccat(string, n);
		n = l->txt[l->i];
	}
	l->i--;
  return string;
}

bool match(Lexer *l, char chr) {
  return (l->txt[l->i] == chr);
}

Token get_next_token(Lexer *l) {
	Token t;
	init_token(&t);

	const Ctx ctx = l->ctx;

	do {
		switch(ctx) {
			case CTX_TEXT:
				t.type = TOKEN_TEXT;
				switch(l->txt[l->i]) {
					case '<':
						l->ctx = CTX_TAG;
						break;
					case '\n':
						break;
					default:
						t.content = strccat(t.content, l->txt[l->i]);
						break;
				}
				break;
			case CTX_TAG:
				if (match(l,'/')) {
					t.type = TOKEN_CLOSE_TAG;
					advance(l);
				}
				else {
					t.type = TOKEN_OPEN_TAG;
				}
				t.content = get_next_string(l, ">");
				if (l->txt[l->i] != '>' && t.type == TOKEN_OPEN_TAG) {
					l->ctx = CTX_ATTR_NAME;
				}
				else {
					l->ctx = CTX_TEXT;
					advance(l);
				}
				break;
			case CTX_ATTR_NAME:
				t.type = TOKEN_ATTR_NAME;
        switch(l->txt[l->i]) {
          case ' ':
            break;
          case '=':
							l->ctx = CTX_ATTR_VALUE;
            break;
          case '>':
            l->ctx = CTX_TEXT;
            break;
          default:
            t.content = get_next_string(l, "=");
            break;
        }
				break;
			case CTX_ATTR_VALUE:
				t.type = TOKEN_ATTR_VALUE;
        switch(l->txt[l->i]) {
					case '"':
						break;
          case ' ':
            l->ctx = CTX_ATTR_NAME;
            break;
          default:
            t.content = get_next_string(l, "\"");
            l->ctx = CTX_ATTR_NAME;
						advance(l);
            break;
        }
				break;
		}
  } while(ctx == l->ctx && advance(l));
	
	if (!advance(l))
		l->ctx = CTX_EOF;

	return t;
}


void tokenize(Tokens *tl, const char *text) {
	Lexer l;
	init_lexer(&l, text);

	Token t = get_next_token(&l);
	do {
		t = get_next_token(&l);
		if (strcmp(t.content, "")) {
			add_token(tl, t);
		}
	} while (l.ctx != CTX_EOF);
}

