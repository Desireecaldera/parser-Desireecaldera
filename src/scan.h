#ifndef __SCAN_H
#define __SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    NO_TOKEN_TYPE,    // 0
    INVALID_TOKEN,    // 1
    REPEAT_TOKEN,     // 2
    PRINT_TOKEN,      // 3
    IDENT_TOKEN,      // 4
    INT_TOKEN,        // 5
    FLOAT_TOKEN,      // 6
    ASSIGNMENT_TOKEN, // 7
    LPAREN_TOKEN,     // 8
    RPAREN_TOKEN,     // 9
    ADD_OP_TOKEN,     // 10
    MULT_OP_TOKEN,    // 11
    SEMICOLON_TOKEN,  // 12
    EOF_TOKEN         // 13
} TOKEN_TYPE;

typedef union
{
    long integral;
    double floating_point;
    char *string;
    char op;
} TOKEN_VALUE;

typedef struct token
{
    TOKEN_TYPE type;
    TOKEN_VALUE val;
} TOKEN;

TOKEN *scanner();
void freeToken(TOKEN **);
void printToken(TOKEN **);
void ungetToken(TOKEN **);

#define BUF_SIZE 128

#endif