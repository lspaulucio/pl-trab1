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


    colunas = p.getNumColunas() + numVF + numVA; //calcula numero de colunas do tablau
    linhas = p.getNumLinhas(); //calcula numero de linhas

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

    bases = (int *) calloc(numVB, sizeof(int));

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
    for (i = 1; i < p.getNumLinhas(); i++)
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

    //printa as variaveis que estao na base
    for (j = 1; j <= numVN; j++)
        printf("X%d\t", k++);

    for (; j <= numVN + numVF; j++)
        printf("X%df\t", k++);

    if (numVA > 0) {
        for (; j <= numVN + numVF + numVA; j++)
            printf("X%da\t", k++);
    }

    printf("\n");

    k = 0; //variavel auxiliar para printar os indices que estao na base

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

    if(numVA > 0)
    {
        //primeira fase
    }
    else{
        printf("Iniciando segunda fase do simplex\n");
        segundaFase();
    }

    //imprime ultimo tablau
    printf("Tablau final\n");
    imprimeTablau();
}

void Simplex::segundaFase() {

    int indEntra = entraBase(); //Obtem nova variavel que entra na base
    int indSai;

    while(indEntra != -1) { //Checa se existe um índice que pode entrar na base

        indSai = saiBase(indEntra); //obtem variavel que sai da base

        imprimeTablau(); //imprime tablau

        //Se o pivo for diferente de 1 ajusta a linha
        if (tabela[indSai][indEntra] != 1.0) {
            ajustaLinhaPivo(indSai, indEntra);
        }

        //Atualiza o tablau com a nova variavel basica
        atualizaTablau(indSai, indEntra);

        //Obtem nova variavel que entra na base
        indEntra = entraBase();
    }

}

int Simplex::entraBase(){

    int indiceMaximo = -1;
    double maior = 0;

    for(int i = 1; i < colunas; i++) {
        if (tabela[z][i] > maior) {
            indiceMaximo = i;
            maior = tabela[z][i];
        }
    }

    if(indiceMaximo != -1) {
        printf("X%d entra na base\n", indiceMaximo);
        checaColuna(indiceMaximo);
    }

    return indiceMaximo;
}

int Simplex::saiBase(int coluna) {

    int indiceMinimo = 0;
    double min = 1000000000;

    for(int i = 1; i < linhas; i++) {
        if (tabela[i][coluna] > 0) {
            if (tabela[i][b] / tabela[i][coluna] < min) {
                min = tabela[i][b] / tabela[i][coluna];
                indiceMinimo = i;
            }
        }
    }

    if(indiceMinimo > 0) //Se for um indice valido
        printf("X%d sai da base\n ", bases[indiceMinimo-1]);

    return  indiceMinimo;
}

void Simplex::ajustaLinhaPivo(int l, int c){

    double pivo = tabela[l][c];

    //divide toda a linha do pivo pelo proprio pivo para ele se tornar 1

    for(int i = 0; i < colunas; i++)
    {
        tabela[l][i] /= pivo;
    }
}

void Simplex::atualizaTablau(int l, int c) {

    double fator; //variavel que multiplica o pivo para tornar a coluna igual a 0

    for(int i = 0; i < linhas; i++) {
        if(i != l) { //se a linha nao for a do pivo zera
            if (tabela[i][c] != 0) {

                fator = -tabela[i][c]; //calcula o fator da linha

                //atualiza toda a linha
                for (int j = 0; j < colunas; j++) {
                    tabela[i][j] += (fator * tabela[l][j]);
                }
            }
        }
    }

    //atualiza a variavel na base
    bases[l-1] = c;
}

bool Simplex::checaColuna(int c){

    //checa se a coluna da variavel candidata e toda negativa

    for(int i = 1; i < linhas; i++)
        if(tabela[i][c] > 0)
            return true;

    //se for solucao tende ao infinito

    printf("Tablau final\n");
    imprimeTablau();

    printf("Nao existe solucao: z -> ");
    if(funcao == MIN)
        printf("-inf\n");
    else
        printf("+inf\n");

    exit(EXIT_SUCCESS);
}

void Simplex::imprimeResultado() {

    bool variasSolucoes = false;
    bool degenerada = false;

    printf("z* = %.3lf\n", tabela[z][b]);
    printf("x* = ( ");

    //
    for(int i = 1; i < colunas; i++){

        if(naBase(i)){ //se a variavel esta na base
            int j = getIndBase(i);
            printf("%.3lf ", tabela[j+1][b]); //imprime o valor da variavel
        }
        else {
            if(tabela[z][i] == 0) //checa se uma variavel nao basica e zero na linha do z
                variasSolucoes = true; //se for tem varias solucoes

            printf("0 ");
        }
    }
    printf(")\n");

    //checa se alguma variavel basica e zero, se for a solucao e degenerada
    for(int i = z + 1; i < linhas; i++)
        if(tabela[i][b] == 0)
            degenerada = true;

    if(variasSolucoes)
        printf("Infinitas solucoes\n");
    else
        printf("Solucao unica\n");

    if(degenerada)
        printf("Solucao degenerada\n");
}

bool Simplex::naBase(int b)
{
    //verifica se a variavel b esta na base
    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return true;
    }
    return false;
}

int Simplex::getIndBase(int b){
    // obtem o indice da variavel na base, para poder imprimir
    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return i;
    }
    return -1;
}