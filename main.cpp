#include <stdlib.h>
#include <stdio.h>
#include "src/ppl.h"
#include "src/simplex.h"

int main() {

//    FILE* file = fopen("exemplo.txt","rt");

//    if (file == NULL){
//        printf("\nArchive Error\n\n");
//        return 1;
//    }

    PPL problema;

    problema.readPPL(stdin);
//    problema.imprimePPL();

    Simplex t;
    t.criaTablau(problema);
    t.simplex();
    t.imprimeResultado();

    return 0;
}