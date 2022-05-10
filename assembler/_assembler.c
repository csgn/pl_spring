#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdint.h>
#include <ctype.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHITE "\x1B[37m"
#define BLUE "\x1B[36m"
#define GRY "\x1B[30m"
#define BGRY "\x1B[40m"
#define BRED "\x1B[41m"
#define RESET "\033[0m"



/* UTILS */
// mask
#define h(n) n & 0xFF

// hex to bin
char *BIN(const int8_t hex)
{
    char *binary = (char *)malloc(9);
    char *hex_buffer = malloc(3);

    sprintf(hex_buffer, "%02X", hex & 0xFF);

    char ch1 = hex_buffer[1];
    char ch2 = hex_buffer[0];

    char *bin1 = malloc(sizeof(char) * 4);
    char *bin2 = malloc(sizeof(char) * 4);

    switch (ch1)
    {
    case '0':
        bin1 = "0000";
        break;
    case '1':
        bin1 = "0001";
        break;
    case '2':
        bin1 = "0010";
        break;
    case '3':
        bin1 = "0011";
        break;
    case '4':
        bin1 = "0100";
        break;
    case '5':
        bin1 = "0101";
        break;
    case '6':
        bin1 = "0110";
        break;
    case '7':
        bin1 = "0111";
        break;
    case '8':
        bin1 = "1000";
        break;
    case '9':
        bin1 = "1001";
        break;
    case 'a':
    case 'A':
        bin1 = "1010";
        break;
    case 'b':
    case 'B':
        bin1 = "1011";
        break;
    case 'c':
    case 'C':
        bin1 = "1100";
        break;
    case 'd':
    case 'D':
        bin1 = "1101";
        break;
    case 'e':
    case 'E':
        bin1 = "1110";
        break;
    case 'f':
    case 'F':
        bin1 = "1111";
        break;
    }

    switch (ch2)
    {
    case '0':
        bin2 = "0000";
        break;
    case '1':
        bin2 = "0001";
        break;
    case '2':
        bin2 = "0010";
        break;
    case '3':
        bin2 = "0011";
        break;
    case '4':
        bin2 = "0100";
        break;
    case '5':
        bin2 = "0101";
        break;
    case '6':
        bin2 = "0110";
        break;
    case '7':
        bin2 = "0111";
        break;
    case '8':
        bin2 = "1000";
        break;
    case '9':
        bin2 = "1001";
        break;
    case 'a':
    case 'A':
        bin2 = "1010";
        break;
    case 'b':
    case 'B':
        bin2 = "1011";
        break;
    case 'c':
    case 'C':
        bin2 = "1100";
        break;
    case 'd':
    case 'D':
        bin2 = "1101";
        break;
    case 'e':
    case 'E':
        bin2 = "1110";
        break;
    case 'f':
    case 'F':
        bin2 = "1111";
        break;
    }

    strcpy(binary, bin2);
    strcat(binary, bin1);
    free(hex_buffer);
    binary[8] = '\0';

    return binary;
}

// bin to hex
int8_t HEX(const char *binary)
{
    return h(strtol(binary, NULL, 2));
}


typedef struct 
{
      FILE *stream;
      char* path;
      int size;
      bool status;
      char* mode;
} StreamObject;

StreamObject *open_stream(char *_filepath, char *_mode)
{
    StreamObject *temp_streamObject = malloc(sizeof(StreamObject));
    temp_streamObject->path = _filepath;
    temp_streamObject->mode = _mode;
    temp_streamObject->stream = fopen(temp_streamObject->path,
                                      temp_streamObject->mode);
    struct stat st;
    stat(temp_streamObject->path, &st);
    temp_streamObject->size = st.st_size;

    if (!temp_streamObject->stream)
    {
        perror("File Does Not Exist");
        free(temp_streamObject);
        return 0;
    }

    if (temp_streamObject->size == 0 && strcmp(_mode, "r") == 0)
    {
        perror("File is empty");
        return 0;
    }

    temp_streamObject->status = true;
    return temp_streamObject;
}

bool close_stream(StreamObject *_streamObject)
{
    int status = 1;

    if (_streamObject == NULL)
        fprintf(stdout, "Stream object is not exist");
    else if (_streamObject->status)
    {
        status = fclose(_streamObject->stream) ? 1 : 0;
        if (status == 0)
        {
            _streamObject->stream = NULL;
            _streamObject->path = NULL;
            _streamObject->size = 0;
            _streamObject->status = NULL;
            _streamObject->mode = NULL;

            free(_streamObject->stream);
            free(_streamObject->mode);
            free(_streamObject);
            //_streamObject = NULL;
        }
    }
    else
        fprintf(stdout, "File is already closed");

    return status;
}

void write_stream(StreamObject *_streamObject, char *const text_)
{
    fprintf(_streamObject->stream, "%s", text_);
}

char *read_stream(StreamObject *_streamObject)
{
    if (!_streamObject->status || _streamObject->size == 0)
        return NULL;

    char *m_destination = calloc(_streamObject->size, sizeof(char));

    char *buffer = (char *)malloc(0xff);
    while (fgets(buffer, 0xff, _streamObject->stream))
        strcat(m_destination, buffer);

    return m_destination;
}

