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


void printPartitionEmployeeID(FILE *file, char partitionName[])
{

    printf("\nIDs Ingressos of partition %s: \n --->  ", partitionName);

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
int sizeFile(FILE *file, int contSizeFile)
{

    int bytesAUX = 0;

    while(!feof(file))
    {

        fseek(file, bytesAUX * sizeof(TIng), SEEK_SET);

        TIng *aux = readRegisterIngresso(file);
        if(aux != NULL)
        {
            contSizeFile++;
        }

        bytesAUX++;
    }

    return contSizeFile;
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

int naturalSelection(FILE *file, char nameFilePartition[]) {

    int  sizeFileAux = sizeFile(file, 0), position = 0, numberOfPartition = 0, smallElement = 999999999, smallElementPosition = 0, sizeReservoir = 0, flag = 0;

    struct Ingresso ingresso[6];

    int auxIngVet[6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    printf("\nPerforming substitution selection...");

    while (!feof(file)) {

        FILE *fileReservoir = fopen("reservoir.dat", "wb+");

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(numberOfPartition, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "wb+");

        if (flag == 0) {
            for (int i = 0; i < 6; ++i) {

                TIng *auxIng = readRegisterIngresso(file);
                position++;

                ingresso[i] = *auxIng;
                auxIngVet[i] = auxIng->cod;
            }
        }

        while (!feof(file)) {

            for (int i = 0; i < 6; ++i) {

                if (auxIngVet[i] < smallElement) {
                    smallElement = auxIngVet[i];
                    smallElementPosition = i;
                }
            }

            TIng *auxIng = readRegisterIngresso(file);
            position++;


            if (auxIng->cod < ingresso[smallElementPosition].cod) {
                saveRegisterIngresso(auxIng, fileReservoir);
                sizeReservoir++;

                if (sizeReservoir == 6) {

                    break;
                }

            } else {
                saveRegisterIngresso(&ingresso[smallElementPosition], filePartition);
                auxIngVet[smallElementPosition] = auxIng->cod;
                ingresso[smallElementPosition] = *auxIng;
            }

            smallElement = 999999999;

            if (position >= sizeFileAux) {
                break;
            }

        }

        struct Ingresso aux;

        int k, j;

        for (k = 1; k < 6; k++) {

            for (j = 0; j < 6 - 1; j++) {

                if (ingresso[j].cod > ingresso[j + 1].cod) {
                    aux = ingresso[j];
                    ingresso[j] = ingresso[j + 1];
                    ingresso[j + 1] = aux;
                }
            }
        }

        for (int i = 0; i < 6; ++i) {
            saveRegisterIngresso(&ingresso[i], filePartition);
        }

        rewind(fileReservoir);

        for (int i = 0; i < sizeReservoir; ++i) {
            TIng *auxIngReservoir = readRegisterIngresso(fileReservoir);
            ingresso[i] = *auxIngReservoir;
            auxIngVet[i] = auxIngReservoir->cod;
            flag = 1;
        }


        fclose(fileReservoir);
        fclose(filePartition);

        if (position >= sizeFileAux) {
            break;
        }

        sizeReservoir = 0;
        numberOfPartition++;

    }

    for (int i = 0; i <= numberOfPartition; ++i) {

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        printPartitionEmployeeID(filePartition, partitionName);

        fclose(filePartition);
    }

    return numberOfPartition;
}

void treeWinner(TIng **ing, FILE *file, int sizeInTreeOfWinners, int *auxTreeWinner)
{

    int aux;

    for (int i = sizeInTreeOfWinners - 1; i > 0; i--)
    {

        if (i % 2 != 0 && i == sizeInTreeOfWinners - 1)
        {
            aux = (i - 1) / 2;
            *ing[aux] = *ing[i];
        }
        else
        {
            if (i % 2 == 0 && ing[i]->cod < ing[i-1]->cod)
            {
                aux = (i - 2) / 2;
                *ing[aux] = *ing[i];
            }
            else if (i % 2 == 0 && ing[i-1]->cod < ing[i]->cod)
            {
                aux = (i - 2) / 2;
                *ing[aux] = *ing[i - 1];
            }
        }
    }

    saveRegisterIngresso(ing[0], file);
    *auxTreeWinner +=1;
}

void binaryTreeOfWinners(int numberOfPartition, char nameFilePartition[])
{
    int auxNumberOfPartition = numberOfPartition, flagAuxFinal = 0, auxTreeWinner = 0, sizeInTreeOfWinners;

    if (numberOfPartition % 2 == 0)
    {
        sizeInTreeOfWinners = 2 * numberOfPartition - 1;
        auxNumberOfPartition--;
    }
    else
    {
        sizeInTreeOfWinners = 2 * numberOfPartition;
    }

    int aux = sizeInTreeOfWinners - 1;

    TVet *listFile = calloc(sizeof(*listFile), numberOfPartition);
    TIng *smallElement = calloc(sizeof(*smallElement), 1);
    TIng **ingAux = calloc(sizeof(**ingAux), sizeInTreeOfWinners);
    for (int i = 0; i < auxNumberOfPartition; i++) ingAux[i] = calloc(sizeof(TIng), 1);

    FILE *fileBinaryOfTreeSorted = fopen("binaryOfTreeFileSorted.dat", "wb+");
    printf("teste2_interno");
    for (int i = 0; i < numberOfPartition; i++)
    {
        listFile[i].init_p = 0;

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        listFile[i].f = fopen (partitionName, "rb+");
        fseek(listFile[i].f, 0 * sizeof(TIng), SEEK_SET);

        if (aux+1 >= numberOfPartition)
        {
            ingAux[aux] = readRegisterIngresso(listFile[i].f);
            aux--;
        }
        listFile[i].end_p = 0;
    }

    treeWinner(ingAux, fileBinaryOfTreeSorted, sizeInTreeOfWinners, &auxTreeWinner);

    while (flagAuxFinal < numberOfPartition)
    {
        aux = sizeInTreeOfWinners - 1;

        for (int i = 0; i < numberOfPartition; i++)
        {

            if (fgetc(listFile[i].f) == EOF && listFile[i].end_p == 0 && smallElement->cod == ingAux[aux]->cod)
            {
                flagAuxFinal++;
                listFile[i].end_p = 1;
                i--;

                if (flagAuxFinal == numberOfPartition)
                {
                    break;
                }

                for (int j = 0; j < sizeInTreeOfWinners; j++)
                {
                    if (ingAux[j]->cod == smallElement->cod) ingAux[j]->cod = 15000;
                }

                treeWinner(ingAux, fileBinaryOfTreeSorted, sizeInTreeOfWinners, &auxTreeWinner);
            }
            else
            {
                *smallElement = *ingAux[0];

                if (smallElement->cod == ingAux[aux]->cod && listFile[i].end_p == 0)
                {
                    listFile[i].init_p += 1;

                    if (fgetc(listFile[i].f) != EOF)
                    {
                        fseek (listFile[i].f, listFile[i].init_p * sizeof(TIng), SEEK_SET);
                        free(ingAux[aux]);
                        ingAux[aux] = readRegisterIngresso(listFile[i].f);
                        treeWinner(ingAux, fileBinaryOfTreeSorted, sizeInTreeOfWinners, &auxTreeWinner);
                    }
                }
                aux--;
            }
        }
    }

    for (int i = 0; i < numberOfPartition; i++)
    {
        fclose(listFile[i].f);
    }

    for (int i = 0; i < sizeInTreeOfWinners; i++)
    {
        free(ingAux[i]);
    }

    printf("Rodou.\n");
    free(ingAux);
    free(smallElement);
    free(listFile);
    fclose(fileBinaryOfTreeSorted);

}

//substituição

int allVetFrozen (int vet[6]) {

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

int substitutionSelection (FILE *file, char nameFilePartition[]) {

    int numberOfPartition = 0, contSizeFile = 0, position = 6, smallElementPosition = 0, smallElement = 999999, sizeFileAux = 0, selectedPosition = 0;
    struct Ingresso ingresso[6];
    int auxVetIng [6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    sizeFileAux = sizeFile(file, contSizeFile);


    printf("\nPerforming substitution selection...");

    for (int i = 0; i < 6; ++i) {
        fseek(file, i * sizeof(TIng), SEEK_SET);

        TIng *aux = readRegisterIngresso(file);
        ingresso[i] = *aux;

    }

    for (int i = 0; i < 6; ++i) {
        auxVetIng[i] = ingresso[i].cod;
    }

    while (position != sizeFileAux) {

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(numberOfPartition, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "wb+");

        int auxVetFrozen[6] = {0, 0, 0, 0, 0, 0,};

        while (position != sizeFileAux) {

            smallElement = 9999999;

            for (int i = 0; i < 6; ++i) {

                if (smallElement > auxVetIng[i] && auxVetFrozen[i] != 1) {
                    smallElement = auxVetIng[i];
                    smallElementPosition = i;
                }
            }

            saveRegisterIngresso(&ingresso[smallElementPosition], filePartition);

            fseek(file, position * sizeof(TIng), SEEK_SET);

            TIng *aux = readRegisterIngresso(file);

            position++;

            auxVetIng[smallElementPosition] = aux->cod;
            ingresso[smallElementPosition] = *aux;

            if (aux->cod < smallElement) {
                auxVetFrozen[smallElementPosition] = 1;
            }

            if(allVetFrozen(auxVetFrozen) == 1) {
                numberOfPartition++;
                break;
            }

        }

        fclose(filePartition);

        if (position >= sizeFileAux) {
            break;
        }

    }

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    itoa(numberOfPartition, str1, 10);
    strcat(strcpy(partitionName, nameFilePartition), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartitionFinal = fopen(partitionName, "ab+");

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

    for (int i = 0; i < 6; ++i) {
        saveRegisterIngresso(&ingresso[i], filePartitionFinal);
    }

    fclose(filePartitionFinal);

    for (int i = 0; i <= numberOfPartition; ++i) {


        itoa(i, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        printPartitionEmployeeID(filePartition, partitionName);

        fclose(filePartition);
    }

    return numberOfPartition;
}

void mergeSort(int numberOfPartition, char nameFilePartition[]) {

    int *vetSizePartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetFinalPartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetPositionPartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetValueEmployeePartition = (int *) malloc(numberOfPartition * sizeof(int));
    int flagAuxFinal = 0, count, smallElement = INT_MAX, smallElementPosition = 0;

    FILE *auxFileFinal = fopen("mergeSortFileSorted.dat", "wb+");

    for (int i = 0; i <= numberOfPartition; ++i) {

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        rewind(filePartition);

        vetSizePartition[i] = sizeFile(filePartition, 0);
        vetFinalPartition[i] = 0;
        vetPositionPartition[i] = 0;

        fseek(filePartition, vetPositionPartition[i] * sizeof(TIng), SEEK_SET);
        TIng *auxIng = readRegisterIngresso(filePartition);

        vetValueEmployeePartition[i] = auxIng->cod;

        fclose(filePartition);
    }


    while (flagAuxFinal != 1) {

        count = 0;

        for (int i = 0; i <= numberOfPartition; ++i) {

            if (vetFinalPartition[i] == 1) {
                count++;
            }

            if (count == numberOfPartition) {
                flagAuxFinal = 1;
            }
        }

        for (int i = 0; i <= numberOfPartition; ++i) {

            if (vetValueEmployeePartition[i] < smallElement && vetFinalPartition[i] != 1) {
                smallElement = vetValueEmployeePartition[i];
                smallElementPosition = i;
            }
        }

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(smallElementPosition, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        rewind(filePartition);
        fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(TIng), SEEK_SET);
        TIng *auxIng = readRegisterIngresso(filePartition);
        saveRegisterIngresso(auxIng, auxFileFinal);
        vetPositionPartition[smallElementPosition]++;

        rewind(filePartition);

        if (vetPositionPartition[smallElementPosition] >= vetSizePartition[smallElementPosition]) {
            vetFinalPartition[smallElementPosition] = 1;
        } else {
            fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(TIng), SEEK_SET);
            TIng *auxIng2 = readRegisterIngresso(filePartition);
            vetValueEmployeePartition[smallElementPosition] = auxIng2->cod;
        }

        fclose(filePartition);

        smallElement = INT_MAX;

    }

    printPartitionEmployeeID(auxFileFinal, "mergeSortFileSorted.dat");

    fclose(auxFileFinal);
    free(vetFinalPartition);
    free(vetSizePartition);
    free(vetPositionPartition);
    free(vetValueEmployeePartition);
}
