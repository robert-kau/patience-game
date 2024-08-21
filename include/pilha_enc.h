#ifndef _PILHA_ENC_H_
#define _PILHA_ENC_H_

#include "baralho.h"

typedef Carta Info;

typedef struct NodoPEnc
{
    Info info;
    struct NodoPEnc *prox;
} NodoPEnc;

typedef struct pilhaEnc
{
    NodoPEnc *topo;
} PilhaEnc;

// Funcao que cria uma pilha
PilhaEnc *criaPilhaEnc();

// Funcao que destroi uma pilha
void destroiPilhaEnc(PilhaEnc *pilha);

// Funcao que insere um elemento na pilha
void empilhaPilhaEnc(PilhaEnc *pilha, Info info);

// Funcao que remove um elemento da pilha
Info desempilhaPilhaEnc(PilhaEnc *pilha);

// Funcao que determina se uma pilha eh vazia
int vaziaPilhaEnc(PilhaEnc *pilha);

#endif