char *argparse(int argc, char **argv)
{
    char *buffer = malloc(sizeof(char) * FILENAME_MAX);

    if (argc > 2)
    {
        perror("Too many argument");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        strcpy(buffer, argv[1]);
    }
    else
    {
        printf("Enter file path: ");
        scanf("%255[^\n]%*c", buffer);
    }
  
    return buffer;
}

/* UTILS */

/* TOKEN NODE */

typedef struct node
{
    char *word;
    int type;
    int colstart;
    int colend;
    int lineno;
    struct node *next;
} TokenNode;

TokenNode *new_token(void)
{
    TokenNode *temp = (TokenNode *)malloc(sizeof(TokenNode));

    temp->word = (char *)malloc(sizeof(char) + 1);
	memset(temp->word, 0, sizeof(char*));
    temp->type = 0;
    temp->colstart = 0;
    temp->colend = 0;
    temp->lineno = 0;
    temp->next = NULL;

    return temp;
}

void add_token(TokenNode *r, TokenNode *n)
{
    if (n != NULL)
    {
        TokenNode *temp = new_token();
        strcpy(temp->word, n->word);
        temp->type = n->type;
        temp->colstart = n->colstart;
        temp->colend = n->colend;
        temp->lineno = n->lineno;

        TokenNode *t = r;

        while (t->next)
            t = t->next;

        t->next = temp;
    }
}

/* TOKEN NODE */

/* TOKENIZER */

#define NAN         0
#define NUMBER      1
#define STRING      2
#define COMMA	      3
#define LSQB	      4
#define RSQB        5
#define COLON       6
#define MINUS       7
#define DOT         8
#define NEWLINE     9
#define ENDMARKER   10
#define WHITESPACE  11
#define OPCODE      12
#define REGISTER    13
#define LABEL       14
#define CONSTANT    15
#define OFFSET      16

#define ISEOF(x)            ((x) == ENDMARKER)
#define ISTERMINAL(x)       ((x) < OFFSET)
#define ISNONTERMINAL(x)    ((x) >= OFFSET)
#define ISWHITESPACE(x)     ((x) == WHITESPACE)
#define ISNEWLINE(x)        ((x) == NEWLINE)

static int
get_char_type(char curchr)
{
    if (isdigit(curchr))
        return NUMBER;
    else if (isalpha(curchr))
        return STRING;

    switch (curchr) {
    case ',':
        return COMMA;
    case '[':
        return LSQB;
    case ']':
        return RSQB;
    case ':':
        return COLON;
    case '-':
        return MINUS;
    case '\n':
    case '\r':
        return NEWLINE;
    case ' ':
        return WHITESPACE;
    }
    return NAN;
}

static void
next_state(char* source, int* c, char* curchr, int* curtype)
{
    *curchr = source[(*c)++];
    *curtype = get_char_type(*curchr);
}

static inline void
add_char(char* word, char curchr) { 
    int len = strlen(word);
    word[len] = curchr;
    word[len+1] = '\0';
}

static int
get_word_type(char* word)
{
    if (strlen(word) == 1 && !isalpha(word[0]))
        return get_char_type(word[0]);

    if (strcmp(word, "HRK") == 0
        || strcmp(word, "TOP") == 0
        || strcmp(word, "CIK") == 0
        || strcmp(word, "CRP") == 0
        || strcmp(word, "BOL") == 0
        || strcmp(word, "DEG") == 0
        || strcmp(word, "VE") == 0
        || strcmp(word, "VEYA") == 0
        || strcmp(word, "SS") == 0
        || strcmp(word, "SSD") == 0
        || strcmp(word, "SN") == 0
        || strcmp(word, "SP") == 0)
        return OPCODE;

    if (strcmp(word, "AX") == 0
        || strcmp(word, "BX") == 0
        || strcmp(word, "CX") == 0
        || strcmp(word, "DX") == 0)
        return REGISTER;

    int alpha_counter = 0;
    int digit_counter = 0;

    for (int i = 0; i < strlen(word); i++) 
    {
        if (word[i] == '-')
            digit_counter++;

        else if (isalpha(word[i]) != 0)
            alpha_counter++;

        else if (isdigit(word[i]) != 0)
            digit_counter++;
    }

    if (strlen(word) == digit_counter)
        return CONSTANT;

    if (isalpha(word[0]) && (alpha_counter + digit_counter == strlen(word)))
        return LABEL;

    return NAN;
}

