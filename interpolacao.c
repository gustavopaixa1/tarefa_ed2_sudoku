#include "interpolacao.h"
#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 164
#define MAX_STRING_LENGTH 81

// Funções private--------------------------------------------------------
static int hashFunc(char* start_game, int primo, int tamanho, int MOD) {
    long long hash = 0;  // Usamos long long para evitar overflow durante as somas

    for (int i = 0; i < tamanho && start_game[i] != '\0'; i++) {
        hash = (hash * primo + start_game[i]) % MOD;
    }

    return (int)hash;  // Retorna o hash como um número inteiro
}

// Função para trocar dois elementos
static void swap(pair** a, pair **b) {
    pair* temp = *a;
    *a = *b;
    *b = temp;
}

// Função para particionar o vetor
static int partition(arrayInter* interpol, int low, int high) {
    int pivot = interpol->hash_map[high]->hash_value;  // O pivô é o último elemento
    int i = low - 1;  // Índice do menor elemento

    for (int j = low; j < high; j++) {
        // Se o elemento atual for menor ou igual ao pivô
        if (interpol->hash_map[j]->hash_value <= pivot) {
            i++;    // Incrementa o índice do menor elemento
            swap(&(interpol->hash_map[i]), &(interpol->hash_map[j]));
        }
    }

    // Coloca o pivô no lugar correto
    swap(&(interpol->hash_map[i+1]), &(interpol->hash_map[high]));
    return (i + 1);  // Retorna o índice do pivô
}

// Função do Quicksort
static void quicksort(arrayInter* interpol, int low, int high) {
    if (low < high) {
        // Encontrar o índice de particionamento
        int pi = partition(interpol, low, high);

        // Ordenar as duas metades
        quicksort(interpol, low, pi - 1);  // Ordena a parte à esquerda do pivô
        quicksort(interpol, pi + 1, high); // Ordena a parte à direita do pivô
    }
}

static void insert(char* start_game, char* solution, arrayInter** interpol) {
    // TODO Insere a solução no hash_map
    int hash_number = hashFunc(start_game, (*interpol)->primo_hash, MAX_STRING_LENGTH, (*interpol)->MOD);

    if ((*interpol)->tamanho_do_array == (*interpol)->totalElementos) {
        int novoTamanho = (*interpol)->tamanho_do_array*2;
        pair **temp;

        temp = realloc((*interpol)->hash_map, novoTamanho*sizeof(pair*));
        // Verifica se a realocação foi bem-sucedida
        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            return;  // Se a realocação falhar, não alteramos o ponteiro original
        }

        (*interpol)->hash_map = temp;
        (*interpol)->tamanho_do_array = novoTamanho;
    }

    int index = (*interpol)->totalElementos;

    (*interpol)->hash_map[index] = malloc(sizeof(pair));
    (*interpol)->totalElementos++;

    (*interpol)->hash_map[index]->hash_value = hash_number;
    (*interpol)->hash_map[index]->start_game = start_game;
    (*interpol)->hash_map[index]->game_solution = solution;
}
// ----------------------------------------------------------------------



// Funções public--------------------------------------------------------
arrayInter* buildADI(int _size) {
    
    arrayInter *my_array_inter = malloc(sizeof(arrayInter));

    // Verifica se a alocação foi bem-sucedida
    if (my_array_inter == NULL) {
        return NULL;  // Se não conseguiu alocar memória, retorna NULL
    }

    my_array_inter->primo_hash = 31;
    my_array_inter->MOD = 1000000007;
    my_array_inter->tamanho_do_array = 1;
    my_array_inter->totalElementos = 0;

    my_array_inter->totalElementos = _size;

    my_array_inter->hash_map = malloc(sizeof(pair*));

    // Verifica se a alocação do hash_map foi bem-sucedida
    if (my_array_inter->hash_map == NULL) {
        free(my_array_inter);  // Libera a memória de arrayInter antes de retornar
        return NULL;
    }

    return my_array_inter;
}

static void destroyADIHashMap(pair** hash_map, int total) {
    for (int i = 0; i < total; i++) {
        free(hash_map[i]);
    }
    free(hash_map);
}

void destroyADI(arrayInter* interpol, int inst_size) {
    // TODO Libera espaço de memória associado

    destroyADIHashMap(interpol->hash_map, interpol->totalElementos);
    free(interpol);
}

