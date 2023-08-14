#ifndef INGRESSO_H_INCLUDED
#define INGRESSO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

typedef struct Ingresso
{
  int cod;
  char nome[50];
  char show[15];
  char data_show[11];
  double preco;
} TIng;

//estrutura para armazenar o vetor de particoes
typedef struct vetor{
    TIng *ing;
    int aux_p;
    long init_p;
    long end_p;
    FILE *f;
}TVet;


//Imprime ingresso
void imprime(TIng *ing);

//Cria ingresso.
TIng *ingresso(int cod, char *nome, char *show, char *data_show, double preco);

//Salva ingresso no out, na posicao atual do cursor
void salva(TIng *ing, FILE *out);

//Le ingresso do arquivo in, na posicao atual do cursor
TIng *le(FILE *in);

//Retorna tamanho do ingresso em bytes
int tamanho();

//Retorna tamanho do arquivo
int tamanho_arquivo(FILE *arq);

int sizeFile(FILE *file, int contSizeFile);

//Faz a busca sequencial
TIng *buscaSequencial(int id, FILE *out, int *comparacao);

//Faz a ordenação insertionSort
void insertion_sort_disco(FILE *out, int tam);

//Imprime arquivo
void imprime_arquivo(FILE *out);

//Faz busca binária
TIng *busca_binaria(int chave, FILE *in, int inicio, int fim);


void classificacaoInterna(FILE *in, int M);

void intercalacao_basico(FILE *out, int num_p);


int compare(const void *a, const void *b);
// Função para ler um ingresso do arquivo
int lerIngresso(FILE *arquivo, TIng *ingresso);

// Função para escrever um ingresso no arquivo
void escreverIngresso(FILE *arquivo, TIng *ingresso);

int naturalSelection(FILE *file, char nameFilePartition[]);
void saveRegisterIngresso(TIng *ing, FILE *out);
TIng *readRegisterIngresso(FILE *out);
void printPartitionEmployeeID(FILE *file, char partitionName[]);
void binaryTreeOfWinners(int numberOfPartition, char nameFilePartition[]);
void treeWinner(TIng **ing, FILE *file, int sizeInTreeOfWinners, int *auxTreeWinner);
void mergeSort(int numberOfPartition, char nameFilePartition[]);
int substitutionSelection (FILE *file, char nameFilePartition[]);
int allVetFrozen (int vet[]);
#endif // INGRESSO_H_INCLUDED