TokenNode*
tokenize(char* source)
{
    int lineno = 0;
    int c = 0;
    char curchr;
    int curtype;
    TokenNode* root = new_token();
    TokenNode* tok = new_token();

    next_state(source, &c, &curchr, &curtype);
    while (c <= strlen(source)) {
        switch (curtype) {
        case STRING:
            do {
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                tok->colend++;
            } while (curtype == STRING || curtype == NUMBER);
            tok->colend--;
            break;

        case NUMBER:
        case MINUS:
            do {
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                tok->colend++;
            } while (curtype == NUMBER);
            tok->colend--;
            break;

        case WHITESPACE:
            next_state(source, &c, &curchr, &curtype);
            tok->colstart++;
            tok->colend++;
            continue;

        case NEWLINE:
            add_char(tok->word, curchr);
            next_state(source, &c, &curchr, &curtype);
            lineno++;
            break;

        case LSQB:
        case RSQB:
        case COLON:
        case COMMA:
        case NAN:
            add_char(tok->word, curchr);
            next_state(source, &c, &curchr, &curtype);
            break;

        default:
            add_char(tok->word, curchr);
            next_state(source, &c, &curchr, &curtype);
            break;

        }

        if (tok) {
            tok->lineno = lineno;
            tok->type = get_word_type(tok->word);
            add_token(root, tok);

            int buffer_colend = tok->colend;

            tok->next = new_token();
            tok = tok->next;
            tok->colstart = buffer_colend + 1;
            tok->colend = tok->colstart;
            tok->lineno = lineno;
            tok->type = 0;
        }
    }

    //free(tok->word);
    free(tok);
    TokenNode* end_token = new_token();
    end_token->type = ENDMARKER;
    add_token(root, end_token);

    root = root->next;
    return root;
}
/* TOKENIZER */

/* PARSER */
static TokenNode *s_token;
static char *src;

static void
next_token()
{
  if (s_token->next)
    s_token = s_token->next;
  else
  {
    s_token = NULL;
  }
}

static int
label_stmt()
{
  // DEC:          DEC   :
  //               token token->next
  if (s_token->type == LABEL)
  {
    if (s_token->next->type == COLON)
    {
      if (s_token->next->next->type == NEWLINE || s_token->next->next->type == ENDMARKER)
      {
        next_token(); // :
        next_token(); // \n , \0
        return 1;
      }
    }
  }

  return 0;
}

static int
variable_stmt()
{
  if (s_token->type == LABEL)
  {
    if (s_token->next->type == COLON)
    {
      if (s_token->next->next->type == NUMBER || s_token->next->next->type == CONSTANT)
      {
        next_token(); // :
        next_token(); // number
        next_token(); // \n
        return 1;
      }
    }
  }

  return 0;
}

static int
memory_stmt()
{
  if (s_token->type == LSQB)
  {
    if (s_token->next->type == LABEL)
    {

      if (s_token->next->next->type == RSQB)
      {
        next_token(); // label
        next_token(); // rsqb
        return 1;
      }
    }
  }

  return 0;
}

static int
field_stmt(void)
{
  if (ISTERMINAL(s_token->type))
  {
    if (label_stmt())
      return 1;
    else if (variable_stmt())
      return 1;

    if (s_token->type != OPCODE)
      return 0;

    if (strcmp(s_token->word, "DEG") == 0)
    {
      next_token();
      if (s_token->type == REGISTER);
      else if (s_token->type == LSQB)
      {
        if (!memory_stmt())
          return 0;
      }
      else if (s_token->type == NUMBER || s_token->type == CONSTANT);
      else
        return 0;
    }
    else if (strcmp(s_token->word, "SS") == 0 || strcmp(s_token->word, "SSD") == 0 || strcmp(s_token->word, "SN") == 0 || strcmp(s_token->word, "SP") == 0)
    {
      next_token();
      if (s_token->type != LABEL)
        return 0;
    }
    else
    {
      next_token();
      if (s_token->type != REGISTER)
        return 0;

      next_token();

      if (s_token->type == COMMA)
      {
        if (s_token->next->type == NUMBER || s_token->next->type == CONSTANT)
        {
          next_token();
        }
        else if (s_token->next->type == LSQB)
        {
          next_token();
          if (memory_stmt() == 0)
            return 0;
        }
        else if (s_token->next->type == REGISTER)
        {
          next_token();
        }
        else return 0;

      } 
      else
        return 0;
    }

    next_token();

    if (s_token->type == NEWLINE || s_token->type == ENDMARKER) // \n, \0
      return 1;
    else
      return 0;
  }

  return 0;
}

static void
program_stmt(void)
{
  while (s_token)
  {
    if (s_token->type == NEWLINE)
      next_token();
    else if (s_token->type == ENDMARKER)
      return;
    else if (field_stmt());
    else
    {
      perror(RED "Error " RESET);

      int row = 0;
      for (int i = 0; i < strlen(src); i++)
      {
        if (src[i] == '\n')
          row++;

        if (row == s_token->lineno)
          printf(BRED "%c" RESET, src[i]);
        else
          printf(BGRY "%c" RESET, src[i]);
      }
      exit(EXIT_FAILURE);
    }

    if (s_token && ISNONTERMINAL(s_token->type))
      exit(EXIT_FAILURE);
  }
}

void parse(TokenNode *tk, char *sr)
{
  s_token = tk;
  src = sr;
  program_stmt();
  printf(GRN "%s" RESET, sr);
}

/* PARSER */


/* ASSEMBLE */
#define INSTRUCTION_OFFSET 12

// MEMORY OFFSETS
static uint8_t CODE_SEGMENT_BEGIN = 0x01;
static uint8_t CODE_SEGMENT_END = 0xc8;

static uint8_t DATA_SEGMENT_BEGIN = 0xc9;
static uint8_t DATA_SEGMENT_END = 0xff;



