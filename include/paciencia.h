#ifndef _PACIENCIA_H_
#define _PACIENCIA_H_

#define NUM_COLUNAS 7
#define NUM_FUNDACOES 4


struct FilaEnc;
struct PilhaEnc;

typedef FilaEnc Baralho;

typedef struct {
   PilhaEnc *faceDown;
   FilaEnc *faceUp;
} Coluna;

typedef struct
{
    PilhaEnc *espadas;
    PilhaEnc *paus;
    PilhaEnc *copas;
    PilhaEnc *ouros;
} Fundacao;

void montaJogo(void);


#endif