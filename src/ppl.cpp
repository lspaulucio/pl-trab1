#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppl.h"

PPL::PPL(){
    matrizPPL = NULL;
    restricoes = NULL;
    n = 0;
    m = 0;
    tipoFuncao = 0;
    numLinhas = 0;
    numColunas = 0;
}

void PPL::readPPL(FILE* file){

    char tipo[3];

    //Lendo a função de max ou min e o tamanho MxN
    fscanf(file, "%s", tipo);
    fscanf(file, "%d %d", &m, &n);

    /*ALOCANDO A MATRIZ PARA A LEITURA DO PPL*****************/

    numLinhas = m + 1;
    numColunas = n + 1;

    restricoes = (int*) calloc(numLinhas, sizeof(int));
    matrizPPL = (double**) calloc(numLinhas, sizeof(double*));

    for(int i = 0; i < numLinhas; i++){
        matrizPPL[i] = (double*) calloc(numColunas, sizeof(double));
    }


    /************************************************************/
    int i, j;
    char sinal[2];

    for(i = 1; i < numColunas ; i++)
        fscanf(file, "%lf", &matrizPPL[0][i]);

    matrizPPL[0][0] = 0;

    tipoFuncao = checaSinal(tipo);

    restricoes[0] = checaSinal(tipo);

    for(i = 1; i < numLinhas; i++)
    {
        for(j = 1; j < numColunas; j++)
            fscanf(file, "%lf", &matrizPPL[i][j]);

        fscanf(file, "%s %lf", sinal, &matrizPPL[i][0]);
        restricoes[i] = checaSinal(sinal);
        //criar funcao para caso o b seja negativo inverter o sinal da linha
    }
}

void PPL::imprimePPL(){

    int i, j;
    printf("%d\n", tipoFuncao);
    printf("%d %d\n",m,n);

    for(i = 0; i < numLinhas ; i++)
    {
        for(j = 0; j < numColunas; j++)
            printf("%.3lf\t", matrizPPL[i][j]);

        printf("%d\n", restricoes[i]);
    }
}

int PPL::checaSinal(char *s){

    if(!strcmp(s, "<="))
    {
        return POS_SA;
    }
    else if(!strcmp(s, ">="))
    {
        return NEG_CA;
    }
    else if(!strcmp(s, "="))
    {
        return SO_CA;
    }
    else if(!strcmp(s, "max"))
    {
        return MAX;
    }
    else if(!strcmp(s, "min"))
    {
        return MIN;
    }

    printf("%s\n", s);
    return 0;
}

int PPL::getNumLinhas() const {
    return numLinhas;
}

void PPL::setNumLinhas(int numLinhas) {
    PPL::numLinhas = numLinhas;
}

int PPL::getNumColunas() const {
    return numColunas;
}

void PPL::setNumColunas(int numColunas) {
    PPL::numColunas = numColunas;
}

double PPL::getElement(int i, int j) {
    return matrizPPL[i][j];
}

int PPL::getTipoFuncao(){
    return tipoFuncao;
}

int PPL::getRestricao(int i){
    return restricoes[i];
}

int PPL::getNumVN()
{
    return n;
}

int PPL::getNumVA(){
    int num = 0;

    for(int i = 1; i < numLinhas; i++){
        if(getRestricao(i) == SO_CA || getRestricao(i) == NEG_CA) {
            num++;
        }
    }
    return num;
}

int PPL::getNumVF(){
    int num = 0;

    for(int i = 1; i < numLinhas; i++){
        if(getRestricao(i) == NEG_CA || getRestricao(i) == POS_SA) {
            num++;
        }
    }
    return num;
}

int PPL::getNumVB(){
    return m;
}