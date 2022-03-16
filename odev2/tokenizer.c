#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

FILE *open(const char *const file_path_,
      const char *const file_mode_)
{
   return fopen(file_path_, file_mode_);
}

bool close(FILE *const stream_)
{
   bool status = true;

   if (stream_ != NULL)
      status = (fclose(stream_) == 0) ? true : false;
   else
      printf("File is already closed\n");

   return status;
}

bool write(FILE *const stream_, const char *const text_)
{
   if (stream_ == NULL)
   {
      printf("File is not opened\n");
      return false;
   }

   fprintf(stream_, "%s", text_);

   return true;
}

char *read(FILE *const stream_)
{
   char *m_destination = (char*) malloc(sizeof(char)*0xF4240); // max 1 Mb
   char m_line;
   int m_line_counter = 0;

   while ((m_line = fgetc(stream_)) != EOF) 
      m_destination[m_line_counter++] = m_line;

   if (strlen(m_destination) == 0)
   {
      free(m_destination);
      return 0;
   }

   return m_destination;
}

typedef enum _char_type
{
   DIGIT,
   LETTER,
   OTHER
} CHR_TYPE;

int length(const char *string_)
{
   return (int)strlen(string_);
}

void fit(char *string_)
{
   char *m_fitted_string = string_;

   do
   {
      while ( (*m_fitted_string == '\n') ||  // for LF
              (*m_fitted_string == '\r' && *(m_fitted_string+1) == '\n') ) // for CRLF
         m_fitted_string++;
   } while ((*string_++ = *m_fitted_string++));
}

void reverse(char *string_)
{
   char *m_reversed_string = (char*)malloc(sizeof(string_) + 1);

   size_t i = 0;
   for (int m_i = strlen(string_) - 1; m_i >= 0; m_i--)
      m_reversed_string[i++] = string_[m_i];

   memcpy(string_, m_reversed_string, strlen(m_reversed_string) + 1);
   free(m_reversed_string);
}

CHR_TYPE type(char char_)
{
   if (isalpha(char_) != 0 || char_ == '_')
      return LETTER;
   else if (isdigit(char_) != 0)
      return DIGIT;
   else
      return OTHER;
}

void append(char *string_, char char_)
{
   char *m_temp_string = (char*)malloc(sizeof(string_) * sizeof(char_) + 1);

   strcpy(m_temp_string, string_);
   strcat(m_temp_string, &char_);
   memcpy(string_, m_temp_string,
         length(m_temp_string) + length(string_) + 1);

   free(m_temp_string);
}

typedef enum _token_name
{
   NUMBER,       // 0
   IDENTIFIER,   // 1
   LPAR,         // 2
   RPAR,         // 3
   LSQB,         // 4
   RSQB,         // 5
   LBRACE,       // 6
   RBRACE,       // 7
   DOT,          // 8
   COMMA,        // 9
   SEMI,         // 10
   PLUS,         // 11
   MINUS,        // 12
   STAR,         // 13
   SLASH,        // 14
   PERCENT,      // 15
   BSLASH,       // 16
   AMPER,        // 17
   PIPE,         // 18
   LESS,         // 19
   GREATER,      // 20
   EQUAL,        // 21
   GREATEREQUAL, // 22
   LESSEQUAL,    // 23
   NOTEQUAL,     // 24
   EQEQUAL,      // 25
   AND,          // 26
   OR,           // 27
   QUOTE,        // 28
   DQUOTE,       // 29
   PLUSEQUAL,    // 30
   MINUSEQUAL,   // 31
   STAREQUAL,    // 32
   SLASHEQUAL,   // 33
   PERCENTEQUAL, // 34
   INT,          // 35
   FLOAT,        // 36
   DOUBLE,       // 37
   CHAR,         // 38
   VOID,         // 39
   IF,           // 40
   ELSEIF,       // 41
   ELSE,         // 42
   FOR,          // 43
   WHILE,        // 44
   PRINTF,       // 45
   SCANF,        // 46
   FORMATINT,    // 47
   FORMATCHAR,   // 48
   FORMATSTRING, // 49
   FORMATFLOAT,  // 50
   NEWLINE,      // 51
   BANG,         // 52
   INCREASE,     // 53
   DECREASE,     // 54
   NIL           // 53
} TOKEN;

