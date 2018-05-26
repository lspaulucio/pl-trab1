    #include "tablau.h"
    #include "ppl.h"

    Tablau::Tablau(){
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


    void Tablau::criaTablau(PPL p){

        numVN = p.getNumVN(); //numero de variaveis naturais
        numVF = p.getNumVF(); //numero de variaveis folga
        numVA = p.getNumVA(); //numero de variaveis artificiais

        colunas = p.getNumColunas() + numVF + numVA; //calculas numero de colunas do tablau
        linhas = p.getNumLinhas(); //Calcula numero de linhas

        //se tem variaveis artificiais adiciona mais uma linha ao tablau
        if(numVA > 0) {
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
        tabela = (double**) calloc(linhas, sizeof(double*));

        for(i = 0; i < linhas; i++)
            tabela[i] = (double*) calloc(colunas, sizeof(double));

        bases = (int*) calloc(p.getNumVB(), sizeof(int));

        //Fim da alocacao

        //iniciando za artificial
        if(numVA > 0){
            for(i = indiceArtificial; i < colunas; i++)
                tabela[za][i] = -1.0;
        }

        //Iniciando linha do z
        for(i = 1; i <= p.getNumVN(); i++)
            if(funcao == MIN){
                tabela[z][i] = -p.getElement(0,i);
            }
            else{
                tabela[z][i] = p.getElement(0,i);
            }

        //Inicializando resto do tablau
        for(i = 1; i < p.getNumLinhas(); i++)
        {
            for(j = 0; j < p.getNumColunas(); j++){
                tabela[z + i][j] = p.getElement(i, j);
            }
            printf("\n");
        }

        //Inicializando variaveis de folga e artificiais
        int indF = indiceFolga, indA = indiceArtificial;
        int indB = 0;
        for(int i = 1; i < p.getNumLinhas(); i++)
            switch(p.getRestricao(i))
            {
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

    void Tablau::imprimeTablau() {
        int i, j, k=1;

        printf("\tb\t");

        for(j = 1; j <= numVN; j++)
            printf("X%d\t",k++);

        for(; j <= numVN + numVF; j++)
            printf("X%df\t",k++);

        if(numVA > 0) {
            for (; j <= numVN + numVF + numVA; j++)
                printf("X%da\t", k++);
        }

        printf("\n");

        k=0;
        for(i = 0; i < linhas ; i++)
        {
            if(i == 0 && numVA > 0) //Verifica se existem variaveis artificiais para printar o za
                printf("Za\t");
            else if((numVA > 0 && i == 1) || (numVA == 0 && i == 0)) //verifica se z é a linha 0 ou a linha 1
                printf("Z\t");
            else
                printf("X%d\t", bases[k++]); //printa as variaveis basicas


//                printf("x1\t");

            for(j = 0; j < colunas; j++)
                printf("%.3lf\t", tabela[i][j]);

            printf("\n");
        }
    }

