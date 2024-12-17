#ifndef INTERPOLACAO_H
#define INTERPOLACAO_H

#include "mainLibs.h"

typedef struct {
    int hash_value;
    char* start_game;
    char*game_solution;
} pair;

typedef struct {
    int primo_hash;       // número total de ímpares para cálculo do hash
    int MOD;              // módulo utilizado para limitar o valor do hash
    int totalElementos;   // contém o total de elemntos
    int tamanho_do_array; // contém a quantidade de unidades de memória contígua reservada

    pair** hash_map;
} arrayInter;

extern arrayInter* buildADI(int _size);                                                                          // Inicializa o array
extern void destroyADI(arrayInter* interpol, int inst_size);                                                     // Libera todo o espaço de memória associado
extern void leituraADI(char* instancia, arrayInter** interpol, char*** input, char*** output, int *inst_size); // Lê os dados do csv para o problema
extern void execADI(char* instancia, arrayInter* arr, char*** respostas, int *inst_size, int doOutPut);                           // Ler os dados da instância

#endif
