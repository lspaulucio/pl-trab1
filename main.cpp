#include <stdlib.h>
#include <stdio.h>
#include "src/ppl.h"
#include "src/tablau.h"

int main() {

//    FILE* file = fopen("exemplo.txt","rt");

//    if (file == NULL){
//        printf("\nArchive Error\n\n");
//        return 1;
//    }

    PPL problema;

    problema.readPPL(stdin);
//    problema.imprimePPL();

    Tablau t;
    t.criaTablau(problema);
    t.imprimeTablau();


    return 0;
}