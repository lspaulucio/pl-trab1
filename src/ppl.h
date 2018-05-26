#ifndef PPL_H_
#define PPL_H_

#include <stdlib.h>
#include <stdio.h>

enum tipo {MAX = 1, MIN, POS_SA, NEG_CA, SO_CA};

int checaSinal(char*);

class PPL {

	private:

    int n, m;
    double **matrizPPL;
    int *restricoes;

    int tipoFuncao;

    int numLinhas;
    int numColunas;

public:

    int getRestricao(int);

    int getNumVN();
    int getNumVA();
    int getNumVF();
    int getNumVB();


    int getNumLinhas() const;
    void setNumLinhas(int);
    int getNumColunas() const;
    void setNumColunas(int);
    double getElement(int, int);
	int getTipoFuncao();
public:

    PPL();
    void readPPL(FILE *file);
    void imprimePPL();
    int checaSinal(char*);

};

#endif /* PPL_H_ */
