#ifndef __EVAL_H
#define __EVAL_H

#include "parse.h"

typedef struct symbol_table_node
{
    char *ident;
    NUMBER value;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

SYMBOL_TABLE_NODE *findSymbol(SYMBOL_TABLE_NODE *table, char *ident);
SYMBOL_TABLE_NODE *createSymbol(char *ident, NUMBER value);
void reassignSymbol(SYMBOL_TABLE_NODE* symbol, NUMBER value);
void addSymbolToTable(SYMBOL_TABLE_NODE **table, SYMBOL_TABLE_NODE *newSymbol);
NUMBER evalSymbol(SYMBOL_TABLE_NODE *symbol);
void cleanUpSymbolTables();

void evalProgram(NODE *node);
void evalStatement(NODE *node);
void evalAssignStmt(NODE *node);
void evalRepeatStmt(NODE *node);
void evalPrintStmt(NODE *node);
NUMBER evalExpr(NODE *node);
NUMBER evalTerm(NODE *node);
NUMBER evalFactor(NODE *node);
NUMBER evalNumber(NODE *node);
NUMBER evalId(NODE *node);
NUMBER evalOperation(NUMBER operand1, NUMBER operand2, char op);

#endif //PARSER_EVAL_H
