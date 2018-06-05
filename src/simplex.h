#ifndef _TABLAU_H
#define _TABLAU_H

#include <stdlib.h>
#include <stdio.h>

#include "ppl.h"

class Simplex {

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

    Simplex();

    void criaTablau(PPL);
    void imprimeTablau();
    void simplex();
    int entraBase(int);
    int saiBase(int);
    void atualizaTablau(int, int);
    void ajustaLinhaPivo(int, int);
    bool checaColuna(int);
    void imprimeResultado();
    void somaLinhas(int, int);
    void ajustaColunaArtificial();
    void primeiraFase();
    void analisaPrimeiraFase();
    void verificaBase();
    void segundaFase();
    int getIndBase(int);
    bool naBase(int);

};


#endif //_TABLAU_H
