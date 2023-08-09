#include "ingresso.h"
#include "particoes.c"
#include "pilha.c"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

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


void classificacaoInterna(FILE *arq, int M) {

    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;
    int nIng = tamanho_arquivo(arq); nIng
    int tamanho_arquivo = 0;
    int t = tamanho();
    char nome[10];
    char numero[5];
    char extensao[5];

        //itoa(qtdParticoes(tamanho_arquivo),numero,10);

    while (reg != nIng) {
        //le o arquivo e coloca no vetor
        TIng *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho(), SEEK_SET);
            v[i] = le(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TIng *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        printf("\n\nParticao %d",qtdParticoes);
        strcpy(nome,"particao");
        strcpy(extensao,".dat");
        itoa(qtdParticoes,numero,10);
        strcat(nome,numero);
        strcat(nome,extensao);

        //printf("\n%s\n", nome);

        FILE *p;

        if ((p = fopen(nome, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho(), SEEK_SET);
                salva(v[i], p);
                imprime(v[i]);
            }
            fclose(p);
            qtdParticoes++;
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
    //nParticoes = qtdParticoes;
}


void intercalacao_basico(FILE *out, int num_p) {

    char nome[10];
    char numero[5];
    char extensao[5];

                    //criando pilha com o conteudo das particoes geradas
                    TPilha **vetPilhas;
                    vetPilhas = (TPilha **) malloc(sizeof(TPilha *) * (num_p));
                    int *vetTopo;
                    vetTopo = (int *) malloc(sizeof(int) * (num_p));

                    for(int p = 0 ; p < num_p ; p++){
                        //alocando a memÃ³ria para salvar uma pilha para cada particao
                        vetPilhas[p] = (TPilha *) malloc(sizeof(TPilha) * (8));
                        //abrindo primeira partiÃ§Ã£o para leitura
                        strcpy(nome,"particao");
                        strcpy(extensao,".dat");
                        itoa(p,numero,10);
                        strcat(nome,numero);
                        strcat(nome,extensao);
                        FILE *part = fopen(nome, "rb+");
                        //printf("%s \n", "Particao 0");
                        //nomes = nomes->prox;
                        //imprimindo os funcionÃ¡rios gravados nas partiÃ§Ãµes
                        //imprimirBase(part);
                        //inicializando os topos das pilhas
                        vetTopo[p] = -1;
                        //criando pilha atravÃ©s da leitura da partiÃ§Ã£o
                        cria_pilha_particao(vetPilhas[p], part, 8, &vetTopo[p]);
                        //fechando a partiÃ§Ã£o
                        fclose(part);
                    }
                    //crioupilhas = 1;
                    //nomes = prox;

    int fim = 0; //variavel que controla fim do procedimento



        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            strcpy(nome,"particao");
            strcpy(extensao,".dat");
            itoa(i,numero,10);
            strcat(nome,numero);
            strcat(nome,extensao);

            //printf("%s",nome);

            v[i].f = fopen(nome, "rb");
            v[i].aux_p = 0;

            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho(), SEEK_SET);
                TIng *f = le(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].ing = ingresso(INT_MAX, "","","",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].ing = f;
                }
            }
            else {
                fim = 1;
            }
            //nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].ing->cod < menor){
                    menor = v[i].ing->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saÃ­da
                fseek(out, aux * tamanho(), SEEK_SET);
                salva(v[pos_menor].ing, out);
                //printf("%d ",pos_menor);
                //atualiza posiÃ§Ã£o pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho(), SEEK_SET);
                TFunc *f = le(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiÃ§ao do vetor
                    v[pos_menor].ing = ingresso(INT_MAX, "", "", "",0.0);
                }
                else {
                    v[pos_menor].ing = f;
                }

            }
        }

        //fecha arquivos das partiÃ‡Ãµes de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        //    free(v[i].ing);
        }
        //fecha arquivo de saÃ­da
        //fclose(out);

}












































/*
//Método por substituição
void particoes_selecao_substituicao(FILE *in, Lista *nome_arquivos_saida, int M, int nIngr) {
    rewind(in); // posiciona cursor no início do arquivo

    int reg = 0;

    while (reg < nIngr) {
        TIng *v[M];
        int i = 0;
        while (!feof(in)) {
            fseek(in, (reg) * tamanho(), SEEK_SET);
            v[i] = le(in);
            i++;
            reg++;
            if (i >= M) break;
        }

        // faz ordenação por seleção por substituição
        for (int j = 0; j < M; j++) {
            int pos_menor = j;
            for (int k = j + 1; k < M; k++) {
                if (v[k]->cod < v[pos_menor]->cod) {
                    pos_menor = k;
                }
            }

            TIng *temp = v[j];
            v[j] = v[pos_menor];
            v[pos_menor] = temp;
        }

        // cria arquivo de partição e faz gravação
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro ao criar arquivo de saída\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho(), SEEK_SET);
                salva(v[i], p);
                imprime(v[i]);
            }
            fclose(p);
        }

        for (int jj = 0; jj < M; jj++)
            free(v[jj]);
    }
}

//----------------INTERCALAÇÃO-------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// Função de intercalação ótima
void intercalacao_otima(Lista *nomes, int nParticoes, FILE *saida) {
    FILE **particoes = (FILE **)malloc(nParticoes * sizeof(FILE *));
    TIng **registros = (TIng **)malloc(nParticoes * sizeof(TIng *));

    // Abre os arquivos das partições e carrega o primeiro registro de cada uma
    int i;
    for (i = 0; i < nParticoes; i++) {
        particoes[i] = fopen(nomes->nome, "rb");
        registros[i] = le(particoes[i]);
        nomes = nomes->prox;
    }

    // Realiza a intercalação ótima
    while (1) {
        int menor = -1;
        TIng *menorRegistro = NULL;

        // Encontra o menor registro entre os registros atuais de todas as partições
        for (i = 0; i < nParticoes; i++) {
            if (registros[i]) {
                if (menor == -1 || registros[i]->cod < menorRegistro->cod) {
                    menor = i;
                    menorRegistro = registros[i];
                }
            }
        }

        if (menor == -1) {
            // Não há mais registros em nenhuma partição, intercalação finalizada
            break;
        }

        // Escreve o menor registro no arquivo de saída
        salva(menorRegistro, saida);

        // Lê o próximo registro da partição cujo registro foi escolhido
        registros[menor] = le(particoes[menor]);
    }

    // Fecha todos os arquivos das partições
    for (i = 0; i < nParticoes; i++) {
        fclose(particoes[i]);
    }

    // Libera a memória alocada
    free(particoes);
    free(registros);
}*/



