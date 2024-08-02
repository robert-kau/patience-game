#ifndef _BARALHO_H_
#define _BARALHO_H_

typedef struct {
   char naipe;
   int valor;
} Carta;

struct filaEnc;

// Funcao que cria um baralho
struct filaEnc* criaBaralho();

#endif
