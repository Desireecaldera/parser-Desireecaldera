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
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = ASSIGN_STMT_NODE;

    node->leftChild = ident(currToken);

    //verify that next token is is a valid sign (=)

    *currToken = getNextToken( currToken );
    if( (*currToken)->type !=ASSIGNMENT_TOKEN ){
        error( "Expected assigment token in assignment statement");

    }
    *currToken = getNextToken(currToken);

    node->rightChild = expr( currToken );

    //now read next token
    *currToken = getNextToken(currToken);
    //currTok should be a semicol now

    if( (*currToken)->type !=SEMICOLON_TOKEN){
        error( "Expected assigment token in assignment statement");

    }

    freeToken( currToken );


    // TODO(done)
    return node;
}

NODE *repeatStmt(TOKEN **currToken)
{
    //repeat ( <expr> ) <statement>
    NODE *node = calloc( sizeof( NODE ), 1);
    node->type = REPEAT_STMT_NODE;

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != REPEAT_TOKEN ){
        error( "Expected repeat keyword");
    }

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != LPAREN_TOKEN ){
        error( "Expected left parenthesis");
    }

    *currToken = getNextToken( currToken );
    node->leftChild = expr( currToken );

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != RPAREN_TOKEN ){
        error( "Expected right parenthesis");
    }

    *currToken = getNextToken( currToken );
    node->rightChild = statement();

    //do i need to free here? yes because we do not pass into a function that frees it
    //nor do we call getNextToken AGAIN to free currToken
    freeToken( currToken);

    // TODO(done double check)
    return node;
}

NODE *printStmt(TOKEN **currToken)
{
    //print ( <expr> );
    NODE *node = calloc( sizeof( NODE ), 1 );
    node->type = PRINT_STMT_NODE;

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != PRINT_TOKEN ){
        error( "Expected repeat keyword");
    }

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != LPAREN_TOKEN ){
        error( "Expected left parenthesis");
    }

    *currToken = getNextToken( currToken );
    node->leftChild = expr( currToken );

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != RPAREN_TOKEN ){
        error( "Expected reft parenthesis");
    }

    *currToken = getNextToken( currToken );
    if( (*currToken)->type != SEMICOLON_TOKEN ){
        error( "Expected a semicolon" );
    }


    freeToken( currToken );

    // TODO(done double check)
    return node;
}

NODE *expr(TOKEN **currToken)
{
    //<term> | <term> <addop> <expr>
    NODE *node = calloc( sizeof( NODE ), 1 );
    node->type = EXPR_NODE;

    //<term>
    *currToken = getNextToken( currToken );
    node->leftChild = term( currToken );

    *currToken = getNextToken( currToken );

    //check for <addop>
    if( (*currToken)->type != ADD_OP_TOKEN ){
        ungetToken( currToken );
    } else{
        //double check this one vvv
        node->data.op =  *currToken;

        //<expr>
        *currToken = getNextToken( currToken );
        node->rightChild = expr( currToken );

    }

    // TODO(done double check)
    return node;
}

NODE *term(TOKEN **currToken)
{
    //<factor> | <factor> <multop> <term>

    NODE *node = calloc( sizeof( NODE ), 1 );
    node->type = TERM_NODE;

    //<factor>
    *currToken = getNextToken( currToken );
    node->leftChild = factor( currToken );

    *currToken = getNextToken( currToken );

    //checking for <multop>
    if( (*currToken)->type != MULT_OP_TOKEN ){
        ungetToken( currToken );
    } else{
        node->data.op = *currToken;

        //<term>
        *currToken = getNextToken( currToken );
        node->rightChild = term( currToken );
    }


    // TODO(done double check)
    return node;
}

NODE *factor(TOKEN **currToken)
{
   // <id> | <number> | <addop> <factor> | ( <expr> )
    NODE *node = calloc( sizeof( NODE ), 1 );
    node->type = FACTOR_NODE;


    *currToken = getNextToken( currToken );
    //<id>
    if( (*currToken)->type == IDENT_TOKEN ){
        node->leftChild = ident( currToken );

        //<number>
    } else if( (*currToken)->type == NO_TOKEN_TYPE ){
        node->leftChild = number( currToken);

        //<addop>
    } else if( (*currToken)->type == ADD_OP_TOKEN ){
        node->data.op = *currToken;

        //<factor>
        *currToken = getNextToken( currToken );
        node->rightChild = factor( currToken );


        // (<expr>)
    }else if( (*currToken)->type == LPAREN_TOKEN ){
        node->leftChild = expr( currToken);

        *currToken = getNextToken( currToken );
        if( (*currToken)->type != RPAREN_TOKEN ){
            error( "Expected reft parenthesis");
        }
        freeToken( currToken );

    } else{
        error( "Not a valid factor statement");
    }


    // TODO(done double check)
    return node;
}

NODE *ident(TOKEN **currToken)
{
    // <letter> | <id> <letter> | <id> <digit>
    NODE *node  = calloc( sizeof( NODE ), 1 );
    node->type = IDENT_NODE;

    //we need to strcpy into ident

    // TODO
    return NULL;
}

NODE *number(TOKEN **currToken)
{
    //<int> | <float>
    NODE *node = calloc( sizeof( NODE ), 1 );
    node->type = NUMBER_NODE;

    *currToken = getNextToken( currToken );
    if( (*currToken)->type == INT_TOKEN ){
        node->data.number.type = INT_TYPE;
        node->data.number.value.integral = strtol(*currToken, NULL, 10 );
    } else if ((*currToken)->type == FLOAT_TOKEN ){
        node->data.number.type = FLOAT_TYPE;
        node->data.number.value.floating_point = strtod(*currToken, NULL);
    }

    // TODO(done but double check)
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
