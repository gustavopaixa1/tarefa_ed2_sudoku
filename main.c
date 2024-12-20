#include "estruturaBB.h"
#include "estruturaDeMov.h"
#include "interpolacao.h"
#include <stdio.h>
#include <string.h>

void show(char **output, int n)
{
    for (int i = 0; i < n; i++)
    {
        puts(output[i]);
    }
}

int main(int argc, char *argv[])
{

    // A estrutura utilizada e a instância a ser resolvida são passado como parâmetro
    char instancia[100];
    int doOutPut;

    // size_of_instances
    int inst_size_ADI;
    int inst_size_EDM;
    int inst_size_BB;

    // Leitura dos dados

    // Input arrays
    char **inputADI;
    char **inputEDM;
    char **inputBB;

    // output arrays
    char **outputADI;
    char **outputEDM;
    char **outputBB;

    // arrays de resposta
    char **respADI;
    char **respEDM;
    char **respBB;

    // Variáveis para calculas o tempo
    clock_t inicio, fim;
    double tempoDeCpu;

    // Declaração das principais variáveis do problema
    arrayInter *my_interpol = buildADI(0);
    arrayEDM *my_array_mov = buildEDM(0);
    arrayBB *my_array_bb = buildBB(0);

    // if (my_array_bb == NULL) {
    //     perror("Falha na alocação de memória para my_array_bb");
    //     return 1;
    // }

    if (my_array_mov == NULL) {
        perror("Falha na alocação de memória para my_array_mov");
        return 2;
    }

    if (my_interpol == NULL)
    {
        perror("Falha na alocação de memória para my_interpol");
        return 3;
    }

    // Leitura dos dados e armazenamento de acordo com entrada
    leituraADI("sudoku.csv", &my_interpol, &inputADI, &outputADI, &inst_size_ADI);
    leituraEDM("sudoku.csv", &my_array_mov, &inputEDM, &outputEDM, &inst_size_EDM);
    // leituraBB("sudoku_file/sudoku.csv", &my_array_bb, &inputBB, &outputBB, &inst_size_BB);

    do
    {
        printf("Selecione a busca desejada: \n");
        printf("1. Busca com interpolacao.\n");
        printf("2. Busca com movimentacao.\n");
        printf("3. Interpolação e movimentacao. \n");
        printf("0. Sair.\n");

        int opcao;
        printf("Digite a opcao: ");
        scanf("%d", &opcao);
        scanf("%*c"); // Limpar o buffer de entrada

        if (opcao == 0)
        {
            break; // Encerra o loop se o usuário escolher sair
        }

        printf("Digite a instancia que voce deseja rodar: ");
        fgets(instancia, sizeof(instancia), stdin);

        printf("Deseja que as solucoes encontradas para os jogos sejam mostradas? (1 = sim, = 0 = nao): ");
        scanf("%d", &doOutPut);
        scanf("%*c");

        // Removendo o \n
        size_t len = strlen(instancia);
        if (len > 0 && instancia[len - 1] == '\n')
        {
            instancia[len - 1] = '\0'; // Substitui o \n por \0
        }

        switch (opcao)
        {
        case 1:
        {
            printf("Executando busca com interpolacao...\n");
            inicio = clock();
            execADI(instancia, my_interpol, &respADI, &inst_size_ADI, doOutPut);
            fim = clock();
            tempoDeCpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Tempo de CPU - Interpolacao: %lf\n\n", tempoDeCpu);
            free(respADI);
            break;
        }

        case 2:
        {
            printf("Executando busca com movimentacao...\n");
            inicio = clock();
            execEDM(instancia, my_array_mov, &respEDM, &inst_size_EDM, doOutPut);
            fim = clock();
            tempoDeCpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Tempo de CPU - Movimentacao: %lf\n\n", tempoDeCpu);
            free(respEDM);
            break;
        }

        case 3:
        {
            printf("Executando busca com interpolacao...\n");
            inicio = clock();
            execADI(instancia, my_interpol, &respADI, &inst_size_ADI, doOutPut);
            fim = clock();
            tempoDeCpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Tempo de cpu - Interpolacao: %lf\n\n", tempoDeCpu);
            free(respADI);

            printf("Executando busca com movimentacao...\n");
            inicio = clock();
            execEDM(instancia, my_array_mov, &respEDM, &inst_size_EDM, doOutPut);
            fim = clock();
            tempoDeCpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Tempo de CPU - Movimentacao: %lf\n\n", tempoDeCpu);
            free(respEDM);

            break;
        }

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }

        // Busca Binaria
        // inicio = clock();
        // execBB("input/inst1.txt", my_array_bb, &respBB, &inst_size_BB);
        // fim = clock();
        // tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        // printf("%lf\n", tempoDeCpu);

    } while (strcmp(instancia, "0") != 0);

    // Destroi as estruturas criadas

    destroyADI(my_interpol, inst_size_ADI);
    destroyEDM(my_array_mov, inst_size_EDM);
    // destroyBB(my_array_bb, inst_size_BB);

    // Liberar memória ADI
    for (int i = 0; i < inst_size_ADI; i++)
    {
        free(inputADI[i]);
        free(outputADI[i]);
    }

    free(inputADI);
    free(outputADI);

    // Liberar memória EDM
    for (int i = 0; i < inst_size_EDM; i++)
    {
        free(inputEDM[i]);
        free(outputEDM[i]);
    }
    free(inputEDM);
    free(outputEDM);

    // // Liberar memória BB
    // for (int i = 0; i < inst_size_BB; i++) {
    //     free(inputBB[i]);
    //     free(outputBB[i]);
    // }
    // free(inputBB);
    // free(outputBB);

    return 0;
}