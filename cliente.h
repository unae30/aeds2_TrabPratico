#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


typedef struct Cliente
{
  int cod_id;
  char nome[50];
  char cpf[15];
  char data_nasc[11];
} TCliente;


//Imprime cliente
void imprime_cliente(TCliente *cliente);

//Cria cliente.
TCliente *cliente(int cod_id, char *nome, char *cpf, char *data_nasc);

//Salva cliente no out, na posicao atual do cursor
void salva_cliente(TCliente *cliente, FILE *out);

//Le cliente do arquivo in, na posicao atual do cursor
TCliente *le_cliente(FILE *in);

//Retorna tamanho do cliente em bytes
int tamanho_cliente();

//Retorna tamanho do arquivo
int tamanho_arquivo_cliente(FILE *arq);

//Faz a busca sequencial
TCliente *buscaSequencial_cliente(int id, FILE *out, int *comparacao);

//Faz a ordenação insertionSort
void insertion_sort_disco_cliente(FILE *out, int tam);

//Imprime arquivo
void imprime_arquivo_cliente(FILE *out);

//Faz busca binária
TCliente *busca_binaria_cliente(int chave, FILE *in, int inicio, int fim);


#endif // CLIENTE_H_INCLUDED
