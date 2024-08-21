/**
 * @file main.c
 * @author Robert Buss Kaufmann
 * @brief 
 * @version 1.0
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <stdio.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include "interface.h"

// Funcao principal de entrada do programa
int main()
{
    initInterface();

    initGame();

    gameLoop();

    return 0;
}
