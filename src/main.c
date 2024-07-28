#include <stdio.h>
#include <string.h>
#include "fila_enc.h"

int main(){

   FilaEnc *fila;
   fila = criaFilaEnc();
   
   // Aqui seriam chamadas as funcoes de inicializacao de nodos
   Info info1 = {1, "Fulano de Tal", "fulano@email.com"};
   Info info2 = {2, "Beltrana de Tal", "beltrana@email.com"};
   Info info3 = {3, "Cicrano de Tal", "cicrano@email.com"};
   Info info4 = {4, "Ciclana de Tal", "ciclana@email.com"};
   Info info5 = {5, "Ciclano de Tal", "ciclano@email.com"};
     
   enfileiraFilaEnc(fila, info1); 
   enfileiraFilaEnc(fila, info2); 
   enfileiraFilaEnc(fila, info3); 
   enfileiraFilaEnc(fila, info4); 
   enfileiraFilaEnc(fila, info5); 
   
   Info infoAux;
   while (!vaziaFilaEnc(fila)){
      infoAux = desenfileiraFilaEnc(fila);
      printf("%d | %s (%s)\n", infoAux.matricula, infoAux.nome, infoAux.email);   
   }

   destroiFilaEnc(fila);

   return 0;
}
