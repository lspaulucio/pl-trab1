#include "simplex.h"
#include "ppl.h"

Simplex::Simplex() {
    tabela = NULL;
    bases = NULL;
    funcao = 0;
    linhas = 0;
    colunas = 0;
    za = -1;
    z = 0;
    b = 0;
    numVN = 0;
    numVF = 0;
    numVA = 0;
    numVB = 0;
}


void Simplex::criaTablau(PPL p){

    numVN = p.getNumVN(); //numero de variaveis naturais
    numVF = p.getNumVF(); //numero de variaveis folga
    numVA = p.getNumVA(); //numero de variaveis artificiais
    numVB = p.getNumVB(); //numero de variaveis basicas


    colunas = p.getNumColunas() + numVF + numVA; //calculas numero de colunas do tablau
    linhas = p.getNumLinhas(); //Calcula numero de linhas

    //se tem variaveis artificiais adiciona mais uma linha ao tablau
    if (numVA > 0) {
        linhas++;
        za = 0; //coloca a linha de za para 0
        z = 1;  //coloca a linha de z para 1
    }

    funcao = p.getTipoFuncao(); //obtem o tipo da funcao MAX ou MIN

    //indice de folga comeca no indice depois da ultima variavel natural

    int indiceFolga = numVN + 1;
    int indiceArtificial = numVN + numVF + 1;

    int i, j;

    //    printf("%d %d\n",colunas, linhas);


    //Alocando espaços de memoria para vetores e matriz
    tabela = (double **) calloc(linhas, sizeof(double *));

    for (i = 0; i < linhas; i++)
        tabela[i] = (double *) calloc(colunas, sizeof(double));

    bases = (int *) calloc(p.getNumVB(), sizeof(int));

    //Fim da alocacao

    //iniciando za artificial
    if (numVA > 0) {
        for (i = indiceArtificial; i < colunas; i++)
            tabela[za][i] = -1.0;
    }

    //Iniciando linha do z
    for (i = 1; i <= p.getNumVN(); i++)
        if (funcao == MIN) {
            tabela[z][i] = -p.getElement(0, i);
        } else {
            tabela[z][i] = p.getElement(0, i);
        }

    //Inicializando resto do tablau
    for (i = 1; i < p.getNumLinhas(); i++) {
        for (j = 0; j < p.getNumColunas(); j++) {
            tabela[z + i][j] = p.getElement(i, j);
        }
        printf("\n");
    }

    //Inicializando variaveis de folga e artificiais
    int indF = indiceFolga, indA = indiceArtificial;
    int indB = 0;
    for (int i = 1; i < p.getNumLinhas(); i++)
        switch (p.getRestricao(i)) {
            case POS_SA:
                tabela[z + i][indF] = 1;
                bases[indB++] = indF++;
                break;

            case NEG_CA:
                tabela[z + i][indF++] = -1;
                tabela[z + i][indA] = 1;
                bases[indB++] = indA++;
                break;

            case SO_CA:
                tabela[z + i][indA] = 1;
                bases[indB++] = indA++;
                break;

            default:
                printf("ERRO\n");
                break;
        }
}

void Simplex::imprimeTablau() {
    int i, j, k = 1;

    printf("\tb\t");

    for (j = 1; j <= numVN; j++)
        printf("X%d\t", k++);

    for (; j <= numVN + numVF; j++)
        printf("X%df\t", k++);

    if (numVA > 0) {
        for (; j <= numVN + numVF + numVA; j++)
            printf("X%da\t", k++);
    }

    printf("\n");

    k = 0;
    for (i = 0; i < linhas; i++) {
        if (i == 0 && numVA > 0) //Verifica se existem variaveis artificiais para printar o za
            printf("Za\t");
        else if ((numVA > 0 && i == 1) || (numVA == 0 && i == 0)) //verifica se z é a linha 0 ou a linha 1
            printf("Z\t");
        else
            printf("X%d\t", bases[k++]); //printa as variaveis basicas


//                printf("x1\t");

        for (j = 0; j < colunas; j++)
            printf("%.3lf\t", tabela[i][j]);

        printf("\n");
    }
    printf("\n");
}

void Simplex::simplex() {

    int indEntra = entraBase();
    int indSai;

    while(indEntra != -1) {

        checaColuna(indEntra);

        indSai = saiBase(indEntra);

        imprimeTablau();

        if (tabela[indSai][indEntra] != 1.0) {
            ajustaLinhaPivo(indSai, indEntra);
        }

        atualizaTablau(indSai, indEntra);

        indEntra = entraBase();
    }

    printf("Tablau final\n");
    imprimeTablau();
}

int Simplex::entraBase(){

    int indiceMaximo = -1;
    int maior = 0;

    for(int i = 1; i < colunas; i++) {
        if (tabela[z][i] > maior) {
            indiceMaximo = i;
            maior = tabela[z][i];
        }
    }

    if(indiceMaximo != -1) {
        printf("X%d entra na base\n", indiceMaximo);
    }

    return indiceMaximo;
}

int Simplex::saiBase(int coluna) {

    int indiceMinimo;
    double min = 1000000000;

    for(int i = 1; i < linhas; i++) {
        if (tabela[i][coluna] > 0) {
            if (tabela[i][b] / tabela[i][coluna] < min) {
                min = tabela[i][b] / tabela[i][coluna];
                indiceMinimo = i;
            }
        }
    }

    printf("X%d sai da base\n ", bases[indiceMinimo-1]);

    return  indiceMinimo;
}

void Simplex::ajustaLinhaPivo(int l, int c){

    double pivo = tabela[l][c];

    for(int i = 0; i < colunas; i++)
    {
        tabela[l][i] /= pivo;
    }
}

void Simplex::atualizaTablau(int l, int c) {

    double fator;

    for(int i = 0; i < linhas; i++) {
        if(i != l) {
            if (tabela[i][c] != 0) {
                fator = -tabela[i][c];

                for (int j = 0; j < colunas; j++) {
                    tabela[i][j] += (fator * tabela[l][j]);
                }
            }
        }
    }
    bases[l-1] = c;
}

bool Simplex::checaColuna(int c){

    for(int i = 1; i < linhas; i++)
        if(tabela[i][c] > 0)
            return true;

    printf("z -> ");
    if(funcao == MIN)
        printf("-inf\n");
    else
        printf("+inf\n");

    exit(1);
}

void Simplex::imprimeResultado() {

    bool variasSolucoes = false;

    printf("z* = %.3lf\n", tabela[z][b]);
    printf("x* = ( ");


    for(int i = 1; i < colunas; i++){
        if(naBase(i)){
            int j = getIndBase(i);
            printf("%.3lf ", tabela[j+1][b]);
        }
        else {
            if(tabela[z][i] == 0)
                variasSolucoes = true;

            printf("0 ");
        }
    }
    printf(")\n");

    if(variasSolucoes)
        printf("Infinitas solucoes\n");
    else
        printf("Solucao unica\n");

}

bool Simplex::naBase(int b)
{
    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return true;
    }
    return false;
}

int Simplex::getIndBase(int b){

    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return i;
    }
    return -1;
}