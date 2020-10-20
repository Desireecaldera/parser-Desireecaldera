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
    //<ident> = <expr>;

       //creating ident var with its value
       SYMBOL_TABLE_NODE *newVariable = createSymbol( node->leftChild->data.identifier, evalExpr( node->rightChild ));
       //checking if var is already in the table
       SYMBOL_TABLE_NODE *foundVar = findSymbol( symbolTable, newVariable->ident);
        if( foundVar!= NULL ){
            //if the variable is found, the value inside the variable is changed
            reassignSymbol( foundVar ,newVariable->value);

        } else{
            addSymbolToTable( &symbolTable, newVariable );
        }


    // TODO(done)
}

void evalRepeatStmt(NODE *node)
{
    //repeat ( <expr> ) <statement>

    int counter = 0;
    NUMBER expressionVal = evalExpr( node->leftChild );
    if( expressionVal.type == INT_TYPE ){

        //checking if expr > 0
        if(  expressionVal.value.integral > 0 ){
            counter =  (int) expressionVal.value.integral;
        }
        for( int i = 0; i < counter; i++ ){
            evalStatement(node->rightChild);
        }
    } else if( expressionVal.type == FLOAT_TYPE ){

        //checking if expr > 0
        if(  expressionVal.value.integral > 0 ){
            counter =  (int) node->leftChild->data.number.value.floating_point;
        }

        for( int i = 0; i < counter; i++ ){
            evalStatement(node->rightChild);
        }

    }

    // TODO(done)
}

void evalPrintStmt(NODE *node)
{
    //print ( <expr> );
   NUMBER expressionVal = evalExpr( node->leftChild);
    if( expressionVal.type == INT_TYPE ){
        printf( "INT: %ld \n", abs(expressionVal.value.integral) );
    } else if( expressionVal.type == FLOAT_TYPE ){
        printf( "FLOAT: %f \n", fabs(expressionVal.value.floating_point));
    }else{
        puts( "Expected a NUMBER in <expr> statement ");
    }
    // TODO(double check)
}

NUMBER evalExpr(NODE *node)
{
    //<term> | <term> <addop> <expr>
    NUMBER term = evalTerm( node->leftChild );
    if( node->rightChild != NULL ){
       return evalOperation( term, evalExpr(node->rightChild), node->data.op );

    }
    // TODO(done)
    return term;
}

NUMBER evalTerm(NODE *node)
{
    //<factor> | <factor> <multop> <term>
    NUMBER factor = evalFactor( node->leftChild );
    if( node->rightChild != NULL ){
       return evalOperation( factor, evalTerm( node->rightChild), node->data.op);
    }

    // TODO(done)
    return factor;
}

NUMBER evalFactor(NODE *node)
{
    // <id> | <number> | <addop> <factor> | ( <expr> )
    NUMBER factorVal = node->leftChild->data.number;
    switch ( node->leftChild->type ) {
        case IDENT_NODE:
       factorVal = evalId( node->leftChild );
            break;
        case NUMBER_NODE:
         factorVal = evalNumber( node->leftChild);

            break;
        case EXPR_NODE:
            factorVal = evalExpr( node->leftChild );

            break;
        default:
            return evalOperation( factorVal, evalFactor( node->leftChild), node->data.op);
    }

    // TODO(done)
    return factorVal;
}

NUMBER evalNumber(NODE *node)
{

        NUMBER numberVal;
        if( node->data.number.type == INT_TYPE ){
             numberVal.type = INT_TYPE;
            numberVal.value.integral = node->data.number.value.integral;
        } else {
            numberVal.type = FLOAT_TYPE;
            numberVal.value.floating_point = node->data.number.value.floating_point;
        }

    // TODO(done)
        return numberVal;

}

NUMBER evalId(NODE *node)
{
    SYMBOL_TABLE_NODE *foundVar = findSymbol( symbolTable, node->data.identifier);
    if( foundVar == NULL ){
        return NAN_NUMBER;
    }

    // TODO(done)
        return evalSymbol( foundVar );

}

