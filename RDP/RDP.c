#include <stdio.h>

#define LETTER 0
#define DIGIT 1
#define FLOAT_SEPARATOR 2
#define INVALID 98
#define UNKNOWN 99


#define INT_LIT 10
#define FLOAT_LIT 11
#define IDENT 12
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int nextToken;
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int invalidToken = 0;
FILE *in_fp, *fopen();

void expr();
void term();
void factor();
void error();
int lex();

/* getChar - a function to get the next character of
 input and determine its character class */
void getChar() {
    int prevCharClass;
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else if(nextChar == '.')
            charClass = FLOAT_SEPARATOR;
        else 
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}


main() {
/* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
            expr();
        } while (nextToken != EOF);
    }
}


/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */
void expr() {
    printf("Enter <expr>\n");
    /* Parse the first term */
    term();
    /* As long as the next token is + or -, get
    the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
}

/* term
Parses strings in the language generated by the rule:
<term> -> <factor> {(* | /) <factor>)
*/
void term() {
    printf("Enter <term>\n");
    /* Parse the first factor */
    factor();
    /* As long as the next token is * or /, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
} 

/* factor
Parses strings in the language generated by the rule:
<factor> -> id | int_constant | ( <expr )
*/
void factor() {
    printf("Enter <factor>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT || nextToken == FLOAT_LIT)
        /* Get the next token */
        lex();
        /* If the RHS is ( <expr>), call lex to pass over the
        left parenthesis, call expr, and check for the right
        parenthesis */
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
            else
                error();
        } else
            error();
    } /* End of else */
    printf("Exit <factor>\n");;
} 

void error(){
    printf("Error\n");
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}


void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
        }
    return nextToken;
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            nextToken = INT_LIT;
            while (charClass == DIGIT || charClass ==  FLOAT_SEPARATOR) {
                addChar();
                getChar();
                if (charClass == FLOAT_SEPARATOR){
                    nextToken = FLOAT_LIT;
                }
                while(charClass == LETTER){
                    invalidToken = 1;
                    addChar();
                    getChar();
                }
            }
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } 

    if(invalidToken){
        printf("Next token is: %d, Next lexeme is %s\n",
    INVALID, lexeme);
                error();

        invalidToken = 0;
    }else{
        printf("Next token is: %d, Next lexeme is %s\n",
    nextToken, lexeme);
    }
    return nextToken;
} 
