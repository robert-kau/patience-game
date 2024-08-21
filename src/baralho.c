#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"

// Funcao que cria uma fila de cartas
FilaEnc *criaBaralho(void)
{
    FilaEnc *baralho = criaFilaEnc();
    Carta novaCarta;

    const char naipes[NUM_NAIPES] = {'C', 'O', 'P', 'E'};
    const int valores[NUM_CARTAS_POR_NAIPE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    for (int i = 0; i < 4; i++) // Loop para os naipes
    {
        novaCarta.naipe = naipes[i];
        for (int j = 0; j < 13; j++) // Loop para os valores
        {
            novaCarta.valor = valores[j];

            enfileiraFilaEnc(baralho, novaCarta);
        }
    }
    return baralho;
}

// Funcao que detroi a fila de cartas
void destroiBaralho(FilaEnc *baralho)
{
    destroiFilaEnc(baralho);
}

// Funcao para embaralhar cartas
void embaralhaCartas(FilaEnc *fila)
{
    int tamanho = tamanhoFilaEnc(fila);
    if (tamanho <= 1)
        return; //nao ha necessidade de embaralhar se houver 0 ou 1 carta

    //array para armazenar cartas temporariamente
    Carta *arrayCartas = (Carta *)malloc(tamanho * sizeof(Carta));

    //remover cartas da fila e armazenar no array
    for (int i = 0; i < tamanho; i++)
    {
        arrayCartas[i] = desenfileiraFilaEnc(fila);
    }

    //embaralhar o array de cartas
    srand(time(NULL));
    for (int i = 0; i < tamanho; i++)
    {
        int j = rand() % tamanho;
        Carta temp = arrayCartas[i];
        arrayCartas[i] = arrayCartas[j];
        arrayCartas[j] = temp;
    }

    //reinserir cartas na fila
    for (int i = 0; i < tamanho; i++)
    {
        enfileiraFilaEnc(fila, arrayCartas[i]);
    }

    //liberar memoria do array temporario
    free(arrayCartas);
}