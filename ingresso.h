#ifndef INGRESSO_H_INCLUDED
#define INGRESSO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


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

void gerarParticoesOrdenadas(const char *nomeArquivo);

void intercalacao_otima();

int compare(const void *a, const void *b);
// Função para ler um ingresso do arquivo
int lerIngresso(FILE *arquivo, TIng *ingresso);

// Função para escrever um ingresso no arquivo
void escreverIngresso(FILE *arquivo, TIng *ingresso);
#endif // INGRESSO_H_INCLUDED
