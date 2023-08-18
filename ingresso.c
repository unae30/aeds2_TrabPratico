#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "ingresso.h"

//Imprime ingresso
void imprime(TIng *ing)
{
    printf("\n");
    printf("\nCodigo do ingresso ");
    printf("%d", ing->cod);
    printf("\nNome: ");
    printf("%s", ing->nome);
    printf("\nShow: ");
    printf("%s", ing->show);
    printf("\nData do show: ");
    printf("%s", ing->data_show);
    printf("\nPreco:R$ ");
    printf("%4.2f", ing->preco);
    printf("\n");
}

// irá criar um ingresso
TIng *ingresso(int cod, char *nome, char *show, char *data_show, double preco)
{
    TIng *ing = (TIng *)malloc(sizeof(TIng));

    // inicializa espaço de memória com ZEROS
    if (ing)
        memset(ing, 0, sizeof(TIng));

    // Salva os dados nos campos que a estrutura ingresso tem
    ing->cod = cod;
    strcpy(ing->nome, nome);
    strcpy(ing->show, show);
    strcpy(ing->data_show, data_show);
    ing->preco = preco;
    return ing;
}
//Salva o ingresso no arquivo (out)

void salva(TIng *ing, FILE *out)
{
    //Irá salvar na posicao atual do cursor
    fwrite(&ing->cod, sizeof(int), 1, out);
    fwrite(ing->nome, sizeof(char), sizeof(ing->nome), out);
    fwrite(ing->show, sizeof(char), sizeof(ing->show), out);
    fwrite(ing->data_show, sizeof(char), sizeof(ing->data_show), out);
    fwrite(&ing->preco, sizeof(double), 1, out);
}

TIng *le(FILE *in)
{
    TIng *ing = (TIng *)malloc(sizeof(TIng));


    if (0 >= fread(&ing->cod, sizeof(int), 1, in)) // "fread" foi utilizado aqui por ser umm comando em C que ler dados de um arq binário
    {
        free(ing);
        return NULL;
    }
    fread(ing->nome, sizeof(char), sizeof(ing->nome), in);
    fread(ing->show, sizeof(char), sizeof(ing->show), in);
    fread(ing->data_show, sizeof(char), sizeof(ing->data_show), in);
    fread(&ing->preco, sizeof(double), 1, in);
    return ing;
}


void imprime_arquivo(FILE *arq)
{

    rewind(arq); // posiciona cursor no inicio do arquivo
    TIng *i = le(arq); // le o arquivo e o coloca no vetor



    // feof retorna um valor diferente de zero se uma operação de leitura tentou ler após o final do arquivo
    while (!feof(arq))
    {
        imprime(i);
        i = le(arq);
    }
}


void printPartitionEmployeeID(FILE *file, char nomeDaParticao[])
{

    printf("\nIDs Ingressos of partition %s: \n --->  ", nomeDaParticao);

    for (int i = 0; i < sizeFile(file, 0); ++i)
    {

        fseek(file, i * sizeof(TIng), SEEK_SET);
        TIng *aux = readRegisterIngresso(file);

        printf(" %i ", aux->cod);
    }

    printf("\n");
}

int tamanho()
{
    return sizeof(int)         // codigo
           + sizeof(char) * 50 // nome do comprador
           + sizeof(char) * 15 // Nome do Show
           + sizeof(char) * 11 // data do  show
           + sizeof(double);   // valor do ingresso
}

int tamanho_arquivo(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = round((double)ftell(arq) / tamanho());
    return tam;
}
int sizeFile(FILE *file, int contTamanhoArquivo)
{

    int bytesAUX = 0;

    while(!feof(file))
    {

        fseek(file, bytesAUX * sizeof(TIng), SEEK_SET);

        TIng *aux = readRegisterIngresso(file);
        if(aux != NULL)
        {
            contTamanhoArquivo++;
        }

        bytesAUX++;
    }

    return contTamanhoArquivo;
}

