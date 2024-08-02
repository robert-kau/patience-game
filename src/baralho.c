#include <stdio.h>
#include <string.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"


FilaEnc* criaBaralho()
{
    FilaEnc *baralho = criaFilaEnc();
    Carta novaCarta;

    const char naipes[] = {'C', 'O', 'P', 'E'};
    const int valores[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

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