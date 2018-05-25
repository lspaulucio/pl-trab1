#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <iomanip>

#include "ppl.h"
using namespace std;



void PPL::readPPL(FILE* file){


    //Lendo a função de max ou min e o tamanho MxN
    fscanf(file, "%[^\n]s", funcao);
    fscanf(file, "%d %d\n", &m, &n);

    /*ALOCANDO OS VETORES E MATRIZ PARA LEITURA*****************/
    b = (double*)malloc(m*sizeof(double));
    z = (double*)malloc(m*sizeof(double));

    matrixRestricao = (double**)malloc(m*sizeof(double*));
    for(int i = 0; i < m; i++){
        matrixRestricao[i] = (double*)malloc(n*sizeof(double));
    }
    restricoes = (char**)malloc(m*sizeof(char*));
    for(int i = 0; i < m; i++){
        restricoes[i] = (char*)malloc(3*sizeof(char));
    }
    /************************************************************/


    for(int i = 0; i < n; i++){
        fscanf(file, "%lf", &z[i]); 
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            fscanf(file, "%lf", &matrixRestricao[i][j]);
        }
        fscanf(file, "%3s", restricoes[i]);
        fscanf(file, "%lf", &b[i]);
    }
}


void PPL::imprimePPL(){
    printf("%s\n",funcao);
    printf("%d %d\n",m,n);

    for(int i = 0; i < n; i++){
        printf("%lf ", z[i]); 
    }
    printf("\n");
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf("%.1lf ", matrixRestricao[i][j]);
        }
        printf("%s ", restricoes[i]);
        printf("%.1lf\n", b[i]);
    }
}