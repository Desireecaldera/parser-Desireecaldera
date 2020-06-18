#ifndef __PARSE_H
#define __PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include "scan.h"

typedef enum
{
    PROGRAM_NODE,
    STATEMENT_NODE,
    ASSIGN_STMT_NODE,
    REPEAT_STMT_NODE,
    PRINT_STMT_NODE,
    EXPR_NODE,
    TERM_NODE,
    FACTOR_NODE,
    IDENT_NODE,
    NUMBER_NODE
} NODE_TYPE;

typedef enum
{
    INT_TYPE,
    FLOAT_TYPE
} NUMBER_TYPE;

typedef union
{
    long integral;
    double floating_point;
} NUMBER_VALUE;

typedef struct
{
    NUMBER_TYPE type;
    NUMBER_VALUE value;
} NUMBER;

typedef struct node
{
    NODE_TYPE type;
    union
    {
        char *identifier;
        NUMBER number;
        char op;
    } data;
    struct node *leftChild;
    struct node *rightChild;
} NODE;

TOKEN *getNextToken(TOKEN **currToken);

NODE *program();
NODE *statement();
NODE *assignStmt(TOKEN **currToken);
NODE *repeatStmt(TOKEN **currToken);
NODE *printStmt(TOKEN **currToken);
NODE *expr(TOKEN **currToken);
NODE *term(TOKEN **currToken);
NODE *factor(TOKEN **currToken);
NODE *ident(TOKEN **currToken);
NODE *number(TOKEN **currToken);

void freeParseTree(NODE **node);

void error(char *errorFormat, ...);

#endif