#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define BLACK_PAIR 1
#define RED_PAIR 2
#define WHITE_PAIR 3
#define CUSTOM_GRAY 8

#define DEFAULT_LINES_TERMINAL 24
#define DEFAULT_COL_TERMINAL 80

#define MAX_LINES DEFAULT_LINES_TERMINAL
#define MAX_COL DEFAULT_COL_TERMINAL

#define START_LINE_SEVEN_COL 5
#define START_COL_SEVEN_COL 6

#define SIZE_CARD 7
#define DISTANCE_SEVEN_COL (SIZE_CARD + 3)

#define LINE_STOCK 2
#define COL_STOCK 6

#define LINE_FUND 2
#define COL_FUND 26

#define MSG_MENU "Digite um comando: "
#define MSG_CMD "Digite um movimento: "
#define MSG_STATUS "Status: "

#define LINE_PROMPT MAX_LINES - 3
#define COL_PROMPT 2
#define COL_CMD (int)(COL_PROMPT + strlen(MSG_CMD))

#define LINE_STATUS (LINE_PROMPT + 1)
#define COL_STATUS COL_PROMPT
#define COL_MSG_STATUS (int)(COL_PROMPT + strlen(MSG_STATUS))

#define MAX_CMD_SIZE 50

struct Coluna;
struct MonteCompra;

void initInterface(void);

void limpaTela(void);

void desenhaColunas(Coluna coluna[NUM_COLUNAS]);

void desenhaMonteCompra(MonteCompra *monte);

void desenhaFundacoes(Fundacao fundacao[NUM_FUNDACOES]);

void getUserCmd(void);

void promptComando(void);

void exibeMsgStatus(const char *msg);

void desenhaTelaMenu(void);

void desenhaTelaVencido(void);

#endif