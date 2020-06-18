#include "eval.h"
#include <math.h>

#define NAN_NUMBER (NUMBER){FLOAT_TYPE, .value.floating_point = NAN}
#define DEFAULT_NUMBER (NUMBER){INT_TYPE, 0}


SYMBOL_TABLE_NODE *symbolTable = NULL;

void evalProgram(NODE *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->leftChild != NULL)
    {
        evalStatement(node->leftChild);
    }

    if (node->rightChild != NULL)
    {
        evalProgram(node->rightChild);
    }
}

void evalStatement(NODE *node)
{
    if (node->leftChild != NULL)
    {
        switch (node->leftChild->type)
        {
            case ASSIGN_STMT_NODE:
                evalAssignStmt(node->leftChild);
                break;
            case REPEAT_STMT_NODE:
                evalRepeatStmt(node->leftChild);
                break;
            case PRINT_STMT_NODE:
                evalPrintStmt(node->leftChild);
                break;
            default:
                error("Invalid child of type %d in statement node.", node->leftChild->type);
        }
    }
}

void evalAssignStmt(NODE *node)
{
    // TODO
}

void evalRepeatStmt(NODE *node)
{
    // TODO
}

void evalPrintStmt(NODE *node)
{
    // TODO
}

NUMBER evalExpr(NODE *node)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalTerm(NODE *node)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalFactor(NODE *node)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalNumber(NODE *node)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalId(NODE *node)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalAdd(NUMBER op1, NUMBER op2)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalSub(NUMBER op1, NUMBER op2)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalMult(NUMBER op1, NUMBER op2)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalDiv(NUMBER op1, NUMBER op2)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalMod(NUMBER op1, NUMBER op2)
{
    // TODO
    return DEFAULT_NUMBER;
}

NUMBER evalOperation(NUMBER operand1, NUMBER operand2, char op)
{
    switch (op)
    {
        case '+':
            return evalAdd(operand1, operand2);
        case '-':
            return evalSub(operand1, operand2);
        case '*':
            return evalMult(operand1, operand2);
        case '/':
            return evalDiv(operand1, operand2);
        case '%':
            return evalMod(operand1, operand2);
        default:
            error("Invalid operation character %c.", op);
    }
    return DEFAULT_NUMBER; // unreachable
}

SYMBOL_TABLE_NODE *findSymbol(SYMBOL_TABLE_NODE *table, char *ident)
{
    while (table != NULL)
    {
        if (strcmp(ident, table->ident) == 0)
        {
            return table;
        }
        table = table->next;
    }
    return NULL;
}

SYMBOL_TABLE_NODE *createSymbol(char *ident, NUMBER value)
{
    SYMBOL_TABLE_NODE *newSymbol = calloc(sizeof(SYMBOL_TABLE_NODE), 1);
    newSymbol->ident = calloc(sizeof(char), strlen(ident)+1);
    strcpy(newSymbol->ident, ident);
    newSymbol->value = value;
    return newSymbol;
}

void reassignSymbol(SYMBOL_TABLE_NODE* symbol, NUMBER value)
{
    symbol->value = value;
}

void addSymbolToTable(SYMBOL_TABLE_NODE **table, SYMBOL_TABLE_NODE *newSymbol)
{
    newSymbol->next = *table;
    *table = newSymbol;
}

NUMBER evalSymbol(SYMBOL_TABLE_NODE *symbol)
{
    return symbol->value;
}

void freeSymbolTable(SYMBOL_TABLE_NODE **table)
{
    if (table == NULL)
    {
        return;
    }
    if (*table == NULL)
    {
        return;
    }

    freeSymbolTable(&(*table)->next);
    free((*table)->ident);
    free(*table);
    *table = NULL;
}

void cleanUpSymbolTables()
{
    freeSymbolTable(&symbolTable);
}
