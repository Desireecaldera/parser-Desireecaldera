#include "eval.h"

int main(int argc, char **argv)
{
    freopen(argv[1], "r", stdin);

    NODE *fullProgram = program();
    printf("Done parsing...\n");
    evalProgram(fullProgram);
    freeParseTree(&fullProgram);
    cleanUpSymbolTables();

    exit(0);
}