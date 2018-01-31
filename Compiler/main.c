#include <stdio.h>
#include <stdlib.h>

#include "lexical.h"
#include "util.h"
#include "parse.h"


int main(int argc, char* argv[])
{
    if(argc <= 1) {
        printf(" g3idaCompiler: fatal error no input files\n");
        return 0;
    }

    input = fopen(argv[1], "r");
    if(input == NULL) {
        printf(" g3idaCompiler: fatal error file not found %s\n", argv[1]);
    }

    printTree(parse());

    return 0;
}
