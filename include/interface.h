#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define BLACK_PAIR 1
#define RED_PAIR 2
#define CUSTOM_GRAY 8

struct Coluna;
struct MonteCompra;

void initInterface(void);

void limpaTela(void);

void desenhaColunas(Coluna coluna[]);

void desenhaMonteCompra(MonteCompra *monte);

void desenhaFundacoes(Fundacao fundacao[]);

void getUserCmd(void);

void promptComando(void); 

void exibeMsgStatus(const char* msg);

void desenhaTelaMenu(void);

void desenhaTelaVencido(void);

#endif