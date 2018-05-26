#ifndef _TABLAU_H
#define _TABLAU_H

#include <stdlib.h>
#include <stdio.h>

#include "ppl.h"

class Tablau {

    private:

    int funcao; //tipo da funcao MAX ou MIN

    double **tabela; //tablau
    int linhas; //numero de linhas do tablau
    int colunas; //numero de colunas do tablau

    int *bases; //vetor dos indices basicos

    int za; //indice da linha de za
    int z; //indice da linha de z

    int b; //indice da coluna de b

    int numVN; //numero de variaveis naturais
    int numVF; //numero de variaveis de folga
    int numVA; //numero de variaveis artificiais
    int numVB; //numero de variaveis basicas

    public:

    Tablau();

    void criaTablau(PPL);
    void imprimeTablau();

};


#endif //_TABLAU_H
