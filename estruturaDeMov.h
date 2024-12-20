#ifndef ESTRUTURAEDM_H
#define ESTRUTURAEDM_H

#include "mainLibs.h"

typedef struct {
    int hash_value;
    char* start_game;
    char* game_solution;
} pairEDM;

typedef struct {
    int primo_hash;       // número total de ímpares para cálculo do hash
    int MOD;              // módulo utilizado para limitar o valor do hash
    int totalElementos;   // contém o total de elemntos
    int tamanho_do_array; // contém a quantidade de unidades de memória contígua reservada

    pairEDM** hash_map;
} arrayEDM;

extern arrayEDM* buildEDM(int _size);                                                                          // Inicializa o array
extern void destroyEDM(arrayEDM* interpol, int inst_size);                                                     // Libera todo o espaço de memória associado
extern void leituraEDM(char* instancia, arrayEDM** interpol, char*** input, char*** output, int *inst_size);   // Lê os dados do csv para o problema
extern void execEDM(char* instancia, arrayEDM* interpol, char*** respostas, int *inst_size, int doOutPut);     // Ler os dados da instância

#endif