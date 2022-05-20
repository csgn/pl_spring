#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	CHUNK	1024



enum CharType {
	NONE,
	NUMBER,
	TEXT,
	OTHER
};

enum TokenType {
	COMMA,
	LSQB,
	RSQB,
	NEWLINE,
	WHITESPACE,
	ENDMARKER,
	OPCODE,
	REGISTER,
	IMMEDIATE
};

enum Opcodes {
    HRK,
    TOP,
    CRP,
    CIK,
    BOL,
    VE,
    VEYA,
    DEG,
};

enum Registers {
    AX,
    BX,
    CX,
    DX
};

enum SourceTypes {
    VM_REGISTER,
    VM_DIRECT,
    VM_INDIRECT,
    VM_IMMEDIATE
};


#define ISOPCODE(c)            \
    (  strcmp("HRK",  c) == 0  \
    || strcmp("TOP",  c) == 0  \
    || strcmp("CRP",  c) == 0  \
    || strcmp("CIK",  c) == 0  \
    || strcmp("BOL",  c) == 0  \
    || strcmp("VE",   c) == 0  \
    || strcmp("VEYA", c) == 0  \
    || strcmp("DEG",  c) == 0  )

#define WHICHOPCODE(c)                            \
    ( strcmp("HRK",  c) == 0 ? HRK  :             \
    ( strcmp("TOP",  c) == 0 ? TOP  :             \
    ( strcmp("CRP",  c) == 0 ? CRP  :             \
    ( strcmp("CIK",  c) == 0 ? CIK  :             \
    ( strcmp("BOL",  c) == 0 ? BOL  :             \
    ( strcmp("VE",   c) == 0 ? VE   :             \
    ( strcmp("VEYA", c) == 0 ? VEYA :             \
    ( strcmp("DEG",  c) == 0 ? DEG  : NONE ))))))))

#define ISREGISTER(c)       \
    (  strcmp("AX", c) == 0 \
    || strcmp("BX", c) == 0 \
    || strcmp("CX", c) == 0 \
    || strcmp("DX", c) == 0 )

#define WHICHREGISTER(c)                 \
    ( strcmp("AX", c) == 0 ? AX :        \
    ( strcmp("BX", c) == 0 ? BX :        \
    ( strcmp("CX", c) == 0 ? CX :        \
    ( strcmp("DX", c) == 0 ? DX : NONE))))

#define ISSYMBOL(c) \
    ( strcmp(c, ",")    == 0   ? COMMA         :            \
    ( strcmp(c, " ")    == 0   ? WHITESPACE    :            \
    ( strcmp(c, "\n")   == 0   ? NEWLINE       :            \
    ( strcmp(c, "\r")   == 0   ? NEWLINE       :            \
    ( strcmp(c, "\t")   == 0   ? WHITESPACE    :            \
    ( strcmp(c, "\0")   == 0   ? ENDMARKER     :            \
    ( strcmp(c, "[")    == 0   ? LSQB          :            \
    ( strcmp(c, "]")    == 0   ? RSQB          : NONE))))))))

struct Buffer {
	int 	    size;
	char* 		text;
};

struct Token {
	char* 		    text;
	enum TokenType 	type;
	struct Token* 	next;
};

struct BLOCK {
    enum Opcodes        opcode;
    int                 src; 
    enum SourceTypes    srcType; 
    int                 dest; 
    enum SourceTypes    destType;
    struct BLOCK*       next;
};


enum TokenType GetType(char*            text,
		               enum CharType    charType) {
    switch (charType) {
    case NUMBER:
        return IMMEDIATE;
    case TEXT:
        if (ISOPCODE(text))
            return OPCODE;
        if (ISREGISTER(text))
            return REGISTER;
        break;
    case OTHER:
        return ISSYMBOL(text);
    }

    return NONE;
}

void AppendChar(char* text, char c) {
    int len = strlen(text);
    text[len] = c;
    text[len+1] = '\0';
}

void NextChar(struct Buffer* 	buffer,
	          int* 	i,
	          enum CharType* 	type) {
    (*i)++;
	char ch = buffer->text[*i];

	if (ch > 47 && ch < 58) { // number
		*type = NUMBER;
	} else if ( (ch > 64 && ch < 91) 
		 || (ch > 96 && ch < 123) ) { // letters
		*type = TEXT;
	} else { // comma, newline, tab, space, endofline, square brackets
		switch (ch) {
		case ',':
		case '\n':
		case '\t':
		case ' ':
		case '\0':
		case '[':
		case ']':
			*type = OTHER;
			break;
		default:
			*type = NONE;
			break;
		}
	}

}

