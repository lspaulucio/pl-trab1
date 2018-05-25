#include <iostream>
#include "ppl.h"

using namespace std;


int main() {

    FILE* file = fopen("exemplo.txt","rt");

    if (file == NULL){
        printf("\nArchive Error\n\n");
        return 1;
    }

    PPL problema;

    problema.readPPL(file);
    problema.imprimePPL();
    
    return 0;
}