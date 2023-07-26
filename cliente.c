#include "cliente.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

//Imprime cliente
void imprime_cliente(TCliente *cliente)
{
    printf("\n");
    printf("\nCodigo identificacao do cliente ");
    printf("%d", cliente->cod_id);
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nShow: ");
    printf("%s", cliente->cpf);
    printf("\nData do cpf: ");
    printf("%s", cliente->data_nasc);
    printf("\n");
}

// irá criar um cliente
TCliente *cliente(int cod_id, char *nome, char *cpf, char *data_nasc)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));

    // inicializa espaço de memória com ZEROS
    if (cliente)
        memset(cliente, 0, sizeof(TCliente));

    // Salva os dados nos campos que a estrutura cliente tem
    cliente->cod_id = cod_id;
    strcpy(cliente->nome, nome);
    strcpy(cliente->cpf, cpf);
    strcpy(cliente->data_nasc, data_nasc);
    return cliente;
}
//Salva o cliente no arquivo (out)

void salva_cliente(TCliente *cliente, FILE *out)
{
    //Irá salvar na posicao atual do cursor
    fwrite(&cliente->cod_id, sizeof(int), 1, out);
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(cliente->cpf, sizeof(char), sizeof(cliente->cpf), out);
    fwrite(cliente->data_nasc, sizeof(char), sizeof(cliente->data_nasc), out);
}

TCliente *le_cliente(FILE *in)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));


    if (0 >= fread(&cliente->cod_id, sizeof(int), 1, in)) // "fread" foi utilizado aqui por ser umm comando em C que ler dados de um arq binário
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
    fread(cliente->cpf, sizeof(char), sizeof(cliente->cpf), in);
    fread(cliente->data_nasc, sizeof(char), sizeof(cliente->data_nasc), in);
    return cliente;
}


void imprime_arquivo_cliente(FILE *arq)
{

    rewind(arq); // posiciona cursor no inicio do arquivo
    TCliente *i = le_cliente(arq); // le o arquivo e o coloca no vetor

    // feof retorna um valor diferente de zero se uma operação de leitura tentou ler após o final do arquivo
    while (!feof(arq))
    {
        imprime_cliente(i);
        i = le_cliente(arq);
    }
}

//Método que retorna o tamanho de cliente em bytes
int tamanho_cliente()
{
    return sizeof(int)         // codigo
           + sizeof(char) * 50 // nome do comprador
           + sizeof(char) * 15 // cpf
           + sizeof(char) * 11 // data de nascimento
           + sizeof(double);   // valor do cliente
}

int tamanho_arquivo_cliente(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = round((double)ftell(arq) / tamanho());
    return tam;
}

TCliente *buscaSequencial_cliente(int id, FILE *out, int *comparacao)
{
    int position = 0;

    rewind(out);

    while (!feof(out))
    {

        fseek(out, position * sizeof(TCliente), SEEK_SET);
        TCliente *cliente = le_cliente(out);

        position++;

        if (cliente == NULL)
        {
            *comparacao += 1;
            return NULL;
        }
        else if (id == cliente->cod_id)
        {
            *comparacao += 1;
            return cliente;
        }
        else
        {
            *comparacao += 1;
        }
    }
    return NULL;
}

void insertion_sort_disco_cliente(FILE *out, int tam)
{
    int i;
    // Realiza insertion sort
    for (int j = 2; j <= tam; j++)
    {
        // posiciona o arquivo no registro j
        fseek(out, (j - 1) * tamanho(), SEEK_SET);
        TCliente *ij = le_cliente(out);

        i = j - 1;
        // posiciona o cursor no registro i
        fseek(out, (i - 1) * tamanho(), SEEK_SET);
        TCliente *ii = le_cliente(out);

        while ((i > 0) && (ii->cod_id > ij->cod_id))
        {
            // posiciona o cursor no registro i+1
            fseek(out, i * tamanho(), SEEK_SET);

            salva_cliente(ii, out);
            i = i - 1;
            // leitura do registro i
            fseek(out, (i - 1) * tamanho(), SEEK_SET);
            ii = le_cliente(out);
//
        }
        // posiciona cursor no registro i + 1
        fseek(out, (i)*tamanho(), SEEK_SET);

        // salva registro j na posição i
        salva_cliente(ij, out);
    }
    // descarrega o buffer para ter certeza que dados foram gravados
    fflush(out);
}

//busca binaria
TCliente *busca_binaria_cliente(int chave, FILE *in, int inicio, int fim)
{
    TCliente *s = NULL;
    int cod_id = -1;
    int contador=0;

    while (inicio <= fim && cod_id != chave)
    {
        int meio = trunc((inicio + fim) / 2);


        contador++;
        printf("Inicio: %d; Fim: %d; Meio: %d; comparacao numero: %d\n", inicio, fim, meio,contador);


        fseek(in, (meio -1) * tamanho(), SEEK_SET);
        s = le_cliente(in);
        cod_id = s->cod_id;
        if (s)
        {
            if (cod_id > chave)
            {
                fim = meio - 1;
            }
            else
            {
                inicio = meio + 1;
            }
        }
    }
    if (cod_id == chave)
    {
        return s;
    }
    else return NULL;
}


