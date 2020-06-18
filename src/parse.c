#include "parse.h"
#include <stdarg.h>

TOKEN *getNextToken(TOKEN **token)
{
    freeToken(token);
    TOKEN* tok = scanner();
    // uncomment the line below if desired for debugging purposes.
    //printToken(&tok); fflush(stdout);
    return tok;
}

NODE *program()
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = PROGRAM_NODE;
    node->leftChild = statement();
    if (node->leftChild != NULL)
    {
        node->rightChild = program();
    }
    else
    {
        free(node);
        node = NULL;
    }
    return node;
}

NODE *statement()
{
    TOKEN *token = getNextToken(NULL);
    if (token == NULL)
    {
        return NULL;
    }
    else if (token->type == EOF_TOKEN)
    {
        freeToken(&token);
        return NULL;
    }

    NODE *node = calloc(sizeof(NODE), 1);
    node->type = STATEMENT_NODE;

    switch(token->type)
    {
        case IDENT_TOKEN:
            node->leftChild = assignStmt(&token);
            break;
        case REPEAT_TOKEN:
            node->leftChild = repeatStmt(&token);
            break;
        case PRINT_TOKEN:
            node->leftChild = printStmt(&token);
            break;
        default:
            // see the TOKEN_TYPE enum to see which numbers mean what...
            error("Invalid token at start of statement : ");
            printToken(&token);
    }

    return node;
}

NODE *assignStmt(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *repeatStmt(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *printStmt(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *expr(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *term(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *factor(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *ident(TOKEN **currToken)
{
    // TODO
    return NULL;
}

NODE *number(TOKEN **currToken)
{
    // TODO
    return NULL;
}

void freeParseTree(NODE **node)
{
    if (node == NULL)
    {
        return;
    }

    if ((*node) == NULL)
    {
        return;
    }

    freeParseTree(&(*node)->leftChild);
    freeParseTree(&(*node)->rightChild);

    if ((*node)->type == IDENT_NODE)
    {
        free((*node)->data.identifier);
    }

    free(*node);
    *node = NULL;
}

void error(char *errorFormat, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, errorFormat);
    vsnprintf (buffer, 255, errorFormat, args);

    printf("\nPARSING ERROR: %s\nExiting...\n", buffer);

    va_end (args);
    exit(1);
}
