#ifndef PPL_H_
#define PPL_H_

#define MAIORIGUAL 1
#define MENORIGUAL 2
#define IGUAL 3

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <iomanip>


using namespace std;

class PPL {
private:
	double **matrixRestricao;
	char **restricoes;
	double *b;
	double *z;
	int n, m;
	char funcao[3];
	enum restricoesEnum { POS_CA, POS_NA, NEG_NA, NEG_CA, SEM_NA, SEM_CA};
    
public:
	/*
	void alocaMatriz(int linhas, int colunas);
	void alocaVetor(int tamanho);
	*/
    void readPPL(FILE *file);
    void imprimePPL();
    void hello();
    //char *getFuncao(){}
};


#endif /* PPL_H_ */
