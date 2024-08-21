#include <stdio.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include "interface.h"

int main()
{
    initInterface();

    initGame();

    gameLoop();

    return 0;
}