TIng *buscaSequencial(int id, FILE *out, int *comparacao)
{
    int posicao = 0;

    rewind(out);

    while (!feof(out))
    {

        fseek(out, posicao * sizeof(TIng), SEEK_SET);
        TIng *ing = le(out);

        posicao++;

        if (ing == NULL)
        {
            *comparacao += 1;
            return NULL;
        }
        else if (id == ing->cod)
        {
            *comparacao += 1;
            return ing;
        }
        else
        {
            *comparacao += 1;
        }
    }
    return NULL;
}

void insertion_sort_disco(FILE *out, int tam)
{
    int i;
    // Realiza insertion sort
    for (int j = 2; j <= tam; j++)
    {
        // posiciona o arquivo no registro j
        fseek(out, (j - 1) * tamanho(), SEEK_SET);
        TIng *ij = le(out);

        i = j - 1;
        // posiciona o cursor no registro i
        fseek(out, (i - 1) * tamanho(), SEEK_SET);
        TIng *ii = le(out);

        while ((i > 0) && (ii->cod > ij->cod))
        {
            // posiciona o cursor no registro i+1
            fseek(out, i * tamanho(), SEEK_SET);

            salva(ii, out);
            i = i - 1;
            // leitura do registro i
            fseek(out, (i - 1) * tamanho(), SEEK_SET);
            ii = le(out);
//
        }
        // posiciona cursor no registro i + 1
        fseek(out, (i)*tamanho(), SEEK_SET);

        // salva registro j na posição i
        salva(ij, out);
    }
    // descarrega o buffer para ter certeza que dados foram gravados
    fflush(out);
}

//busca binaria
TIng *busca_binaria(int chave, FILE *in, int inicio, int fim)
{
    TIng *s = NULL;
    int cod = -1;
    int contador=0;

    while (inicio <= fim && cod != chave)
    {
        int meio = trunc((inicio + fim) / 2);


        contador++;
        printf("Inicio: %d; Fim: %d; Meio: %d; comparacao numero: %d\n", inicio, fim, meio,contador);


        fseek(in, (meio -1) * tamanho(), SEEK_SET);
        s = le(in);
        cod = s->cod;
        if (s)
        {
            if (cod > chave)
            {
                fim = meio - 1;
            }
            else
            {
                inicio = meio + 1;
            }
        }
    }
    if (cod == chave)
    {
        return s;
    }
    else return NULL;
}

TIng *readRegisterIngresso(FILE *out)
{
    TIng *ing = (TIng*) malloc (sizeof(TIng));

    if ( 0 >= fread(&ing->cod, sizeof(int), 1, out))
    {
        free(ing);
        return NULL;
    }

    fwrite(&ing->cod, sizeof(int), 1, out);
    fwrite(ing->nome, sizeof(char), sizeof(ing->nome), out);
    fwrite(ing->show, sizeof(char), sizeof(ing->show), out);
    fwrite(ing->data_show, sizeof(char), sizeof(ing->data_show), out);
    fwrite(&ing->preco, sizeof(double), 1, out);

    return ing;
}


void saveRegisterIngresso(TIng *ing, FILE *out)
{
    fwrite(&ing->cod, sizeof(int), 1, out);
    fwrite(ing->nome, sizeof(char), sizeof(ing->nome), out);
    fwrite(ing->show, sizeof(char), sizeof(ing->show), out);
    fwrite(ing->data_show, sizeof(char), sizeof(ing->data_show), out);
    fwrite(&ing->preco, sizeof(double), 1, out);
}

//substituição

int todoVetCongelado (int vet[6]) {

    int cont = 0;

    for (int i = 0; i < 6; ++i) {
        if (vet[i] == 1) {
            cont++;
        }
    }

    if (cont == 6) {
        return 1;
    } else {
        return 0;
    }
}