struct Token* Tokenize(struct Buffer* buffer) {
	if (buffer == NULL) {
		printf("source is not exists");
		return NULL;
	}

	int i, j;
	enum CharType currCharType;
	enum CharType charType;
	char chr;
	char *tokenText;
	
	struct Token* root = (struct Token*) malloc(sizeof(struct Token));
	struct Token* iter = root;

	i = -1;
	charType = NONE;
    tokenText = (char*) malloc(sizeof(char)*256);
    memset(tokenText, 0, sizeof(tokenText));
	NextChar(buffer, &i, &charType);

	while (i <= buffer->size) {
		switch (charType) {
			case NUMBER:
				do {
                    AppendChar(tokenText, buffer->text[i]);
                    currCharType = charType;
					NextChar(buffer, &i, &charType);
				} while (charType == NUMBER);
				break;
			case TEXT:
				do {
                    AppendChar(tokenText, buffer->text[i]);
                    currCharType = charType;
					NextChar(buffer, &i, &charType);
				} while (charType == TEXT);
				break;
			case OTHER:
                AppendChar(tokenText, buffer->text[i]);
                currCharType = charType;
				NextChar(buffer, &i, &charType);
				break;
		}

        iter->text = (char*) malloc(sizeof(char)*strlen(tokenText));
        strcpy(iter->text, tokenText);
        iter->type = GetType(tokenText, currCharType);
        
        if (i <= buffer->size) {
            iter->next = (struct Token*) malloc(sizeof(struct Token));
            iter = iter->next;
        }

        memset(tokenText, 0, sizeof(tokenText) + 1);
	}

	return root;
}

struct Buffer* ReadFile(char* fp) {
	if (fp == NULL) {
		printf("ERROR: %s is not exists", fp);
		return NULL;
	}

	int c;
	struct Buffer* buffer;
	FILE* stream = fopen(fp, "r");

	buffer = (struct Buffer*) malloc(sizeof(struct Buffer));
	buffer->text = (char*) malloc(sizeof(char)*CHUNK);
	buffer->size = 0;

	while ((c = fgetc(stream)) != EOF) {
		buffer->text[buffer->size++] = (char) c;
	}

	if (buffer->size == 0) {
		printf("ERROR: %s is empty", fp);
		free(buffer->text);
		free(buffer);
		return NULL;
	}
	
	buffer->text[buffer->size] = '\0';
	
	return buffer;
}

struct BLOCK* GenerateBlocks(struct Token* t) {
    struct BLOCK* root = (struct BLOCK*) malloc(sizeof(struct BLOCK));
    struct BLOCK* iter = root;

