#include "scan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

TOKEN *ungottenToken = NULL;

void ungetToken(TOKEN **token)
{
    ungottenToken = *token;
    *token = NULL;
}

void printToken (TOKEN **token)
{
    switch((*token)->type)
    {
        case INVALID_TOKEN:
            printf("<INVALID, %c>", (*token)->val.op);
            break;
        case REPEAT_TOKEN:
            printf("<KEYWORD, repeat>");
            break;
        case PRINT_TOKEN:
            printf("<KEYWORD, print>");
            break;
        case IDENT_TOKEN:
            printf("<IDENT, %s>", (*token)->val.string);
            break;
        case INT_TOKEN:
            printf("<INT, %ld>", (*token)->val.integral);
            break;
        case FLOAT_TOKEN:
            printf("<FLOAT, %lf>", (*token)->val.floating_point);
            break;
        case ASSIGNMENT_TOKEN:
            printf("<ASSIGN>");
            break;
        case LPAREN_TOKEN:
            printf("<LPAREN>");
            break;
        case RPAREN_TOKEN:
            printf("<RPAREN>");
            break;
        case ADD_OP_TOKEN:
            printf("<ADD_OP, %c>", (*token)->val.op);
            break;
        case MULT_OP_TOKEN:
            printf("<MULT_OP, %c>", (*token)->val.op);
            break;
        case SEMICOLON_TOKEN:
            printf("<SEMICOLON>");
            break;
        case EOF_TOKEN:
            printf("<EOF>");
            break;
        case NO_TOKEN_TYPE:
            printf("<NO_TYPE, something when wrong, should have been invalid token...>");
            break;
    }
    printf("\n");
    fflush(stdout);
}

//
// clean up the token structure
//
void freeToken(TOKEN **token)
{
    if (token == NULL)
        return;

    if (*token == NULL)
        return;

    if ((*token)->type == IDENT_TOKEN)
    {
        free((*token)->val.string);
    }

    free(*token);
    *token = NULL;
}

bool updateKeywordOrId(TOKEN *token, char *str)
{
    if (strcmp(str, "repeat") == 0)
    {
        token->type = REPEAT_TOKEN;
    }
    else if (strcmp(str, "print") == 0)
    {
        token->type = PRINT_TOKEN;
    }
    else
    {
        token->type = IDENT_TOKEN;
        token->val.string = malloc((strlen(str) + 1) * sizeof(char));
        strcpy(token->val.string, str);
        return false;
    }
    return true;
}

TOKEN *scanner()
{
    // reuse any token returned to the input
    if (ungottenToken != NULL)
    {
        TOKEN *tok = ungottenToken;
        ungottenToken = NULL;
        return tok;
    }

    // temporary buffer for collecting characters
    char tempString[BUF_SIZE];

    // acquire space for the new token
    TOKEN *token = (TOKEN *) calloc(sizeof(TOKEN), 1);
    token->type = NO_TOKEN_TYPE; // assume that the token is invalid to start with

    // analyze the input character by character figuring out what kind of token they constitute
    char c;
    int i;
    start: c = (char) getchar();
    switch (c)
    {
        case ' ': // skip whitespace
        case '\t':
        case '\n':
            goto start;
        case 'A'...'Z':
        case 'a'...'z':
        case '$':
        case '_':
            i = 0;
            do
            {
                tempString[i++] = c;
                c = (char) getchar();
            } while ((c >= 'A' && c <= 'z') || (c >= '0' && c <= '9') || c == '$' || c == '$');
            tempString[i] = '\0';
            ungetc(c, stdin); // the last character read is not a letter, so return it to the input stream
            updateKeywordOrId(token, tempString);
            break;
        case '0'...'9': // if c is a number
            i = 0;
            do
            {
                tempString[i++] = c;
                c = (char) getchar();
            } while (c >= '0' && c <= '9');
            if (c == '.')
            {
                tempString[i++] = c;
                c = (char) getchar();
                do
                {
                    tempString[i++] = c;
                    c = (char) getchar();
                } while (c >= '0' && c <= '9');
                tempString[i] = '\0';
                token->type = FLOAT_TOKEN;
                token->val.floating_point = strtod(tempString, NULL);
            }
            else
            {
                tempString[i] = '\0';
                token->type = INT_TOKEN;
                token->val.integral = strtol(tempString, NULL, 10);
            }
            ungetc(c, stdin);
            break;
        default:
            token->val.op = c;
            switch (c)
            {
                case '=':
                    token->type = ASSIGNMENT_TOKEN;
                    break;
                case ';':
                    token->type = SEMICOLON_TOKEN;
                    break;
                case '(':
                    token->type = LPAREN_TOKEN;
                    break;
                case ')':
                    token->type = RPAREN_TOKEN;
                    break;
                case '+':
                case '-':
                    token->type = ADD_OP_TOKEN;
                    break;
                case '*':
                case '/':
                case '%':
                    token->type = MULT_OP_TOKEN;
                    break;
                case EOF:
                    token->type = EOF_TOKEN;
                    break;
                default:
                    token->type = INVALID_TOKEN;
                    break;
            }
            break;
    }
    return token;
}