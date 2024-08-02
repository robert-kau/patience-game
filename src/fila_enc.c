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