    while (t->type != ENDMARKER) {
        if (t->type == OPCODE) {
            iter->opcode = WHICHOPCODE(t->text);
            t = t->next;
        } 
		/*
		else {
            printf("ERROR: expected an OPCODE\n");
            return NULL;
        }*/

        while (t->type == WHITESPACE) t = t->next;

        if (t->type == REGISTER) {
            iter->dest = WHICHREGISTER(t->text);
            iter->destType = VM_REGISTER;
            t = t->next;
        } else if (t->type == LSQB) {
            if (t->next->type == LSQB) { // indirect 
                t = t->next->next; // value
                iter->dest = atoi(t->text);
                iter->destType = VM_INDIRECT;

                if (t->next->type == RSQB && t->next->next->type == RSQB) t = t->next->next->next;
                	/*
                else {
                    printf("ERROR: expected ']'\n");
                    return NULL;
                }*/
            } else { // direct
                t = t->next;

                if (t->type == IMMEDIATE) {
                   iter->dest = atoi(t->text);
                   iter->destType = VM_DIRECT;
                   t = t->next;
               }

               if (t->type == RSQB) t = t->next;
               	/*
               else {
                    printf("ERROR: expected ']'\n");
                    return NULL;
               }*/
            }
        } else if (t->type == IMMEDIATE) {
                iter->src = atoi(t->text);
                iter->srcType = VM_IMMEDIATE;
                t = t->next;
        } 
		/*
		else {
            printf("ERROR: expected REGISTER, MEMORY or IMMEDIATE\n");
            return NULL;
        }	*/
        


        if (t->type != NEWLINE && t->type != ENDMARKER) {
            if (t->type == COMMA) {
                 t = t->next;
            }
				/*
			 else {
                printf("ERROR: expected ','\n");
                return NULL;
            }*/

            while (t->type == WHITESPACE) t = t->next;

            if (t->type == REGISTER) {
                iter->src = WHICHREGISTER(t->text);
                iter->srcType = VM_REGISTER;
                t = t->next;
            } else if (t->type == LSQB) {
                if (t->next->type == LSQB) { // indirect 
                    t = t->next->next; // value
                    iter->src = atoi(t->text);
                    iter->srcType = VM_INDIRECT;

                    if (t->next->type == RSQB && t->next->next->type == RSQB) t = t->next->next->next;
                    	/*
                    else {
                        printf("ERROR: expected ']'\n");
                        return NULL;
                    }*/
                }
				
				else { // direct
                    t = t->next;

                    if (t->type == IMMEDIATE) {
                        iter->src = atoi(t->text);
                        iter->srcType = VM_DIRECT;
                        t = t->next;
                    }

                    if (t->type == RSQB) t = t->next;
                    	/*
                    else { 
                        printf("ERROR: expected ']'\n");
                        return NULL;
                    }*/
                }
                
            } else if (t->type == IMMEDIATE) {
                iter->src = atoi(t->text);
                iter->srcType = VM_IMMEDIATE;

                t = t->next;
            } 
				/*
			else {
                printf("ERROR: expected REGISTER, MEMORY or IMMEDIATE\n");
                return NULL;
            }*/
        }

        if (t->type == NEWLINE) {
            t = t->next;
            iter->next = (struct BLOCK*) malloc(sizeof(struct BLOCK));
            iter = iter->next;
        } 
			/*
		else {
            printf("ERROR: expected a NEWLINE \n");
            return NULL;
        }*/
    }

    //free(iter);

    return root;
}


int StartVM(struct BLOCK* block) {
    int REGISTERS[4];
    int MEMORY[500];
    
    int i = 0;
    for (i = 0; i < 500; i++) {
    	MEMORY[i] = 0;
	}
	
	for (i = 0; i < 4; i++) {
		REGISTERS[i] = 0;
	}

    int* src;
    int* dest;

    printf("AX\tBX\tCX\tDX\n");
    while (block) {
    	//printf("OPCODE: %d\tSRC: %d\tSRC TYPE: %d\tDEST: %d\tDEST TYPE: %d\n", block->opcode, block->src, block->srcType, block->dest, block->destType);
    	

        switch (block->srcType) {
        case VM_REGISTER:
            src = &REGISTERS[block->src];
            break;
        case VM_DIRECT:
            src = &MEMORY[block->src];
            break;
        case VM_INDIRECT:
        	
            src = &MEMORY[MEMORY[block->src]];
            break;
        case VM_IMMEDIATE:
        	
            src = &block->src;
            break;
        }

        switch (block->destType) {
        case VM_REGISTER:
            dest = &REGISTERS[block->dest];
            break;
        case VM_DIRECT:
            dest = &MEMORY[block->dest];
            break;
        case VM_INDIRECT:
            dest = &MEMORY[MEMORY[block->dest]];
            break;
        case VM_IMMEDIATE:
            dest = &block->dest;
            break;
        }
    

        switch (block->opcode)
        {
        case HRK:
            *dest = *src;
            break;
        case TOP:
            *dest = (*dest) + (*src);
            break;
        case CRP:
            *dest = (*dest) * (*src);
            break;
        case CIK:
            *dest = (*dest) - (*src);
            break;
        case BOL:
            REGISTERS[3] = (*dest) % (*src);
            *dest = (*dest) / (*src);
            break;
        case VE:
            *dest = (*dest) & (*src);
            break;
        case VEYA:
            *dest = (*dest) | (*src);
            break;
        case DEG:
            *dest = ~(*dest);
            break;
        }
		printf("%d\t%d\t%d\t%d\n", REGISTERS[0], REGISTERS[1], REGISTERS[2], REGISTERS[3]);
       
        block = block->next;
    }

}

int main(int argc, char** argv) {
	char* fp = (char*) malloc(sizeof(char)*255);

	if (argc == 2 && argv[1] != NULL) {
		fp = argv[1];
	} else {
		printf("enter a file: ");
		scanf("%s", fp);
	}
	
	struct Buffer* buffer = ReadFile(fp);
    struct Token* token = Tokenize(buffer);
    struct BLOCK* block = GenerateBlocks(token);
    
   
    StartVM(block);

	return 0;
}
