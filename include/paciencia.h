#ifndef _PACIENCIA_H_
#define _PACIENCIA_H_

#define NUM_COLUNAS 7
#define NUM_FUNDACOES 4


struct FilaEnc;
struct PilhaEnc;
struct Carta;

typedef FilaEnc Baralho;

typedef struct {
   PilhaEnc *faceDown;
   FilaEnc *faceUp;
   int numCartasfaceDown;
   int numCartasfaceUp;
} Coluna;

typedef struct
{
    PilhaEnc *cartas;
    char naipe;
    int numCartas;
} Fundacao;

typedef struct {
   Baralho *oculto;
   Baralho *visualizado;
   Carta cartaVisivel;
   int numCartas;
} MonteCompra;

void montaJogo(void);

void gameLoop(void);

void processaComando(const char* comando);


#endif