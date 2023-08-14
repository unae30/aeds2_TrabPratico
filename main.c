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
        //Abre o arquivo para escrita em modo "append" (adicionar ao final do arquivo)
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
void  Inserir_clientes_ingressos(FILE *out, FILE * out1)
{
    int cont = 0;
    int Clientes_desordenados[5000];
    int Ingressos_desordenados[5000];
    while(cont < 5000)
    {
        //rand - gera valores aleatorios
        int num = rand();
        int num1 = rand();
        int aux = 0;
        for(int i = 0; i < cont; i++)
        {
            //confere resultado da função rand
            if(Clientes_desordenados[i] == num && Ingressos_desordenados[i] == num1)
            {
                aux = 1; //Se for igual recebe 1
                break;
            }
        }
        //se aux == 1, não entra no while
        if(!aux)
        {
            Clientes_desordenados[cont] = num;
            Ingressos_desordenados[cont] = num1;
            cont++;
        }
    }

    //adicionar vetor de numeros do arquivo
    TCliente * sh;
    TIng * ch;
    for(int i = 0; i < 5000; i++)
    {
        //vinculando entidades clientes e ingressos
        sh = cliente(Clientes_desordenados[i],"nome_cliente", "cpf", "da/ta/nasc");
        salva_cliente(sh, out);

        ch = ingresso(Ingressos_desordenados[i],sh->nome, "XXX.XXX.XXX-XX", "da/ta/show", rand());
        salva(ch, out1);
    }

    free(sh);
    free(ch);
}

void le_clientelas(FILE *in)
{
    printf("\n\nLendo clientelas e ingressos do arquivo...\n\n");
    rewind(in);
    TCliente *sh;

    while ((sh = le_cliente(in)) != NULL)
    {
        imprime_cliente(sh);
        free(sh);
    }
}

void le_ingressos(FILE *in)
{
    printf("\n\nLendo ingressos do arquivo...\n\n");
    rewind(in);
    TIng *ch;
    while ((ch = le(in)) != NULL)
    {
        imprime(ch);
        free(ch);
    }
}

int main()
{
    //cria ponteiro para arq
    FILE *out;
    FILE *out1;
    int chave;
    TIng *t;
    TCliente * c;

    //auxiliares
    int codigof, comparacao;
    double tempoGasto = 0.0;


    //classificação interna particoes
    int nParticoes, nElementos;
    //Lista *nomes = NULL;
    //Lista *prox;
    int nIng=5000;

    //Abre arquivo
    if ((out = fopen("ingresso.dat", "w+b")) == NULL || (out1 = fopen("clientes.dat", "w+b")) ==NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else
    {
        //insere ingressos no prog

        Inserir_clientes_ingressos(out1, out);

        int opcao;
        int aux=0;
        int menu;


        printf("\nGERENCIAMENTO DE INGRESSOS e CLIENTES\n");


        while(menu != 9)
        {

            printf("\n1 - Lista completa dos ingressos");
            printf("\n2 - Buscar algum ingresso - busca sequencial");
            printf("\n3 - Inserction Sort ingressos");
            printf("\n4 - Buscar algum ingresso - busca binaria");
            printf("\n5 - Lista completa dos clientes");
            printf("\n6 - Buscar algum cliente - busca sequencial");
            printf("\n7 - Inserction Sort cliente");
            printf("\n8 - Buscar algum cliente - busca binaria");
            printf("\n9 - Selecao natural e arvore bin de 3 venc");
            printf("\n10 - Sair do programa");
            printf("\n");
            printf("\nDigite a opcao:");
            scanf("%d",&opcao);

            int numeroDeParticao = 0;
            switch (opcao)
            {
            case 1:
                printf("\n\n------------------------------LISTA DE INGRESSOS------------------------------n");

                //LE OS INGRESSOS INSERIDOS
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
                printf("\n\n------------------------------LISTA DE CLIENTES------------------------------n");

                //LE OS clientes INSERIDOS
                le_clientelas(out1);
                break;
            case 6:
                printf("\nDigite o codigo do cliente que deseja encontrar: ");
                scanf("%i", &codigof);
                clock_t begin1 = clock();
                TCliente *cliente = buscaSequencial_cliente(codigof, out1, &comparacao);
                clock_t end1 = clock();
                tempoGasto = (double)(end - begin) / CLOCKS_PER_SEC;

                if (cliente == NULL)
                {
                    printf("\Cliente nao encontrado.\n");
                    printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                }
                else
                {
                    printf("\Cliente encontrado: \n");
                    imprime(cliente);
                    printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                    //Salvar tempo gasto em um arquivo
                    salvarTempoGasto(tempoGasto);

                }
                break;
            case 7:
                tempoGasto = 0.0;
                printf("\nOrdenando com o metodo de ordenacao InsertionSort..\n");
                clock_t comeco1 = clock();
                insertion_sort_disco_cliente(out1, tamanho_arquivo(out1));
                clock_t fim1 = clock();
                tempoGasto = (double)(fim - comeco) / CLOCKS_PER_SEC;
                imprime_arquivo_cliente(out1);
                printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);
                //Salvar tempo gasto em um arquivo
                salvarTempoGasto(tempoGasto);
                break;
            case 8:
                //busca binaria
                printf("\n\n------------------------------Busca binaria:------------------------------\n");
                printf("\n\nDigite o codgido do cliente que deseja encontrar: ");
                fflush(stdin);
                scanf("%d", &chave);
                fflush(stdin);
                clock_t comecoo1 = clock();
                t = busca_binaria_cliente(chave, out, 0, tamanho_arquivo(out)-1);
                imprime_cliente(t);
                clock_t fimm1 = clock();
                tempoGasto = (double)(fimm - comecoo) / CLOCKS_PER_SEC;
                printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);
                //Salvar tempo gasto em um arquivo
                salvarTempoGasto(tempoGasto);
                break;

            case 9:
                /*
                                int numberOfPartition = 1;
                numeroDeParticao += naturalSelection(out,"naturalSelectionPartition");
                binaryTreeOfWinners(numeroDeParticao, "naturalSelectionPartition");
                FILE *fileBinaryOfTreeSorted = fopen("binaryOfTreeFileSorted.dat","rb+");
                printPartitionEmployeeID(fileBinaryOfTreeSorted, "binaryOfTreeFileSorted.dat");
                fclose(fileBinaryOfTreeSorted);
                */
                // int numberOfPartition = 0;
                numeroDeParticao = substitutionSelection(out, "substitutionSelectionPartition");
                mergeSort(numeroDeParticao, "substitutionSelectionPartition");
                break;
            case 10:
                menu = 9;
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
        fclose(out1);
        free(t);
    }
}
