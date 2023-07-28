#ifndef PARTICOES_H_INCLUDED
#define PARTICOES_H_INCLUDED

typedef struct Lista {
    char *nome;
    struct Lista *prox;
} Lista;

Lista *cria(char *nome, Lista *prox);


#endif // PARTICOES_H_INCLUDED
