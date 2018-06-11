#include "simplex.h"
#include "ppl.h"


//Construtor
Simplex::Simplex() {
    tablau = NULL;
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

//Funcao que cria o tableau a partir do PPL
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
    tablau = (double **) calloc(linhas, sizeof(double *));

    for (i = 0; i < linhas; i++)
        tablau[i] = (double *) calloc(colunas, sizeof(double));

    bases = (int *) calloc(numVB, sizeof(int));

    //Fim da alocacao

    //iniciando za artificial
    if (numVA > 0) {
        for (i = indiceArtificial; i < colunas; i++)
            tablau[za][i] = -1.0;
    }

    //Iniciando linha do z
    for (i = 1; i <= p.getNumVN(); i++)
        if (funcao == MIN) {
            tablau[z][i] = -p.getElement(0, i);
        } else {
            tablau[z][i] = p.getElement(0, i);
        }

    //Inicializando resto do tablau
    for (i = 1; i < p.getNumLinhas(); i++) {
        for (j = 0; j < p.getNumColunas(); j++) {
            tablau[z + i][j] = p.getElement(i, j);
        }
        printf("\n");
    }

    //Inicializando variaveis de folga e artificiais
    int indF = indiceFolga, indA = indiceArtificial;
    int indB = 0;
    for (i = 1; i < p.getNumLinhas(); i++)
        switch (p.getRestricao(i)) {
            case POS_SA:
                tablau[z + i][indF] = 1;
                bases[indB++] = indF++;
                break;

            case NEG_CA:
                tablau[z + i][indF++] = -1;
                tablau[z + i][indA] = 1;
                bases[indB++] = indA++;
                break;

            case SO_CA:
                tablau[z + i][indA] = 1;
                bases[indB++] = indA++;
                break;

            default:
                printf("ERRO\n");
                break;
        }
}

//Funcao que imprime o tablau
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

        for (j = 0; j < colunas; j++)
            printf("%.3lf\t", tablau[i][j]);

        printf("\n");
    }
    printf("\n");
}

//Funcao que realiza o algoritmo do simplex
void Simplex::simplex() {

    if(numVA > 0)
    {
        printf("Iniciando primeira fase do simplex\n");
        imprimeTablau();
        primeiraFase();
        printf("Criterio de parada para minimizacao de Za atingido.\n");
        analisaPrimeiraFase();
    }


    printf("Iniciando segunda fase do simplex\n\n");
    segundaFase();

    printf("Criterio de parada para minimizacao de Z atingido.\n");
    //imprime ultimo tablau
    printf("Tablau final\n");
    imprimeTablau();
}

//Primeira fase do algoritmo simplex
void Simplex::primeiraFase(){

    int indEntra;
    int indSai;

    ajustaColunaArtificial();

    indEntra = entraBase(za); //Obtem nova variavel que entra na base

    while(indEntra != -1) { //Checa se existe um índice que pode entrar na base

        indSai = saiBase(indEntra); //obtem variavel que sai da base

        imprimeTablau(); //imprime tablau

        //Se o pivo for diferente de 1 ajusta a linha
        if (tablau[indSai][indEntra] != 1.0) {
            ajustaLinhaPivo(indSai, indEntra);
        }

        //Atualiza o tablau com a nova variavel basica
        atualizaTablau(indSai, indEntra);

        //Obtem nova variavel que entra na base
        indEntra = entraBase(za);
    }
}

//Segunda fase do algoritmo simplex
void Simplex::segundaFase() {

    int indEntra = entraBase(z); //Obtem nova variavel que entra na base
    int indSai;

    while(indEntra != -1) { //Checa se existe um índice que pode entrar na base

        indSai = saiBase(indEntra); //obtem variavel que sai da base

        imprimeTablau(); //imprime tablau

        //Se o pivo for diferente de 1 ajusta a linha
        if (tablau[indSai][indEntra] != 1.0) {
            ajustaLinhaPivo(indSai, indEntra);
        }

        //Atualiza o tablau com a nova variavel basica
        atualizaTablau(indSai, indEntra);

        //Obtem nova variavel que entra na base
        indEntra = entraBase(z);
    }

}

//Realiza a analise ao fim da primeira fase
void Simplex::analisaPrimeiraFase() {

    if(tablau[za][b] != 0) {
        imprimeTablau();
        printf("Conjunto vazio\n");
        exit(1);
    }

    printf("Verificando se existe variavel artificial na base.\n");

    verificaBase();

    printf("Nenhuma variavel artificial na base.\n");
    printf("Linha do Za e colunas das variaveis artificiais removidas.\n");
    eliminaLinha(za);
    colunas -= numVA;
    numVA = 0;
    z = 0;

    imprimeTablau();

}

//Elimina linha do tableau
void Simplex::eliminaLinha(int l){

    if(l == linhas - 1){
        linhas--;
        return;
    }

    for(int j = l; j < linhas - 1; j++) {
        for (int c = 0; c < colunas; c++) {
            tablau[j][c] = tablau[j + 1][c];
        }
    }

    linhas--;
}