int substituicaoPorSelecao (FILE *file, char nomeDaParticaoFinal[]) {

    int numeroDeParticao = 0, contTamanhoArquivo = 0, posicao = 6, posicaoMenorElemento = 0, menorElemento = 999999, tamArquivoAux = 0, posicaoSelecionada = 0;
    struct Ingresso ingresso[6];
    int auxVetIng [6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    tamArquivoAux = sizeFile(file, contTamanhoArquivo);


    printf("\nExecutando selecao por substituicao...");

    for (int i = 0; i < 6; ++i) {
        fseek(file, i * sizeof(TIng), SEEK_SET);
        //Ler 6 registros do arquivo para a memória
        TIng *aux = readRegisterIngresso(file);
        ingresso[i] = *aux;

    }

    for (int i = 0; i < 6; ++i) {
        //para a memória
        auxVetIng[i] = ingresso[i].cod;
    }

    while (posicao != tamArquivoAux) {

        char nomeDaParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(numeroDeParticao, str1, 10);
        strcat(strcpy(nomeDaParticao, nomeDaParticaoFinal), str1);
        strcat(strcpy(nomeDaParticao, nomeDaParticao), str2);

        FILE *particaoDoArquivo = fopen(nomeDaParticao, "wb+");

        int VetCongeladoAuxiliar[6] = {0, 0, 0, 0, 0, 0,};

        while (posicao != tamArquivoAux) {

            menorElemento = 9999999;

            for (int i = 0; i < 6; ++i) {

                if (menorElemento > auxVetIng[i] && VetCongeladoAuxiliar[i] != 1) {
                    //Selecionar, no array em memória, o registro r com menor chave
                    menorElemento = auxVetIng[i];
                    posicaoMenorElemento = i;
                }
            }
            //Gravar o registro r na partição de saída
            saveRegisterIngresso(&ingresso[posicaoMenorElemento], particaoDoArquivo);

            fseek(file, posicao * sizeof(TIng), SEEK_SET);

            TIng *aux = readRegisterIngresso(file);

            posicao++;
            //Substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada
            auxVetIng[posicaoMenorElemento] = aux->cod;
            ingresso[posicaoMenorElemento] = *aux;

            //Caso a chave deste último seja menor do que a chave recém gravada,
            //considerá-lo congelado e ignorá-lo no restante do processamento
            if (aux->cod < menorElemento) {
                VetCongeladoAuxiliar[posicaoMenorElemento] = 1;
            }

            if(todoVetCongelado(VetCongeladoAuxiliar) == 1) {
                numeroDeParticao++;
                break;
            }

        }

        //fechar a partição de saída
        fclose(particaoDoArquivo);

        if (posicao >= tamArquivoAux) {
            break;
        }

    }

    char nomeDaParticao[100];
    char str1[100];
    char str2[100] = ".dat";

    itoa(numeroDeParticao, str1, 10);
    strcat(strcpy(nomeDaParticao, nomeDaParticaoFinal), str1);
    strcat(strcpy(nomeDaParticao, nomeDaParticao), str2);

    //abrir nova particao de saida
    FILE *ParticaoFinalDoArquivo = fopen(nomeDaParticao, "ab+");

    int k, j;

    struct Ingresso ingAux;

    for (k = 1; k < 6; k++) {

        for (j = 0; j < 6 - 1; j++) {

            if (ingresso[j].cod > ingresso[j + 1].cod) {
                ingAux = ingresso[j];
                ingresso[j] = ingresso[j + 1];
                ingresso[j + 1] = ingAux;
            }
        }
    }
    //grava a ultima particao
    for (int i = 0; i < 6; ++i) {
        saveRegisterIngresso(&ingresso[i], ParticaoFinalDoArquivo);
    }
    //fechar a particao de saida
    fclose(ParticaoFinalDoArquivo);

    //gerando os arquivos
    for (int i = 0; i <= numeroDeParticao; ++i) {


        itoa(i, str1, 10);
        strcat(strcpy(nomeDaParticao, nomeDaParticaoFinal), str1);
        strcat(strcpy(nomeDaParticao, nomeDaParticao), str2);

        FILE *particaoDoArquivo = fopen(nomeDaParticao, "rb+");

        printPartitionEmployeeID(particaoDoArquivo, nomeDaParticao);

        fclose(particaoDoArquivo);
    }

    return numeroDeParticao;
}

void IntercalacaoOtima(int numeroDeParticao, char nomeDaParticaoFinal[]) {

    int *tamArqParticao = (int *) malloc(numeroDeParticao * sizeof(int));
    int *VetParticaoFinal = (int *) malloc(numeroDeParticao * sizeof(int));
    int *vetPosicaoParticao = (int *) malloc(numeroDeParticao * sizeof(int));
    int *vetValorParticaoIngresso = (int *) malloc(numeroDeParticao * sizeof(int));
    int flagAuxFinal = 0, count, menorElemento = INT_MAX, posicaoMenorElemento = 0;

    FILE *ArquAuxFinal = fopen("intercalacaoOtima.dat", "wb+");

    for (int i = 0; i <= numeroDeParticao; ++i) {

        char nomeDaParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(nomeDaParticao, nomeDaParticaoFinal), str1);
        strcat(strcpy(nomeDaParticao, nomeDaParticao), str2);

        FILE *particaoDoArquivo = fopen(nomeDaParticao, "rb+");

        rewind(particaoDoArquivo);

        tamArqParticao[i] = sizeFile(particaoDoArquivo, 0);
        VetParticaoFinal[i] = 0;
        vetPosicaoParticao[i] = 0;

        fseek(particaoDoArquivo, vetPosicaoParticao[i] * sizeof(TIng), SEEK_SET);
        TIng *auxIng = readRegisterIngresso(particaoDoArquivo);

        vetValorParticaoIngresso[i] = auxIng->cod;

        fclose(particaoDoArquivo);
    }


    while (flagAuxFinal != 1) {

        count = 0;

        for (int i = 0; i <= numeroDeParticao; ++i) {

            if (VetParticaoFinal[i] == 1) {
                count++;
            }

            if (count == numeroDeParticao) {
                flagAuxFinal = 1;
            }
        }

        for (int i = 0; i <= numeroDeParticao; ++i) {

            if (vetValorParticaoIngresso[i] < menorElemento && VetParticaoFinal[i] != 1) {
                menorElemento = vetValorParticaoIngresso[i];
                posicaoMenorElemento = i;
            }
        }

        char nomeDaParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(posicaoMenorElemento, str1, 10);
        strcat(strcpy(nomeDaParticao, nomeDaParticaoFinal), str1);
        strcat(strcpy(nomeDaParticao, nomeDaParticao), str2);

        FILE *particaoDoArquivo = fopen(nomeDaParticao, "rb+");

        rewind(particaoDoArquivo);
        fseek(particaoDoArquivo, vetPosicaoParticao[posicaoMenorElemento] * sizeof(TIng), SEEK_SET);
        TIng *auxIng = readRegisterIngresso(particaoDoArquivo);
        saveRegisterIngresso(auxIng, ArquAuxFinal);
        vetPosicaoParticao[posicaoMenorElemento]++;

        rewind(particaoDoArquivo);

        if (vetPosicaoParticao[posicaoMenorElemento] >= tamArqParticao[posicaoMenorElemento]) {
            VetParticaoFinal[posicaoMenorElemento] = 1;
        } else {
            fseek(particaoDoArquivo, vetPosicaoParticao[posicaoMenorElemento] * sizeof(TIng), SEEK_SET);
            TIng *auxIng2 = readRegisterIngresso(particaoDoArquivo);
            vetValorParticaoIngresso[posicaoMenorElemento] = auxIng2->cod;
        }

        fclose(particaoDoArquivo);

        menorElemento = INT_MAX;

    }

    printPartitionEmployeeID(ArquAuxFinal, "intercalacaoOtima.dat");

    fclose(ArquAuxFinal);
    free(VetParticaoFinal);
    free(tamArqParticao);
    free(vetPosicaoParticao);
    free(vetValorParticaoIngresso);
}
