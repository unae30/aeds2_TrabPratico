#include <stdio.h>
#include <stdlib.h>
#include "ingresso.c"
#include "cliente.c"
#include <time.h>

// Declara um ponteiro para o arquivo que irá registrar o tempo
FILE *arquivoTempos = NULL;

void salvarTempoGasto(double tempo)
{
    if (arquivoTempos == NULL)
    {
        // Abre o arquivo para escrita em modo "append" (adicionar ao final do arquivo)
        if ((arquivoTempos = fopen("tempos.txt", "a")) == NULL)
        {
            printf("Erro ao abrir arquivo tempos.txt\n");
            exit(1);
        }
    }

    // Escreve o tempo gasto no arquivo
    fprintf(arquivoTempos, "%.5f segundos\n", tempo);

}

//----------------------------------
/*
void  Inserir_clientes(FILE *out)
{
    int cont = 0;
    int Clientes_desordenados[5000];
    while(cont < 5000)
    {
        //rand - gera valores aleatorios
        int num = rand();
        int aux = 0;
        for(int i = 0; i < cont; i++)
        {
            //confere resultado da função rand
            if(Clientes_desordenados[i] == num)
            {
                aux = 1; //Se for igual recebe 1
                break;
            }
        }
        //se aux == 1, não entra no while
        if(!aux)
        {
            Clientes_desordenados[cont] = num;
            cont++;
        }
    }

    //adicionar vetor de numeros do arquivo
    TCliente * sh;
    char text [4];
    for(int i = 0; i < 5000; i++)
    {
        //char auxiliar = sprintf(text, "%d", i);
        //char nome_proprio [] = {"nome_proprio"};
        //strcat(nome_proprio,nome_proprio);

        sh = cliente(Clientes_desordenados[i],"xxx", "cpf", "da/ta/nasc");
        salva_cliente(sh, out);
    }

    free(sh);
}

void le_clientelas(FILE *in)
{
    printf("\n\nLendo clientelas do arquivo...\n\n");
    rewind(in);
    TCliente *sh;
    while ((sh = le_cliente(in)) != NULL)
    {
        imprime_cliente(sh);
        free(sh);
    }
}
//-----------------------------------

*/
void  Inserir_ingressos(FILE *out)
{
    int cont = 0;
    int Ingressos_desordenados[5000];
    while(cont < 5000)
    {
        //rand - gera valores aleatorios
        int num = rand();
        int aux = 0;
        for(int i = 0; i < cont; i++)
        {
            //confere resultado da função rand
            if(Ingressos_desordenados[i] == num)
            {
                aux = 1; //Se for igual recebe 1
                break;
            }
        }
        //se aux == 1, não entra no while
        if(!aux)
        {
            Ingressos_desordenados[cont] = num;
            cont++;
        }
    }

    //adicionar vetor de numeros do arquivo
    TIng * sh;
    //TCliente * cl;
    for(int i = 0; i < 5000; i++)
    {
        sh = ingresso(Ingressos_desordenados[i],"xxxx", "XXXXXXXXXXX", "ZZ/ZZ/ZZZZ", rand());
        salva(sh, out);
    }

    free(sh);
}

void le_ingressos(FILE *in)
{
    printf("\n\nLendo ingressos do arquivo...\n\n");
    rewind(in);
    TIng *sh;
    while ((sh = le(in)) != NULL)
    {
        imprime(sh);
        free(sh);
    }
}

int main()
{
    //cria ponteiro para arq
    FILE *out;
    int chave;
    TIng *t;
    //TCliente * c;

    //auxiliares
    int codigof, comparacao;
    double tempoGasto = 0.0;

    //Abre arquivo
    if ((out = fopen("ingresso.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else
    {
        //insere ingressos no prog
        Inserir_ingressos(out);


        //int opcao;
        //int aux=0;
        //int menu;

    /*
    //Abre arquivo
    if ((out = fopen("cliente.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else
    {
        //insere ingressos no prog
        Inserir_clientes(out);
    }
    */
        int opcao;
        int aux=0;
        int menu;


        printf("\nGERENCIAMENTO DE INGRESSOS\n");


        while(menu != 5)
        {

            printf("\n1 - Lista completa dos ingressos");
            printf("\n2 - Buscar algum ingresso - busca sequencial");
            printf("\n3 - Inserction Sort");
            printf("\n4 - Buscar algum ingresso - busca binaria");
            printf("\n5 - Sair do programa");
            printf("\n");
            printf("\nDigite a opcao:");
            scanf("%d",&opcao);

            switch (opcao)
            {
            case 1:
                printf("\n\n------------------------------LISTA DE INGRESSOS------------------------------n");

                //LE OS INGRESSOS INSERIDOS
                //le_clientelas(out);
                le_ingressos(out);
                break;

            case 2:

                printf("\nDigite o codigo do ingresso que deseja encontrar: ");
                scanf("%i", &codigof);
                clock_t begin = clock();
                TIng *ingres = buscaSequencial(codigof, out, &comparacao);
                clock_t end = clock();
                tempoGasto = (double)(end - begin) / CLOCKS_PER_SEC;

                if (ingres == NULL)
                {
                    printf("\nIngresso nao encontrado.\n");
                    printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                }
                else
                {
                    printf("\nIngresso encontrado: \n");
                    imprime(ingres);
                    printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                    //Salvar tempo gasto em um arquivo
                    salvarTempoGasto(tempoGasto);

                }
                break;

            case 3:
                tempoGasto = 0.0;
                printf("\nOrdenando com o metodo de ordenacao InsertionSort..\n");
                clock_t comeco = clock();
                insertion_sort_disco(out, tamanho_arquivo(out));
                clock_t fim = clock();
                tempoGasto = (double)(fim - comeco) / CLOCKS_PER_SEC;
                imprime_arquivo(out);
                printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);
                //Salvar tempo gasto em um arquivo
                salvarTempoGasto(tempoGasto);
                break;
            case 4:
                //busca binaria
                printf("\nOrdenado com sucesso! \n------------------------------Busca binaria:------------------------------\n");
                printf("\n\nDigite o codgido do ingresso que deseja encontrar: ");
                fflush(stdin);
                scanf("%d", &chave);
                fflush(stdin);


                clock_t comecoo = clock();
                t = busca_binaria(chave, out, 0, tamanho_arquivo(out)-1);
                imprime(t);
                clock_t fimm = clock();
                tempoGasto = (double)(fimm - comecoo) / CLOCKS_PER_SEC;
                printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);
                //Salvar tempo gasto em um arquivo
                salvarTempoGasto(tempoGasto);

                break;
            case 5:
                menu = 5;
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
        printf("\nPROGRAMA FINALIZADO.\n");

        //fecha arquivos
        fclose(arquivoTempos);
        fclose(out);
        free(t);
    }
}