TOKEN get_token_type(char char_)
{
   CHR_TYPE m_char_type = type(char_);
   switch (m_char_type)
   {
   case DIGIT:
      return NUMBER;
   case LETTER:
      return IDENTIFIER;
   case OTHER:
      switch (char_)
      {
      case '(':
         return LPAR;
      case ')':
         return RPAR;
      case '[':
         return LSQB;
      case ']':
         return RSQB;
      case ',':
         return COMMA;
      case ';':
         return SEMI;
      case '+':
         return PLUS;
      case '-':
         return MINUS;
      case '*':
         return STAR;
      case '/':
         return SLASH;
      case '\\':
         return BSLASH;
      case '&':
         return AMPER;
      case '<':
         return LESS;
      case '>':
         return GREATER;
      case '=':
         return EQUAL;
      case '!':
         return BANG;
      case '.':
         return DOT;
      case '%':
         return PERCENT;
      case '{':
         return LBRACE;
      case '}':
         return RBRACE;
      case '|':
         return PIPE;
      case '\'':
         return QUOTE;
      case '"':
         return DQUOTE;
      }
   }
   return NIL;
}

void s_getchar(char *string_, int *iter_, char *current_char_, TOKEN *token_type_)
{
   *current_char_ = string_[(*iter_)++];
   *token_type_ = get_token_type(*current_char_);
}

void s_addchar(char *lexeme_, char *m_current_char)
{
   append(lexeme_, *m_current_char);
}

TOKEN get_string_token_type(char *lexeme_)
{
   if (strcmp(lexeme_, ">=") == 0)
      return LESSEQUAL;
   if (strcmp(lexeme_, "<=") == 0)
      return GREATEREQUAL;
   if (strcmp(lexeme_, "!=") == 0)
      return NOTEQUAL;
   if (strcmp(lexeme_, "==") == 0)
      return EQEQUAL;
   if (strcmp(lexeme_, "+=") == 0)
      return PLUSEQUAL;
   if (strcmp(lexeme_, "-=") == 0)
      return MINUSEQUAL;
   if (strcmp(lexeme_, "*=") == 0)
      return STAREQUAL;
   if (strcmp(lexeme_, "/=") == 0)
      return SLASHEQUAL;
   if (strcmp(lexeme_, "%=") == 0)
      return PERCENTEQUAL;
   if (strcmp(lexeme_, "++") == 0)
      return INCREASE;
   if (strcmp(lexeme_, "--") == 0)
      return DECREASE;
   if (strcmp(lexeme_, "%d") == 0)
      return FORMATINT;
   if (strcmp(lexeme_, "%s") == 0)
      return FORMATSTRING;
   if (strcmp(lexeme_, "%c") == 0)
      return FORMATCHAR;
   if (strcmp(lexeme_, "%f") == 0)
      return FORMATFLOAT;
   if (strcmp(lexeme_, "&&") == 0)
      return AND;
   if (strcmp(lexeme_, "||") == 0)
      return OR;
   if (strcmp(lexeme_, "int") == 0)
      return INT;
   if (strcmp(lexeme_, "float") == 0)
      return FLOAT;
   if (strcmp(lexeme_, "char") == 0)
      return CHAR;
   if (strcmp(lexeme_, "double") == 0)
      return DOUBLE;
   if (strcmp(lexeme_, "void") == 0)
      return VOID;
   if (strcmp(lexeme_, "if") == 0)
      return IF;
   if (strcmp(lexeme_, "elseif") == 0)
      return ELSEIF;
   if (strcmp(lexeme_, "else") == 0)
      return ELSE;
   if (strcmp(lexeme_, "for") == 0)
      return FOR;
   if (strcmp(lexeme_, "while") == 0)
      return WHILE;
   if (strcmp(lexeme_, "printf") == 0)
      return PRINTF;
   if (strcmp(lexeme_, "scanf") == 0)
      return SCANF;
   if (strcmp(lexeme_, "\\n") == 0)
      return NEWLINE;

   int type_counter = 0;
   for (int i = 0; i < length(lexeme_); i++)
   {
      if (isdigit(lexeme_[i]))
         type_counter++;
   }

   if (type_counter == length(lexeme_))
      return NUMBER;

   if (length(lexeme_) != 0)
      return IDENTIFIER;

   return NIL;
}

