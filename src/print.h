#ifndef __PRINT_H
#define __PRINT_H

#include "parse.h"
#include <stdarg.h>

void printProgram(NODE *node, int indent);
void printStatement(NODE *node, int indent);
void printAssignStmt(NODE *node, int indent);
void printRepeatStmt(NODE *node, int indent);
void printPrintStmt(NODE *node, int indent);
void printExpr(NODE *node, int indent);
void printTerm(NODE *node, int indent);
void printFactor(NODE *node, int indent);
void printId(NODE *node, int indent);
void printNumber(NODE *node, int indent);
void printfIndented(int indent, char *format, ...);

#endif