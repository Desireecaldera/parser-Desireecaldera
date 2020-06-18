#include "print.h"

int main(int argc, char **argv)
{
    freopen(argv[1], "r", stdin);

    NODE *fullProgram = program();
    printf("Done parsing...\n");
    printProgram(fullProgram, 0);
    freeParseTree(&fullProgram);

    exit(0);
}
