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

// Função para obter o número de cartas na fila utilizando apenas as funções de enfileirar e desenfileirar
int tamanhoFilaEnc(FilaEnc *fila);

// Função para imprimir uma fila
void imprimeFilaEnc(FilaEnc *fila);

// Função que retorna o nodo do final de uma fila
Info finalFilaEnc(FilaEnc *fila);

#endif
