#ifndef _FILA_ENC_H_
#define _FILA_ENC_H_

#include "baralho.h"

typedef Carta Info;

typedef struct NodoFEnc{
   Info info;
   struct NodoFEnc *prox;
} NodoFEnc;

typedef struct filaEnc{
   NodoFEnc *ini;
   NodoFEnc *fim;
} FilaEnc;

// Funcao que cria uma fila
FilaEnc* criaFilaEnc();

// Funcao que destroi uma fila
void destroiFilaEnc(FilaEnc *fila);

// Funcao que insere um elemento na fila
void enfileiraFilaEnc(FilaEnc *fila, Info info);

// Funcao que remove um elemento da fila
Info desenfileiraFilaEnc(FilaEnc *fila);

// Funcao que determina se uma fila eh vazia
int vaziaFilaEnc(FilaEnc* fila);


#endif
