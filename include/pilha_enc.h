#ifndef _PILHA_ENC_H_
#define _PILHA_ENC_H_

#define TAM_MAX_NOME 40
#define TAM_MAX_EMAIL 25

typedef struct{
   int matricula;
   char nome[TAM_MAX_NOME];
   char email[TAM_MAX_EMAIL];
} Aluno;

typedef Aluno Info;

typedef struct nodoLEnc{
   Info info;
   struct nodoLEnc *prox;
} NodoLEnc;

typedef NodoLEnc NodoPEnc;

typedef struct pilhaEnc{
   NodoPEnc *topo;
} PilhaEnc;

// Funcao que cria uma pilha
PilhaEnc* criaPilhaEnc();

// Funcao que destroi uma pilha
void destroiPilhaEnc(PilhaEnc *pilha);

// Funcao que insere um elemento na pilha
void empilhaPilhaEnc(PilhaEnc *pilha, Info info);

// Funcao que remove um elemento da pilha
Info desempilhaPilhaEnc(PilhaEnc *pilha);

// Funcao que determina se uma pilha eh vazia
int vaziaPilhaEnc(PilhaEnc* pilha);

#endif
