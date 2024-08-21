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

//Funcao que cria estruturas a serem usadas no jogo
void montaJogo(void);

// Funcao que inicializa estado do jogo
void initGame(void);

// Funcao que contem loop principal do jogo
void gameLoop(void);

// Funcao que processa comando digitado pelo usuario
void processaComando(char *comando);

// Funcao que retorna o status do jogo (MENU, EM_JOGO, VENCIDO)
enum statusGame obtemStatusJogo();

// Funcao que define o status do jogo (MENU, EM_JOGO, VENCIDO)
void defineStatusJogo(enum statusGame status);

#endif