void leituraADI(char* instancia, arrayInter** interpol, char*** input, char*** output, int *inst_size) {
    FILE *file = fopen(instancia, "r");  // Abre o arquivo da instância
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    // Inicializa o número de instâncias (inst_size)
    *inst_size = 0;

    // Lê as linhas do arquivo até o final
    char line[MAX_LINE_LENGTH];
    char val[2];

    fgets(line, sizeof(line), file);  // Lê e descarta a primeira linha
    while (fgets(line, sizeof(line), file)) {
        fgets(val, sizeof(val), file);
        // printf("%d: ", *inst_size);
        // puts(line);
        (*inst_size)++;
    }
    
    // Rewind para começar a ler novamente
    rewind(file);
    fgets(line, sizeof(line), file);  // Lê e descarta a primeira linha novamente

    // Aloca memória para o vetor de strings (input e output) com o número de linhas
    *input = malloc((*inst_size) * sizeof(char*));
    *output = malloc((*inst_size) * sizeof(char*));
    if (*input == NULL || *output == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        return;
    }

    // Lê as linhas do arquivo e separa em input e output
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        fgets(val, sizeof(val), file);
        // Aloca memória para cada string de input e output
        (*input)[i] = malloc((MAX_STRING_LENGTH + 1) * sizeof(char));  // +1 para o '\0'
        (*output)[i] = malloc((MAX_STRING_LENGTH + 1) * sizeof(char));  // +1 para o '\0'

        if ((*input)[i] == NULL || (*output)[i] == NULL) {
            perror("Erro ao alocar memória para input ou output");
            fclose(file);
            return;
        }

        // Divide a linha em duas partes com base na vírgula
        char* token = strtok(line, ",");
        if (token != NULL) {
            strncpy((*input)[i], token, MAX_STRING_LENGTH);
            (*input)[i][MAX_STRING_LENGTH] = '\0';  // Garante que a string está terminada

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy((*output)[i], token, MAX_STRING_LENGTH);
                (*output)[i][MAX_STRING_LENGTH] = '\0';  // Garante que a string está terminada
            }
        }

        i++;
    }

    for (int i = 0; i < *inst_size; i++) {
        insert((*input)[i], (*output)[i], interpol);
    }

    quicksort((*interpol), 0, (*interpol)->totalElementos-1);

    fclose(file);  // Fecha o arquivo
}

static int buscaInterpol(arrayInter* array, int hash_number, int doOutPut) {
    int size = array->totalElementos;
    int low = 0, high = size - 1;
    int iteracoes = 0;

    while (low <= high && hash_number >= array->hash_map[low]->hash_value && hash_number <= array->hash_map[high]->hash_value) {

        iteracoes++;
        // Cálculo da posição interpolada
        int value1 = (array->hash_map[high]->hash_value - array->hash_map[low]->hash_value);
        int value = ((double)(high - low) / (array->hash_map[high]->hash_value - array->hash_map[low]->hash_value));
        int pos = low + (((double)(high - low) / (array->hash_map[high]->hash_value - array->hash_map[low]->hash_value)) * (hash_number - array->hash_map[low]->hash_value));

        if (low == high) {
            pos = low;
        }
        // Verifica se o elemento foi encontrado
        if (array->hash_map[pos]->hash_value == hash_number) {
            // Move para a esquerda até encontrar a primeira ocorrência
            while (pos > 0 && array->hash_map[pos - 1]->hash_value == hash_number) {
                iteracoes++;
                pos--;
            }

            if (doOutPut) {
                printf("Total de %d iteracoes: ", iteracoes);
            }
            return pos;
        }

        // Ajusta os limites de busca
        if (array->hash_map[pos]->hash_value < hash_number) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }

    // Retorna -1 se o elemento não foi encontrado
    return -1;
}

static char* getSolution(char* start_game, arrayInter* arr, int doOutPut) {
    int hash_number = hashFunc(start_game, arr->primo_hash, MAX_STRING_LENGTH, arr->MOD);
    int index = buscaInterpol(arr, hash_number, doOutPut);

    if (index == -1) {
        return "Elemento nao encontrado!";
    }

    while (index < arr->totalElementos && arr->hash_map[index]->hash_value == hash_number) {

        if (strcmp(arr->hash_map[index]->start_game, start_game) == 0) {
            return arr->hash_map[index]->game_solution;
        }
        index++;
    }

    return "Elemento nao encontrado!";
}

void execADI(char* instancia, arrayInter* arr, char*** respostas, int *inst_size, int doOutPut) {
    FILE* file = fopen(instancia, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    // Lê o número de strings (primeira linha do arquivo)
    if (fscanf(file, "%d\n", inst_size) != 1) {
        perror("Erro ao ler o numero de strings");
        fclose(file);
        return;
    }

    // Aloca memória para o vetor de respostas
    *respostas = malloc((*inst_size) * sizeof(char*));
    if (*respostas == NULL) {
        perror("Erro ao alocar memória para as respostas");
        fclose(file);
        return;
    }

    // Lê as strings do arquivo
    for (int i = 0; i < *inst_size; i++) {
        (*respostas)[i] = malloc((MAX_STRING_LENGTH + 1) * sizeof(char));  // +1 para o '\0'
        if ((*respostas)[i] == NULL) {
            perror("Erro ao alocar memória para uma string");
            fclose(file);
            return;
        }

        if (fgets((*respostas)[i], MAX_STRING_LENGTH + 2, file) != NULL) {
            // Remove o caractere de nova linha, se presente
            (*respostas)[i][strcspn((*respostas)[i], "\n")] = '\0';
        } else {
            perror("Erro ao ler uma linha do arquivo");
            fclose(file);
            return;
        }
    }

    for (int i = 0; i < *inst_size; i++) {
        if (doOutPut) {
            puts(getSolution((*respostas)[i], arr, doOutPut));
        } else {
            getSolution((*respostas)[i], arr, doOutPut);
        }
    }

    fclose(file);
    printf("Leitura concluida: %d strings carregadas.\n", *inst_size);
}
// ----------------------------------------------------------------------

