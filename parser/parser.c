/*
  <if-expr> : if (<cond>) 
                <word>
              {elseif (<cond>)
                <word>
              }
              [else <word>]
 
  <cond> : <id> <op> <id>
  <id> : a | b | c | d | e | f | <id> | <number>
  <op> : > | < | <= | >= | > | ==
  <word> : <id> = <id> + <id>
          |<id> = <id> - <id>
          |<id> = <id> * <id>
          |<id> = <id> / <id>
          |<id> = <id>
  <number> : <digit> | <digit><number>
  <digit> : 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define CHUNK        1024

#define NONE         2
#define NUMBER       4
#define IDENTIFIER   6
#define LPAR         8
#define RPAR         10
#define LBRACE       12
#define RBRACE       14
#define PLUS         16
#define MINUS        18
#define STAR         20
#define SLASH        22
#define BANG         24
#define EQUAL        26
#define LESS         28
#define GREATER      30
#define EQEQUAL      32
#define LESSEQUAL    34
#define GREATEREQUAL 36
#define NOTEQUAL     38
#define SPACE        40
#define NEWLINE      42
#define IFEXPR       44


typedef struct token {
	int type;
	int lineno;
	int begin_colno_offset;
	int end_colno_offset;
	char *text;
	struct token *next;
} TOKEN;

int GetType(char chr)
{
   switch (chr) 
   {
   case '(':
      return LPAR;
   case ')':
      return RPAR;
   case '{':
      return LBRACE;
   case '}':
      return RBRACE;
   case '<':
      return LESS;
   case '>':
      return GREATER;
   case '=':
      return EQUAL;
   case '+':
      return PLUS;
   case '-':
      return MINUS;
   case '*':
      return STAR;
   case '/':
      return SLASH;
   case '!':
      return BANG;
   case ' ':
      return SPACE;
   case '\n':
   case '\r':
      return NEWLINE;
   }

   if (isalpha(chr) != 0)
      return IDENTIFIER;
   else if (isdigit(chr) != 0)
      return NUMBER;

  return NONE;
}

int GetTokenType(char *chr)
{
   if (strcmp(chr, ">=") == 0)
      return LESSEQUAL;
   if (strcmp(chr, "<=") == 0)
      return GREATEREQUAL; 
   if (strcmp(chr, "!=") == 0)
      return NOTEQUAL;
   if (strcmp(chr, "==") == 0)
      return EQEQUAL;
   if (strcmp(chr, "if") == 0)
      return IFEXPR;

   if (strlen(chr) == 1) {
      return GetType(chr[0]);
   }

   int i;
   int type_counter = 0;
   int size = strlen(chr);

   for (i = 0; i < size; i++)
   {
      if (isdigit(chr[i]))
         type_counter++;
   }

   if (type_counter == size)
      return NUMBER;

   if (size != 0)
      return IDENTIFIER;

   return NONE;
}

char* GetTokenTypeName(int token_type)
{
   switch (token_type)
   {
   case NONE:
      return "NONE";
   case NUMBER:
      return "NUMBER";
   case IDENTIFIER:
      return "IDENTIFIER";
   case LPAR:
      return "LPAR";
   case RPAR:
      return "RPAR";
   case LBRACE:
      return "LBRACE";
   case RBRACE:
      return "RBRACE";
   case PLUS:
      return "PLUS";
   case MINUS:
      return "MINUS";
   case STAR:
      return "STAR";
   case SLASH:
      return "SLASH";
   case BANG:
      return "BANG";
   case EQUAL:
      return "EQUAL";
   case LESS:
      return "LESS";
   case GREATER:
      return "GREATER";
   case EQEQUAL:
      return "EQEQUAL";
   case LESSEQUAL:
      return "LESSEQUAL";
   case GREATEREQUAL:
      return "GREATEREQUAL";
   case NOTEQUAL:
      return "NOTEQUAL";
   case SPACE:
      return "SPACE";
   case NEWLINE:
      return "NEWLINE";
   case IFEXPR:
      return "IFEXPR";
   }

   return "NONE";
}


void NextToken(char *text, char *buffer, int *iter, char *current_char, int *current_type, int *begin_colno_offset, int *end_colno_offset)
{
   strncat(text, current_char, 1);

   *current_char = buffer[*iter];
   *current_type = GetType(*current_char);
   //printf("%c\t%d\t%d\n", *current_char, *current_type, *iter);
   (*end_colno_offset)++;
   (*iter)++;
}

TOKEN* CreateToken(char *text, int lineno, int begin_colno_offset, int end_colno_offset, TOKEN *next_token)
{
   next_token->text = (char*) malloc(sizeof(char));
   strcpy(next_token->text, text);
   next_token->lineno = lineno;
   next_token->begin_colno_offset = begin_colno_offset;
   next_token->end_colno_offset = end_colno_offset;
   next_token->type = GetTokenType(text);

   return next_token->next;
}

TOKEN* StartTokenize(char *buffer, int size)
{

   int current_type;
   int lineno = 1;
   int begin_colno_offset = 1, end_colno_offset = 0;
   int iter = 0;

   char current_char;

   char *text = (char*) malloc(sizeof(char)*256); 

   TOKEN *token_root = (TOKEN*) malloc(sizeof(TOKEN));
   TOKEN *token_iter = token_root;

   NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);

   while (iter <= size)
   {
      switch (current_type)
      {
      case IDENTIFIER:
         do {
            NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);
         } while (current_type == IDENTIFIER || current_type == NUMBER);
         break;
      case NUMBER:
         do {
            NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);
         } while (current_type == NUMBER);
         break;
      case EQUAL:
      case GREATER:
      case LESS:
         NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);

         switch (current_type)
         {
         case EQUAL:
            NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);
            break;
         }
         break;
      case NEWLINE:
         lineno++;
         begin_colno_offset = 1; end_colno_offset = 0;
         NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);
         break;
      case NONE:
         return NULL;
      default:
         NextToken(text, buffer, &iter, &current_char, &current_type, &begin_colno_offset, &end_colno_offset);
         break;
      }

      token_iter->next = (TOKEN*) malloc(sizeof(TOKEN));
      token_iter = CreateToken(text, lineno, begin_colno_offset, end_colno_offset, token_iter);
      memset(text, 0, sizeof(text)+1);
      begin_colno_offset = end_colno_offset;
   }

   return token_root;
}

void StartParser(TOKEN *token)
{

}

int main(int argc, char **argv)
{
	char *file_name = (char*) malloc(sizeof(char)*256);

	if (argc > 1)
	{
      file_name = argv[1];
	} else {
      printf("Enter File Name: ");
      scanf("%s", file_name);

      if (strlen(file_name) <= 0) {
         printf("File does not exist!\n");
         exit(EXIT_FAILURE);
      }
   }

   FILE *file;
   int nread;
   char *buffer = malloc(sizeof(char)*CHUNK);

   if (buffer == NULL) {
      printf("MALLOC FAILURE");
      exit(EXIT_FAILURE);
   }

   file = fopen(file_name, "r");
   int lc = 0;
   char line;

   while ((line = fgetc(file)) != EOF) 
      buffer[lc++] = line;

   if (strlen(buffer) == 0)
   {
      free(buffer);
      return 0;
   }

   TOKEN* token = StartTokenize(buffer, lc); 

   if (token == NULL) 
   {
      printf("FAILED: Tokenizer\n");
      exit(EXIT_FAILURE);
   }

#ifdef VERBOSE 
      printf("TOKEN\tLINE\tBEGIN\tEND\tTYPE\n--------------------------------------------\n");
      while (token != NULL && token->next != NULL) {
         if (token->type == NEWLINE) {
            printf("\\n");
         } else if (token->type == SPACE) {
            printf("''");
         } else {
            printf("%s", token->text);
         }
         printf("\t%d\t%d\t%d\t%s\n", token->lineno, token->begin_colno_offset, token->end_colno_offset, GetTokenTypeName(token->type));
         token = token->next;
      }
#endif

   StartParser(token);

   fclose(file);
   return EXIT_SUCCESS;
}
