#include"particoes.h"
#include <stdio.h>
#include <stdlib.h>

Lista *cria(char *nome, Lista *prox){
	Lista *nomes = (Lista *) malloc(sizeof(Lista));
	nomes->nome = nome;
	nomes->prox = prox;
	return nomes;
}