struct adsym
{
    char *symbol;
    int8_t data;
    int8_t address;
};

static const int8_t
    reserved_instruction_table[INSTRUCTION_OFFSET][INSTRUCTION_OFFSET] = {
        {
            // HRK
            0x01,
            0x05,
            0x09,
            0x0D,
            0x02,
            0x06,
            0x0A,
            0x0E,
            0x03,
            0x07,
            0x0B,
            0x0F,
        },
        {
            // TOP
            0x11,
            0x15,
            0x19,
            0x1D,
            0x12,
            0x16,
            0x1A,
            0x1E,
            0x13,
            0x17,
            0x1B,
            0x1F,
        },
        {
            // CRP
            0x21,
            0x25,
            0x29,
            0x2D,
            0x22,
            0x26,
            0x2A,
            0x2E,
            0x23,
            0x27,
            0x2B,
            0x2F,
        },
        {
            // CIK
            0x31,
            0x35,
            0x39,
            0x3D,
            0x32,
            0x36,
            0x3A,
            0x3E,
            0x33,
            0x37,
            0x3B,
            0x3F,
        },
        {
            // BOL
            0x41,
            0x45,
            0x49,
            0x4D,
            0x42,
            0x46,
            0x4A,
            0x4E,
            0x43,
            0x47,
            0x4B,
            0x4F,
        },
        {
            // VE
            0x51,
            0x55,
            0x59,
            0x5D,
            0x52,
            0x56,
            0x5A,
            0x5E,
            0x53,
            0x57,
            0x5B,
            0x5F,
        },
        {
            // VEYA
            0x61,
            0x65,
            0x69,
            0x6D,
            0x62,
            0x66,
            0x6A,
            0x6E,
            0x63,
            0x67,
            0x6B,
            0x6F,
        },
        {
            // DEG
            0x71,
            0x72,
            0x73,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
        {
            // SS
            0x80,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
        {
            // SSD
            0x90,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
        {
            // SN
            0xA0,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
        {
            // SP
            0xB0,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
};

#define ADDRESS 0x20

static void error_repetition(char *str)
{
    printf(BRED "\nError: %s is repeated\n" RESET, str);
    exit(EXIT_FAILURE);
}

static void error_notintable(char *str)
{
    printf(BRED "\nError: %s is not in the address-symbol table\n" RESET, str);
    exit(EXIT_FAILURE);
}


static void error_notinreservedtable()
{
    printf(BRED "\nError: instruction is not in the reserved table\n" RESET);
    exit(EXIT_FAILURE);
}

static bool in_table(struct adsym *table[255], uint8_t size, char *ch)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(table[i]->symbol, ch) == 0)
            return true;
    }

    return false;
}

static struct adsym *get_from_table(struct adsym *table[255], uint8_t size, char *ch)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(table[i]->symbol, ch) == 0)
            return table[i];
    }

    return NULL;
}

static void add_table(struct adsym *table[255], uint8_t *offset, struct adsym *buffer)
{
    if (*offset < 256)
    {
        memcpy(&table[*offset], &buffer, sizeof(buffer));
        (*offset)++;
    }
}

