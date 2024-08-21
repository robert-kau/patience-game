#ifndef _BARALHO_H_
#define _BARALHO_H_

#define NUM_NAIPES 4
#define NUM_CARTAS_POR_NAIPE 13
#define NUM_CARTAS_BARALHO (NUM_CARTAS_POR_NAIPE * NUM_NAIPES)

typedef struct
{
    char naipe;
    int valor;
} Carta;

struct filaEnc;

// Funcao que cria um baralho
struct filaEnc *criaBaralho(void);

// Funcao que detroi a fila de cartas
void destroiBaralho(struct filaEnc *baralho);

// Funcao para embaralhar cartas
void embaralhaCartas(struct filaEnc *fila);

#endif
