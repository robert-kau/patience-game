#include "fila_enc.h"
#include <stdio.h>
#include <stdlib.h>

// Funcao que cria uma fila
FilaEnc* criaFilaEnc(){
   FilaEnc *fila = (FilaEnc*)malloc(sizeof(FilaEnc));
   if (fila != NULL){ // Idealmente, sempre checar!
      fila->ini = NULL;
      fila->fim = NULL;
   }
   return fila;
}

// Funcao que destroi uma fila
void destroiFilaEnc(FilaEnc *fila){
   NodoFEnc *aux = fila->ini;
   while(aux != NULL){
       NodoFEnc *tmp = aux->prox;
       free(aux); // Cuidar ordem do free
       aux = tmp;
   }
   free(fila);
}

// Funcao que insere um elemento na fila
void enfileiraFilaEnc(FilaEnc *fila, Info info){
   NodoFEnc *novo = (NodoFEnc*)malloc(sizeof(NodoFEnc));
   if (fila != NULL){
      novo->info = info;
      novo->prox = NULL;
      if (fila->fim != NULL)
         fila->fim->prox = novo;
      else
         fila->ini = novo;
      fila->fim = novo;
   }
}


// Funcao que remove um elemento da fila
Info desenfileiraFilaEnc(FilaEnc *fila){
   NodoFEnc *aux = fila->ini; 
   Info info = aux->info;
   fila->ini = aux->prox;
   if (fila->ini == NULL)
      fila->fim = NULL;
   free(aux);
   return info;
}

// Funcao que determina se uma fila eh vazia
int vaziaFilaEnc(FilaEnc *fila){
   return (fila->ini == NULL);
}

// Função para obter o número de cartas na fila utilizando apenas as funções de enfileirar e desenfileirar
int tamanhoFilaEnc(FilaEnc *fila) 
{
    int tamanho = 0;
    FilaEnc *tempFila = criaFilaEnc();
    
    while (!vaziaFilaEnc(fila)) 
    {
        Carta carta = desenfileiraFilaEnc(fila);
        enfileiraFilaEnc(tempFila, carta);
        tamanho++;
    }
    
    while (!vaziaFilaEnc(tempFila)) 
    {
        Carta carta = desenfileiraFilaEnc(tempFila);
        enfileiraFilaEnc(fila, carta);
    }

    destroiFilaEnc(tempFila);
    return tamanho;
}

// Função para imprimir uma fila
void imprimeFilaEnc(FilaEnc *fila) 
{
    FilaEnc *tempFila = criaFilaEnc();
    
    while (!vaziaFilaEnc(fila)) 
    {
        Carta carta = desenfileiraFilaEnc(fila);
        enfileiraFilaEnc(tempFila, carta);
        printf("%c | %d\n", carta.naipe, carta.valor);

    }

    while (!vaziaFilaEnc(tempFila)) 
    {
        Carta carta = desenfileiraFilaEnc(tempFila);
        enfileiraFilaEnc(fila, carta);
    }

    destroiFilaEnc(tempFila);
}

Info finalFilaEnc(FilaEnc *fila) 
{
    FilaEnc *tempFila = criaFilaEnc();
    Carta ultimaCarta = {.valor = 0};

    while (!vaziaFilaEnc(fila)) 
    {
        ultimaCarta = desenfileiraFilaEnc(fila);
        enfileiraFilaEnc(tempFila, ultimaCarta);
    }

    while (!vaziaFilaEnc(tempFila)) 
    {
        Carta carta = desenfileiraFilaEnc(tempFila);
        enfileiraFilaEnc(fila, carta);
    }

    destroiFilaEnc(tempFila);

    return ultimaCarta;
}
