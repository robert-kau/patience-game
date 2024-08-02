#include <stdio.h>
#include <string.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"

int main(){

criaBaralho();
   FilaEnc *baralho_compra = NULL;
   baralho_compra = criaBaralho();

   Carta infoAux;
   while (!vaziaFilaEnc(baralho_compra)){
      infoAux = desenfileiraFilaEnc(baralho_compra);
      printf("%c | %d\n", infoAux.naipe, infoAux.valor);   
   }

   return 0;
}
