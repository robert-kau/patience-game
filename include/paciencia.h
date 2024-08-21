#ifndef _PACIENCIA_H_
#define _PACIENCIA_H_

#define NUM_COLUNAS 7
#define NUM_FUNDACOES 4

struct FilaEnc;
struct PilhaEnc;
struct Carta;

typedef FilaEnc Baralho;

typedef struct
{
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

typedef struct
{
    Baralho *oculto;
    Baralho *visualizado;
    Carta cartaVisivel;
    int numCartas;
} MonteCompra;

enum statusGame
{
    MENU,
    EM_JOGO,
    VENCIDO
};

void montaJogo(void);

void initGame(void);

void gameLoop(void);

void processaComando(char *comando);

enum statusGame obtemStatusJogo();

void defineStatusJogo(enum statusGame status);

#endif