NUMBER evalAdd(NUMBER op1, NUMBER op2)
{
    NUMBER total;
    if( op1.type == INT_TYPE && op2.type == INT_TYPE ){
         total.value.integral = op1.value.integral + op2.value.integral;
        total.type = INT_TYPE;

    } else if ( op1.type == INT_TYPE && op2.type == FLOAT_TYPE){
       total.type = FLOAT_TYPE;
        total.value.floating_point = (float) op1.value.integral + op2.value.floating_point;
    } else if(  op1.type == FLOAT_TYPE && op2.type == INT_TYPE){
       total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point + (float) op2.value.integral;
    } else{
       total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point + op2.value.floating_point;
    }


    // TODO(done)
    return total;

}

NUMBER evalSub(NUMBER op1, NUMBER op2)
{
    NUMBER total;
    if( op1.type == INT_TYPE && op2.type == INT_TYPE ){
        total.type = INT_TYPE;
        total.value.integral = op1.value.integral - op2.value.integral;

    } else if ( op1.type == INT_TYPE && op2.type == FLOAT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point = (float ) op1.value.integral - op2.value.floating_point;
    } else if(  op1.type == FLOAT_TYPE && op2.type == INT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point - (float ) op2.value.integral;
    } else{
        total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point - op2.value.floating_point;
    }

    // TODO(done)
    return total;

}

NUMBER evalMult(NUMBER op1, NUMBER op2)
{
    NUMBER total;
    if( op1.type == INT_TYPE && op2.type == INT_TYPE ){
        total.type = INT_TYPE;
        total.value.integral = op1.value.integral * op2.value.integral;

    } else if ( op1.type == INT_TYPE && op2.type == FLOAT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point = (float ) op1.value.integral * op2.value.floating_point;
    } else if(  op1.type == FLOAT_TYPE && op2.type == INT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point * (float ) op2.value.integral;
    } else{
        total.type = FLOAT_TYPE;
        total.value.floating_point =  op1.value.floating_point * op2.value.floating_point;
    }

    // TODO(done)
    return total;

}

NUMBER evalDiv(NUMBER op1, NUMBER op2)
{
    NUMBER total;
    if( op1.type == INT_TYPE && op2.type == INT_TYPE ){
        total.type = INT_TYPE;
        total.value.integral = op1.value.integral / op2.value.integral;

    } else if ( op1.type == INT_TYPE && op2.type == FLOAT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point = floor( (float ) op1.value.integral / op2.value.floating_point );
    } else if(  op1.type == FLOAT_TYPE && op2.type == INT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point =  floor( op1.value.floating_point / (float ) op2.value.integral );
    } else{
        total.type = FLOAT_TYPE;
        total.value.floating_point =  floor( op1.value.floating_point / op2.value.floating_point );
    }

    // TODO(done)
    return total;

}

NUMBER evalMod(NUMBER op1, NUMBER op2)
{
    NUMBER total;
    if( op1.type == INT_TYPE && op2.type == INT_TYPE ){
        total.type = INT_TYPE;
        total.value.integral = op1.value.integral % op2.value.integral;

    } else if ( op1.type == INT_TYPE && op2.type == FLOAT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point = fmod( (float ) op1.value.integral, op2.value.floating_point );
    } else if(  op1.type == FLOAT_TYPE && op2.type == INT_TYPE){
        total.type = FLOAT_TYPE;
        total.value.floating_point =  fmod( op1.value.floating_point, (float) op2.value.integral );
    } else{
        total.type = FLOAT_TYPE;
        total.value.floating_point =  fmod( op1.value.floating_point, op2.value.floating_point );
    }


    // TODO(done)
    return total;
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
            //returns the table
            return table;
        }
        table = table->next;
    }
    //find symbol returns null if not found
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