int lexeme(char *string_, char **lexemes_)
{
   char *m_lexeme = (char *)malloc(0x100 * sizeof(char));
   char m_current_char;

   TOKEN m_token_type;

   int m_iterator = 0;
   int lexeme_iter = 0;

   s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);

   while (m_iterator <= length(string_))
   {
      switch (m_token_type)
      {
      case IDENTIFIER:
         s_addchar(m_lexeme, &m_current_char);
         s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);

         while (m_token_type == IDENTIFIER || m_token_type == NUMBER)
         {
            s_addchar(m_lexeme, &m_current_char);
            s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
         }
         break;
      case NUMBER:
         s_addchar(m_lexeme, &m_current_char);
         s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);

         while (m_token_type == NUMBER)
         {
            s_addchar(m_lexeme, &m_current_char);
            s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
         }
         break;
      case LPAR:
      case RPAR:
      case LSQB:
      case RSQB:
      case COMMA:
      case SEMI:
      case PLUS:
      case MINUS:
      case STAR:
      case SLASH:
      case BSLASH:
      case AMPER:
      case LESS:
      case GREATER:
      case EQUAL:
      case BANG:
      case DOT:
      case PERCENT:
      case LBRACE:
      case RBRACE:
      case PIPE:
      case QUOTE:
      case DQUOTE:
         s_addchar(m_lexeme, &m_current_char);
         TOKEN temp_type = m_token_type;
         s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);

         switch (temp_type)
         {
         case PLUS:
         case MINUS:
         case STAR:
         case SLASH:
         case EQUAL:
         case LESS:
         case GREATER:
         case BANG:
            switch (m_token_type)
            {
            case EQUAL:
               s_addchar(m_lexeme, &m_current_char);
               s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
               break;
            case PLUS:
               if (temp_type == PLUS)
               {
                  s_addchar(m_lexeme, &m_current_char);
                  s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
               }
               break;
            case MINUS:
               if (temp_type == MINUS)
               {
                  s_addchar(m_lexeme, &m_current_char);
                  s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
               }
               break;
            }
            break;
         case PERCENT:
            switch (m_current_char)
            {
            case 'd':
            case 'f':
            case 'c':
            case 's':
               s_addchar(m_lexeme, &m_current_char);
               s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
            }
            break;
         case BSLASH:
            switch (m_current_char)
            {
            case 'n':
            case 't':
            case 'r':
               s_addchar(m_lexeme, &m_current_char);
               s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
            }
            break;
         case AMPER:
            switch (m_token_type)
            {
            case AMPER:
               s_addchar(m_lexeme, &m_current_char);
               s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
            }
            break;
            break;
         case PIPE:
            switch (m_token_type)
            {
            case PIPE:
               s_addchar(m_lexeme, &m_current_char);
               s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
            }
            break;
         }
         break;
      }

      if (length(m_lexeme) > 0)
      {
         strcpy(lexemes_[lexeme_iter], m_lexeme);
         lexeme_iter++;
      }

      memset(m_lexeme, 0, sizeof(m_lexeme) + 1);

      if (m_token_type == NIL)
         s_getchar(string_, &m_iterator, &m_current_char, &m_token_type);
   }

   free(m_lexeme);
   return lexeme_iter;
}

int main(int argc, char **argv)
{
   char *m_file_path = (char*)malloc(sizeof(char) + 1);

   if (argc > 1)
      m_file_path = argv[1];
   else
   {
      printf("Enter file path: ");
      scanf("%s", m_file_path);

      if (strlen(m_file_path) <= 0)
      {
         free(m_file_path);
         printf("File does not exist!\n");
         exit(true);
      }
   }


   FILE *m_stream = open(m_file_path, "r");
   char *m_source = read(m_stream);

   char **m_lexemes = (char**) malloc(sizeof(char *) * length(m_source));
   for (int i = 0; i < length(m_source); i++)
      m_lexemes[i] = (char*) calloc(0x100, sizeof(char));

   int size;
   size = lexeme(m_source, m_lexemes);

   printf("%s", m_source);

   for (int i = 0; i < size; i++)
   {
      printf("%s", m_lexemes[i]);

      if (i != size-1) {
        printf(",");
      }
   }

   return 0;
}
