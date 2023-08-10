#include "ingresso.h"
#include "particoes.c"
#include "pilha.c"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <dirent.h>

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

//Método que retorna o tamanho de ingresso em bytes
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

TIng *buscaSequencial(int id, FILE *out, int *comparacao)
{
    int position = 0;

    rewind(out);

    while (!feof(out))
    {

        fseek(out, position * sizeof(TIng), SEEK_SET);
        TIng *ing = le(out);

        position++;

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

//-----------------------------PARTIÇÔES--------------------------------------------

#define TAM_MAX 100
#define F 49 // Defina o número máximo de partições aqui

// Função de comparação para o qsort
int compare(const void *a, const void *b)
{
    return ((TIng *)a)->cod - ((TIng *)b)->cod;
}

// Função para ler um ingresso do arquivo
int lerIngresso(FILE *arquivo, TIng *ingresso)
{
    return fread(ingresso, sizeof(TIng), 1, arquivo);
}

// Função para escrever um ingresso no arquivo
void escreverIngresso(FILE *arquivo, TIng *ingresso)
{
    fwrite(ingresso, sizeof(TIng), 1, arquivo);
}

// Função para gerar partições ordenadas por seleção por substituição
void gerarParticoesOrdenadas(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    int numParticao = 0;
    TIng buffer[TAM_MAX];
    int tamBuffer = 0;

    while (1)
    {
        int lidos = lerIngresso(arquivo, &buffer[tamBuffer]);
        if (lidos == 0)
        {
            if (tamBuffer > 0)
            {
                // Ordenar e escrever a partição final
                qsort(buffer, tamBuffer, sizeof(TIng), compare);

                char nomeParticao[20];
                snprintf(nomeParticao, sizeof(nomeParticao), "particao%d.dat", numParticao);

                FILE *particao = fopen(nomeParticao, "wb");
                for (int i = 0; i < tamBuffer; i++)
                {
                    escreverIngresso(particao, &buffer[i]);
                }
                fclose(particao);

                numParticao++;
            }
            break; // Fim do arquivo
        }

        tamBuffer++;
        if (tamBuffer == TAM_MAX)
        {
            // Ordenar e escrever a partição atual
            qsort(buffer, tamBuffer, sizeof(TIng), compare);

            char nomeParticao[20];
            snprintf(nomeParticao, sizeof(nomeParticao), "particao%d.dat", numParticao);

            FILE *particao = fopen(nomeParticao, "wb");
            for (int i = 0; i < tamBuffer; i++)
            {
                escreverIngresso(particao, &buffer[i]);
            }
            fclose(particao);

            numParticao++;
            tamBuffer = 0;
        }
    }

    fclose(arquivo);
}

void intercalacao_otima() {
    FILE *particoes[F];
    char nomeParticao[F][20];
    TIng buffer[F][TAM_MAX];
    int tamBuffer[F] = {0};

    // Abrir as partições iniciais
    for (int i = 0; i < F; i++) {
        snprintf(nomeParticao[i], sizeof(nomeParticao[i]), "particao%d.dat", i);
        particoes[i] = fopen(nomeParticao[i], "rb");
        if (particoes[i] == NULL) {
            perror("Erro ao abrir partição");
            exit(1);
        }

        tamBuffer[i] = fread(buffer[i], sizeof(TIng), TAM_MAX, particoes[i]);
        qsort(buffer[i], tamBuffer[i], sizeof(TIng), compare);
    }

    char nomeSaida[] = "saida.dat";
    FILE *saida = fopen(nomeSaida, "wb");
    if (saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        exit(1);
    }

    int numParticao = F;

    while (numParticao > 0) {
        TIng menorValor = buffer[0][0];
        int indiceMenor = 0;

        for (int i = 1; i < F; i++) {
            if (tamBuffer[i] > 0 && compare(&buffer[i][0], &menorValor) < 0) {
                menorValor = buffer[i][0];
                indiceMenor = i;
            }
        }

        int numEscritos = fwrite(&menorValor, sizeof(TIng), 1, saida);
        if (numEscritos < 1) {
            perror("Erro ao escrever arquivo de saída");
            exit(1);
        }

        tamBuffer[indiceMenor]--;
        memmove(&buffer[indiceMenor][0], &buffer[indiceMenor][1], tamBuffer[indiceMenor] * sizeof(TIng));

        if (tamBuffer[indiceMenor] == 0) {
            fclose(particoes[indiceMenor]);
            numParticao--;
            for (int j = indiceMenor; j < F - 1; j++) {
                particoes[j] = particoes[j + 1];
                memcpy(buffer[j], buffer[j + 1], TAM_MAX * sizeof(TIng));
                tamBuffer[j] = tamBuffer[j + 1];
            }
        }
    }

    fclose(saida);

    for (int i = 0; i < F; i++) {
        remove(nomeParticao[i]);
    }
}
