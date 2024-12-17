#ifndef ESTRUTURABB_H
#define ESTRUTURABB_H

#include "mainLibs.h"

typedef struct {
    int hash_value;
    char* start_game;
    char*game_solution;
} pairBB;

typedef struct {
    int primo_hash;       // número total de ímpares para cálculo do hash
    int MOD;              // módulo utilizado para limitar o valor do hash
    int totalElementos;   // contém o total de elemntos
    int tamanho_do_array; // contém a quantidade de unidades de memória contígua reservada

    pairBB** hash_map;
} arrayBB;

extern arrayBB* buildBB(int _size);                                                                          // Inicializa o array
extern void destroyBB(arrayBB* interpol, int inst_size);                                                     // Libera todo o espaço de memória associado
extern void leituraBB(char* instancia, arrayBB** interpol, char*** input, char*** output, int *inst_size);   // Lê os dados do csv para o problema
extern void execBB(char* instancia, arrayBB* interpol, char*** respostas, int *inst_size);                           // Ler os dados da instância

#endif