static void _3block(TokenNode **tk, char *ins, char *addr)
{
    while ((*tk)->type != NEWLINE && (*tk)->type != ENDMARKER)
    {
        if (strcmp((*tk)->word, "HRK") == 0)
            strcpy(ins, "0000");
        else if (strcmp((*tk)->word, "TOP") == 0)
            strcpy(ins, "0001");
        else if (strcmp((*tk)->word, "CRP") == 0)
            strcpy(ins, "0010");
        else if (strcmp((*tk)->word, "CIK") == 0)
            strcpy(ins, "0011");
        else if (strcmp((*tk)->word, "BOL") == 0)
            strcpy(ins, "0100");
        else if (strcmp((*tk)->word, "VE") == 0)
            strcpy(ins, "0101");
        else if (strcmp((*tk)->word, "VEYA") == 0)
            strcpy(ins, "0110");
        else
            exit(EXIT_FAILURE);

        *tk = (*tk)->next;

        if (strcmp((*tk)->word, "AX") == 0)
            strcat(ins, "00");
        else if (strcmp((*tk)->word, "BX") == 0)
            strcat(ins, "01");
        else if (strcmp((*tk)->word, "CX") == 0)
            strcat(ins, "10");
        else if (strcmp((*tk)->word, "DX") == 0)
            strcat(ins, "11");

        *tk = (*tk)->next->next;

        if ((*tk)->type == LSQB)
        {
            *tk = (*tk)->next;
            strcat(ins, "10");
            uint8_t tmp = strtol((*tk)->word, NULL, 16);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next->next;
        }
        else if ((*tk)->type == REGISTER)
        {
            strcat(ins, "01");
            uint8_t tmp;
            if (strcmp((*tk)->word, "AX") == 0)
                tmp = 0x00;
            if (strcmp((*tk)->word, "BX") == 0)
                tmp = 0x01;
            if (strcmp((*tk)->word, "CX") == 0)
                tmp = 0x02;
            if (strcmp((*tk)->word, "DX") == 0)
                tmp = 0x03;

            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }

        else if ((*tk)->type == NUMBER || (*tk)->type == CONSTANT)
        {
            strcat(ins, "11");

            uint8_t tmp = strtol((*tk)->word, NULL, 10);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
    }
}

static void deg(TokenNode **tk, char *ins, char *addr)
{
    strcpy(ins, "0111");
    strcat(ins, "00");

    *tk = (*tk)->next;

    while ((*tk)->type != NEWLINE && (*tk)->type != ENDMARKER)
    {
        if ((*tk)->type == LSQB)
        {
            strcat(ins, "10");
            *tk = (*tk)->next;
            uint8_t tmp = strtol((*tk)->word, NULL, 16);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next->next;
        }
        else if ((*tk)->type == REGISTER)
        {
            strcat(ins, "01");
            uint8_t tmp;
            if (strcmp((*tk)->word, "AX") == 0)
                tmp = 0x00;
            if (strcmp((*tk)->word, "BX") == 0)
                tmp = 0x01;
            if (strcmp((*tk)->word, "CX") == 0)
                tmp = 0x02;
            if (strcmp((*tk)->word, "DX") == 0)
                tmp = 0x03;

            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
        else if ((*tk)->type == NUMBER || (*tk)->type == CONSTANT)
        {
            strcat(ins, "11");
            uint8_t tmp = strtol((*tk)->word, NULL, 10);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
    }
}

static void branch(TokenNode **tk, char *ins, char *addr)
{

    if (strcmp((*tk)->word, "SS") == 0)
        strcpy(ins, "1000");
    else if (strcmp((*tk)->word, "SSD") == 0)
        strcpy(ins, "1001");
    else if (strcmp((*tk)->word, "SN") == 0)
        strcpy(ins, "1010");
    else if (strcmp((*tk)->word, "SP") == 0)
        strcpy(ins, "1011");

    strcat(ins, "0000");

    *tk = (*tk)->next;

    if ((*tk)->type == ADDRESS)
    {
        uint8_t tmp = strtol((*tk)->word, NULL, 16);
        char *bin_tmp = BIN(tmp);
        strcpy(addr, bin_tmp);
        free(bin_tmp);
        *tk = (*tk)->next;
    }
}

char *assemble(TokenNode *tk)
{
    TokenNode *ftk = tk;
    TokenNode *fftk = ftk;
    uint8_t lc = DATA_SEGMENT_BEGIN;
    uint8_t pc = CODE_SEGMENT_BEGIN;
    uint8_t data_offset = 0;
    char *output_hex = "output.hex";
    StreamObject *sobj = open_stream(output_hex, "w");
    struct adsym *address_symbol_table[255];
    uint8_t c = 0;

    // ##################### FIRST PASS #######################
    while (ftk)
    {
        struct adsym *a1 = malloc(sizeof(struct adsym));

        if (ftk->type == LABEL)
        {
            // variable definition ( v1: 10 )
            if (ftk->next->type == COLON && ftk->next->next->type != NEWLINE)
            {
                // variable is not in the table
                if (!in_table(address_symbol_table, c, ftk->word))
                {
                    // v1
                    a1->symbol = ftk->word;

                    // v1, :, N
                    ftk = ftk->next->next;

                    // 1, 2, .. n
                    a1->data = atoi(ftk->word);

                    // M[DATA_SEGMENT_BEGIN]
                    a1->address = lc;
                    lc++;
                    data_offset++;

                    add_table(address_symbol_table, &c, a1);
                }
                // variable repetition
                else error_repetition(ftk->word);
            }
            // pointer call definition ( ex: HRK AX, [v1] )
            else if (ftk->next->type == RSQB)
            {
                // search 'ftk->word' in address-symbol table

                //printf("[%s]\n", ftk->word);
                if (!in_table(address_symbol_table, c, ftk->word))
                    error_notintable(ftk->word);
                ftk = ftk->next->next;
            }
            // label definition ( ex: LOP: )
            else if (ftk->next->type == COLON && (ftk->next->next->type == NEWLINE || ftk->next->next->type == ENDMARKER))
            {
                // search 'LABEL' in address-symbol table
                if (!in_table(address_symbol_table, c, ftk->word))
                {
                    a1->symbol = ftk->word;
                    a1->data = 0;
                    a1->address = pc;

                    add_table(address_symbol_table, &c, a1);
                }
                else error_repetition(ftk->word);

                ftk = ftk->next;
            }
            // label call definition ( ex: SS LOP )
            else if (ftk->next->type == NEWLINE || ftk->next->type == ENDMARKER);
        }
        else
        {
            free(a1);

            while (ftk->next && ftk->type != NEWLINE)
            {
                //printf("%s\t%d\n", ftk->word, pc);
                ftk = ftk->next;

                if (ftk->type == LABEL)
                {
                    struct adsym *tmp = get_from_table(address_symbol_table, c, ftk->word);

                    if (tmp)
                    {
                        memset(ftk->word, 0, sizeof(char) * 2);
                        sprintf(ftk->word, "%02X", h(tmp->address));
                        ftk->type = ADDRESS;
                    }
                }

                // one memory block has 8 bit therefore pc 2 increased.
                if (ftk->type == NEWLINE)
                    pc += 2;
            }
        }

        ftk = ftk->next;
    }

    /*
    printf("========ADDRESS SYMBOL TABLE==========\n");
    for (int i = 0; i < c; i++)
        printf("%s\t%d\t%02X\n", address_symbol_table[i]->symbol, address_symbol_table[i]->data, h(address_symbol_table[i]->address));
    printf("======================================\n");
    */

    // ################## SECOND PASS ##################
    ftk = fftk;
    while (ftk)
    {
        if (ftk->type == LABEL)
        {
            if (ftk->next->type == NEWLINE || ftk->next->type == ENDMARKER || ((ftk->next->type == RSQB && ftk->next->next->type == NEWLINE) || (ftk->next->type == RSQB && ftk->next->next->type == ENDMARKER)))
            {
                struct adsym *tmp = get_from_table(address_symbol_table, c, ftk->word);

                if (tmp)
                {
                    memset(ftk->word, 0, sizeof(char) * 2);
                    sprintf(ftk->word, "%02X", h(tmp->address));
                    ftk->type = ADDRESS;
                }
                else error_notintable(ftk->word);
            }
        }
        ftk = ftk->next;
    }

    // DATA SEGMENT
    for (uint8_t i = 0; i < data_offset; i++)
    {
        char ch[3];
        sprintf(ch, "%02X", h(address_symbol_table[i]->address));
        write_stream(sobj, ch);
        sprintf(ch, "%02X", h(address_symbol_table[i]->data));
        write_stream(sobj, ch);
        write_stream(sobj, "\n");
    }

    write_stream(sobj, "$\n");

    // CODE SEGMENT
    ftk = fftk;
    char *ins = malloc(9 * sizeof(char));
    char *addr = malloc(9 * sizeof(char));

    while (ftk)
    {
        if (ftk->type == LABEL)
        {
            if (ftk->next->type == COLON)
            {
                while (ftk->type != NEWLINE && ftk->type != ENDMARKER)
                    ftk = ftk->next;
            }
        }
        else
        {
            while (ftk->type != NEWLINE && ftk->type != ENDMARKER)
            {
                if (ftk->type == OPCODE)
                {
                    if (strcmp(ftk->word, "DEG") == 0)
                        deg(&ftk, ins, addr);
                    else if (strcmp(ftk->word, "SS") == 0 || strcmp(ftk->word, "SSD") == 0 || strcmp(ftk->word, "SN") == 0 || strcmp(ftk->word, "SP") == 0)
                        branch(&ftk, ins, addr);
                    else
                        _3block(&ftk, ins, addr);
                }

                if (ftk->type == NEWLINE || ftk->type == ENDMARKER)
                {
                    char ch[3];
                    uint8_t tmp1 = h(HEX(ins));
                    sprintf(ch, "%02X", tmp1);

                    uint8_t tmp2 = h(HEX(addr));

                    int i = ch[0];
                    switch (i)
                    {
                    case 'A':
                    case 'a':
                        i = 10;
                        break;
                    case 'B':
                    case 'b':
                        i = 11;
                        break;
                    default:
                        i -= '0';
                        break;
                    }
                    bool b = 0;

                    for (int j = 0; j < INSTRUCTION_OFFSET; j++)
                    {
                        uint8_t t = h(reserved_instruction_table[i][j]);
                        if (t == h(tmp1))
                            b = 1;
                    }

                    if (b == 0) error_notinreservedtable();

                    char w1[3];
                    sprintf(w1, "%02X", tmp1);
                    write_stream(sobj, w1);

                    sprintf(w1, "%02X", tmp2);
                    write_stream(sobj, w1);

                    write_stream(sobj, "\n");
                    memset(ins, 0, sizeof(char) * 9);
                    memset(addr, 0, sizeof(char) * 9);
                }
            }
        }

        ftk = ftk->next;
    }

    free(ins);
    free(addr);
    close_stream(sobj);
    return output_hex;
}

/* ASSEMBLE */

/* VM */


// OPCODES
#define HRK   0x00
#define TOP   0x01
#define CRP   0x02
#define CIK   0x03
#define BOL   0x04
#define VE    0x05
#define VEYA  0x06
#define DEG   0x07
#define SS    0x08
#define SSD   0x09
#define SN    0x0A
#define SP    0x0B

// REGISTERS
#define AX 0x0
#define BX 0x1
#define CX 0x2
#define DX 0x3

#define IR 0x4
#define AR 0x5
#define DR 0x6
#define TR 0x7
#define PC 0x8

// ADDRESSING MODE
#define MB 0x0
#define MR 0x1
#define MM 0x2
#define MI 0x3

// MEMORY PROPERTY
#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8

// (8 bit, MSB sign bit, max = 127, min = -128)

// REGISTERS
static int8_t RAX = 0x00;
static int8_t RBX = 0x00;
static int8_t RCX = 0x00;
static int8_t RDX = 0x00;
static uint8_t RIR = 0x00;
static uint8_t RAR = 0x00;
static uint8_t RDR = 0x00;
static int8_t RTR = 0x00;
static uint8_t RPC = 0x01;

// MEMORY
static char MEMORY[MEMORY_SIZE][INSTRUCTION_SIZE + 1];

// ########################### GENERAL UTILS ###############################

// ########################### OPCODES #############################
static int8_t not(int8_t d1)
{
    return h(~d1);
}

static inline int8_t sum(int8_t s1, int8_t s2)
{
    return h(s1 + s2);
}

static inline int8_t mul(int8_t s1, int8_t s2)
{
    return h(s1 * s2);
}

static inline int8_t sub(int8_t s1, int8_t s2)
{
    return h(s1 - s2);
}

static inline int8_t divi(int8_t s1, int8_t s2)
{
    if (s2 == 0)
    {
        printf(BRED "\nError: Division by zero\n"RESET);
        exit(EXIT_FAILURE);
    }

    return h(s1 / s2);
}

// ##################### MEMORY | REGISTER DUMP ######################
static void MEMDUMP()
{
    int i, j = 1;

	printf(RED "\n--------------------------------------------Memory--------\n" RESET);
    printf(WHITE "00\t%s\n" RESET, MEMORY[0]);

    for (i = 1; i <= CODE_SEGMENT_END; i += 2)
    {
        printf("%02x\t", i);
        if (RPC == i)
#ifdef _WIN32
            printf("[ %s %s ]\n", MEMORY[j], MEMORY[j + 1]);
#else
            printf(GRN "%s %s\n" RESET, MEMORY[j], MEMORY[j + 1]);
#endif
        else
            printf("%s %s\n", MEMORY[j], MEMORY[j + 1]);

        j += 2;
    }
}

static void REGDUMP()
{
    char *bin_rax = BIN(RAX);
    char *bin_rbx = BIN(RBX);
    char *bin_rcx = BIN(RCX);
    char *bin_rdx = BIN(RDX);
    char *bin_rir = BIN(RIR);
    char *bin_rar = BIN(RAR);
    char *bin_rdr = BIN(RDR);
    char *bin_rtr = BIN(RTR);
    char *bin_rpc = BIN(RPC);

	printf(BLUE "\n------------------------------------------Registers-------\n" RESET);
    printf("AX\t%s\t%02X\t%d\n" RESET, bin_rax, h(RAX), RAX);
    printf(WHITE "BX\t%s\t%02X\t%d\n" RESET, bin_rbx, h(RBX), RBX);
    printf("CX\t%s\t%02X\t%d\n", bin_rcx, h(RCX), RCX);
    printf(WHITE "DX\t%s\t%02X\t%d\n\n" RESET, bin_rdx, h(RDX), RDX);
    printf("IR\t%s\t%02X\t%d\n", bin_rir, h(RIR), RIR);
    printf(WHITE "AR\t%s\t%02X\t%d\n" RESET, bin_rar, h(RAR), RAR);
    printf("DR\t%s\t%02X\t%d\n", bin_rdr, h(RDR), RDR);
    printf(WHITE "TR\t%s\t%02X\t%d\n\n" RESET, bin_rtr, h(RTR), RTR);
    printf("PC\t%s\t%02X\t%d\n", bin_rpc, h(RPC), RPC);

    free(bin_rax);
    free(bin_rbx);
    free(bin_rcx);
    free(bin_rdx);
    free(bin_rir);
    free(bin_rar);
    free(bin_rdr);
    free(bin_rtr);
    free(bin_rpc);
}

// #################### FETCH & DECODE & EXECUTE ###################
static void ftdcex()
{
    /*********** FETCH ***********/
    // opcode, register, mode
    RAR = RPC;
    RIR = HEX(MEMORY[RAR]);
    RPC++;

    // address
    RAR = RPC;
    RAR = HEX(MEMORY[RAR]);
    RPC++;

    /*********** DECODE ***********/
    char *ir = BIN(RIR);

    char *opcode_b = malloc(5);
    char *reg_b = malloc(3);
    char *mode_b = malloc(3);

    int8_t opcode;
    int8_t reg;
    int8_t mode;

    opcode_b[0] = ir[0];
    opcode_b[1] = ir[1];
    opcode_b[2] = ir[2];
    opcode_b[3] = ir[3];
    opcode_b[4] = '\0';
    opcode = HEX(opcode_b); //strtol(opcode, NULL, 2);

    reg_b[0] = ir[4];
    reg_b[1] = ir[5];
    reg_b[2] = '\0';
    reg = HEX(reg_b); //strtol(reg, NULL, 2);

    mode_b[0] = ir[6];
    mode_b[1] = ir[7];
    mode_b[2] = '\0';
    mode = HEX(mode_b); //strtol(mode, NULL, 2);

    free(ir);
    free(opcode_b);
    free(reg_b);
    free(mode_b);

    switch (mode)
    {
    case MR:
        switch (RAR)
        {
        case AX:
            RDR = RAX;
            break;
        case BX:
            RDR = RBX;
            break;
        case CX:
            RDR = RCX;
            break;
        case DX:
            RDR = RDX;
            break;
        }
        break;
    case MI:
    case MB:
        RDR = RAR;
        break;
    case MM:
        RDR = HEX(MEMORY[RAR]);
        break;
    }

    /*********** EXECUTE ***********/
    char *bin_rdr;
    switch (opcode)
    {
    case HRK:
        switch (reg)
        {
        case AX:
            RAX = RDR;
            break;
        case BX:
            RBX = RDR;
            break;
        case CX:
            RCX = RDR;
            break;
        case DX:
            RDX = RDR;
            break;
        }
        break;
    case TOP:
        switch (reg)
        {
        case AX:
            RAX = sum(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = sum(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = sum(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = sum(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case CRP:
        switch (reg)
        {
        case AX:
            RAX = mul(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = mul(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = mul(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = mul(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case CIK:
        switch (reg)
        {
        case AX:
            RAX = sub(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = sub(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = sub(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = sub(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case BOL:
        switch (reg)
        {
        case AX:
            RAX = divi(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = divi(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = divi(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = divi(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case VE:
        switch (reg)
        {
        case AX:
            RAX = RAX & RDR;
            RTR = RAX;
            break;
        case BX:
            RBX = RBX & RDR;
            RTR = RBX;
            break;
        case CX:
            RCX = RCX & RDR;
            RTR = RCX;
            break;
        case DX:
            RDX = RDX & RDR;
            RTR = RDX;
            break;
        }
        break;
    case VEYA:
        switch (reg)
        {
        case AX:
            RAX = RAX | RDR;
            RTR = RAX;
            break;
        case BX:
            RBX = RBX | RDR;
            RTR = RBX;
            break;
        case CX:
            RCX = RCX | RDR;
            RTR = RCX;
            break;
        case DX:
            RDX = RDX | RDR;
            RTR = RDX;
            break;
        }
        break;
    case DEG:
        switch (mode)
        {
        case MR:
            switch (RAR)
            {
            case AX:
                RAX = not(RDR);
                break;
            case BX:
                RBX = not(RDR);
                break;
            case CX:
                RCX = not(RDR);
                break;
            case DX:
                RDX = not(RDR);
                break;
            }
            break;
        case MM:
            bin_rdr = BIN(not(RDR));
            strcpy(MEMORY[RAR], bin_rdr);
            free(bin_rdr);
            break;
        case MI:
            RDR = not(RDR);
            break;
        default:
            exit(EXIT_FAILURE);
        }
        break;
    case SS:
        if (RTR == 0)
            RPC = RDR;
        break;
    case SSD:
        if (RTR != 0)
            RPC = RDR;
        break;
    case SN:
        if (RTR < 0)
            RPC = RDR;
        break;
    case SP:
        if (RTR > 0)
            RPC = RDR;
        break;
    }
}

// ######################### MANAGER ###############################
static void init()
{
    RAX = h(0x00);
    RBX = h(0x00);
    RCX = h(0x00);
    RDX = h(0x00);
    RIR = h(0x00);
    RAR = h(0x00);
    RDR = h(0x00);
    RTR = h(0x00);

    CODE_SEGMENT_END = RPC - 1;

    RPC = h(0x01);
}

static void run()
{
    while (RPC < CODE_SEGMENT_END)
    {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
        MEMDUMP();
        ftdcex();
        REGDUMP();
        getc(stdin);
    }
}

void load_program(char *executable)
{
    StreamObject *streamObject = open_stream(executable, "r");
    if (streamObject == NULL)
    {
        printf(BRED"\nError: executable file is null.\n"RESET);
        exit(EXIT_FAILURE);
    }

    char buffer[3];
    int8_t hex;
    char *temp;

    int data_end_flag = 0;

    while (fgets(buffer, sizeof(buffer), streamObject->stream))
    {
        if (*buffer == '$')
            data_end_flag = 1;

        if (*buffer != '\n' && *buffer != '$')
        {
            if (data_end_flag == 0)
            {
                uint8_t address = strtol(buffer, NULL, 16);
                fgets(buffer, sizeof(buffer), streamObject->stream);
                hex = h(strtol(buffer, NULL, 16));
                temp = BIN(hex);
                strcpy(MEMORY[address], temp);
            }
            else
            {
                hex = h(strtol(buffer, NULL, 16));
                temp = BIN(hex);
                strcpy(MEMORY[RPC++], temp);
            }
            free(temp);
        }
    }

    init();
    run();
}

/* VM */



int main(int argc, char **argv)
{
    char *m_file_path = argparse(argc, argv);

    StreamObject *streamObject = open_stream(m_file_path, "r");
    if (streamObject == NULL)
        exit(EXIT_FAILURE);

    char *f_source = read_stream(streamObject);

    TokenNode *tk = tokenize(f_source);

    if (tk == NULL)
        perror("error: tokenize failed");
    else
    {
        TokenNode *tk_iter = tk;

        printf("\n");
        parse(tk_iter, f_source);
        char *executable_file = assemble(tk_iter);

        if (executable_file)
        {
            printf("\nPress enter to run the code...");
            getc(stdin);
            load_program(executable_file);
            printf("Done");
        }
    }

    free(tk);
    free(m_file_path);
    free(f_source);
    close_stream(streamObject);
	
#ifdef _WIN32
	printf("\nPress enter to exit...");
	getc(stdin);
#endif

    return 0;
}
