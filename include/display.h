#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define BLACK_PAIR 1
#define RED_PAIR 2
#define CUSTOM_GRAY 8

struct Coluna;


void initDisplay(void);

void desenha_colunas(Coluna coluna[7]);


#endif