//Ajusta a base ao final da primeira fase se existirem artificias nela
void Simplex::verificaBase(){

    imprimeTablau();

    int indArt = linhaArtificialBase();

    while(indArt != -1){

        int linha = indArt+z+1;
        bool nula = true;

        for(int j = 0; j < numVN + numVF + 1; j++)
            nula &= (tablau[linha][j] == 0.0);

        if(nula){

            printf("Linha da variavel basica artificial nula, removendo linha\n");
            eliminaLinha(linha);

            imprimeTablau();

            if(indArt == numVB - 1) {
                numVB--;
            }
            else {
                for (int i = indArt; i < numVB - 1; i++) {
                    bases[i] = bases[i + 1];
                    numVB--;
                }
            }
        }
        else {

            for(int i = 1; i < colunas; i++){
                if (!taNaBase(i)) {
                    ajustaLinhaPivo(linha, i);
                    atualizaTablau(linha, i);

                    printf("Linha da variavel basica artificial nao nula.\n");
                    printf("Forcando X%d a entrar na base\n", i);
                    imprimeTablau();
                    break;
                }
            }
        }

        indArt = linhaArtificialBase();
    }
}

//Retorna a linha da artificial na base
int Simplex::linhaArtificialBase(){

    int indArt = numVN + numVF;

    for(int i = 0; i < numVB; i++){
        if(bases[i] > indArt)
            return i;
    }

    return -1;
}


//Soma linhas do Tableau
void Simplex::somaLinhas(int linha1, int linha2){

    for(int i = 0; i < colunas; i++)
        tablau[linha1][i] += tablau[linha2][i];
}

//Realiza o ajuste inicial da primeira fase do simplex
void Simplex::ajustaColunaArtificial(){

    printf("Ajustando colunas das variaveis basicas artificiais.\n");

    int col = numVN + numVF + 1;

    for(int i = col; i < colunas; i++) {
        for(int j = z + 1; j < linhas; j++){
           if(tablau[j][i] == 1) {
               somaLinhas(za, j);
               imprimeTablau();
               break;
           }
       }
    }
}

//Encontra a variavel que entrara na base
int Simplex::entraBase(int linha){

    int indiceMaximo = -1;
    double maior = 0;

    for(int i = 1; i < colunas; i++) {
        if (tablau[linha][i] > maior) {
            indiceMaximo = i;
            maior = tablau[linha][i];
        }
    }

    if(indiceMaximo != -1) {
        printf("X%d entrara na base\n", indiceMaximo);
        checaColuna(indiceMaximo);
    }

    return indiceMaximo;
}

//Encontra a variavel que saira da base
int Simplex::saiBase(int coluna) {

    int indiceMinimo = 0;
    double min = 1000000000;

    for(int i = z + 1; i < linhas; i++) {
        if (tablau[i][coluna] > 0) {
            if (tablau[i][b] / tablau[i][coluna] < min) {
                min = tablau[i][b] / tablau[i][coluna];
                indiceMinimo = i;
            }
        }
    }

    if(indiceMinimo > 0) //Se for um indice valido
        printf("X%d saira da base\n ", bases[indiceMinimo-z-1]);

    return indiceMinimo;
}

//Ajusta a linha do pivo se ele for diferente de 1
void Simplex::ajustaLinhaPivo(int l, int c){

    double pivo = tablau[l][c];

    //divide toda a linha do pivo pelo proprio pivo para ele se tornar 1

    for(int i = 0; i < colunas; i++)
    {
        tablau[l][i] /= pivo;
    }
}

//Realiza as operacoes sobre o tableau para que a coluna seja zero menos na variavel da base
void Simplex::atualizaTablau(int l, int c) {

    double fator; //variavel que multiplica o pivo para tornar a coluna igual a 0

    for(int i = 0; i < linhas; i++) {
        if(i != l) { //se a linha nao for a do pivo zera
            if (tablau[i][c] != 0) {

                fator = -tablau[i][c]; //calcula o fator da linha

                //atualiza toda a linha
                for (int j = 0; j < colunas; j++) {
                    tablau[i][j] += (fator * tablau[l][j]);
                }
            }
        }
    }

    //atualiza a variavel na base
    bases[l-z-1] = c;
}

//Verifica se a coluna e negativa e a solucao tende a inf
bool Simplex::checaColuna(int c){

    //checa se a coluna da variavel candidata e toda negativa

    for(int i = z + 1; i < linhas; i++)
        if(tablau[i][c] > 0)
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

//Verifica resultado ao final da segunda fase e imprime o resultado
void Simplex::imprimeResultado() {

    bool variasSolucoes = false;
    bool degenerada = false;

    printf("z* = ");

    if(funcao == MAX)
        printf("%.3lf\n", -tablau[z][b]);
    else
        printf("%.3lf\n", tablau[z][b]);

    printf("x* = ( ");

    //
    for(int i = 1; i < colunas; i++){

        if(taNaBase(i)){ //se a variavel esta na base
            int j = getIndBase(i);
            printf("%.3lf ", tablau[j+1+z][b]); //imprime o valor da variavel
        }
        else {
            if(tablau[z][i] == 0) //checa se uma variavel nao basica e zero na linha do z
                variasSolucoes = true; //se for tem varias solucoes

            printf("0.000 ");
        }
    }
    printf(")\n");

    //checa se alguma variavel basica e zero, se for a solucao e degenerada
    for(int i = z + 1; i < linhas; i++)
        if(tablau[i][b] == 0)
            degenerada = true;

    if(variasSolucoes)
        printf("Multiplas solucoes\n");
    else
        printf("Solucao unica\n");

    if(degenerada)
        printf("Solucao degenerada\n");
}


//Verifica se a variavel b esta na base
bool Simplex::taNaBase(int b)
{
    //verifica se a variavel b esta na base
    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return true;
    }
    return false;
}

//Retorna o indice da variavel b que esta na base
int Simplex::getIndBase(int b){
    // obtem o indice da variavel na base, para poder imprimir
    for(int i = 0; i < numVB ; i++){
        if(bases[i] == b)
            return i;
    }
    return -1;
}