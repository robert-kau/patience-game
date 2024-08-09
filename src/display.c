#include <ncurses.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"


#define BLACK_PAIR 1
#define RED_PAIR 2
#define CUSTOM_GRAY 8
#define WHITE_PAIR 3


#define DEFAULT_LINES_TERMINAL 24
#define DEFAULT_COL_TERMINAL 80

#define MAX_LINES DEFAULT_LINES_TERMINAL
#define MAX_COL DEFAULT_COL_TERMINAL

#define START_LINE_SEVEN_COL 5
#define START_COL_SEVEN_COL 16

#define SIZE_CARD 4
#define DISTANCE_SEVEN_COL (SIZE_CARD + 4)


void draw_rectangle(int start_y, int start_x, int height, int width) 
{
    // Desenha as linhas horizontais
    mvhline(start_y, start_x, 0, width); // Topo
    mvhline(start_y + height - 1, start_x, 0, width); // Base

    // Desenha as linhas verticais
    mvvline(start_y, start_x, 0, height); // Esquerda
    mvvline(start_y, start_x + width - 1, 0, height); // Direita

    // Desenha os cantos
    mvaddch(start_y, start_x, ACS_ULCORNER); // Canto superior esquerdo
    mvaddch(start_y, start_x + width - 1, ACS_URCORNER); // Canto superior direito
    mvaddch(start_y + height - 1, start_x, ACS_LLCORNER); // Canto inferior esquerdo
    mvaddch(start_y + height - 1, start_x + width - 1, ACS_LRCORNER); // Canto inferior direito
}

void fill_rectangle(int start_y, int start_x, int height, int width) 
{
    // Usa o par de cores GRAY_PAIR para preencher o retângulo
    attron(COLOR_PAIR(BLACK_PAIR));
    for (int y = start_y + 1; y < start_y + height - 1; y++) {
        mvhline(y, start_x + 1, ' ', width - 2);
    }
    attroff(COLOR_PAIR(BLACK_PAIR));
}


// Função para converter o valor da carta em uma string legível
const char* valor_para_string(char valor) {
    switch (valor) {
        case 1: return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: {
            static char buffer[3];
            snprintf(buffer, sizeof(buffer), "%d", valor);
            return buffer;
        }
    }
}

// Função para exibir as colunas no terminal
void desenha_colunas(Coluna coluna[7]) 
{

    PilhaEnc *tempPilha = criaPilhaEnc();
    FilaEnc *tempFila = criaFilaEnc();

    int row = 0;


    // Desenhando as colunas
    for (int col = 0; col < 7; col++) 
    {
        row = 0;

        // Imprime cartas ocultas
        while (!vaziaPilhaEnc(coluna[col].faceDown)) 
        {
            Carta carta = desempilhaPilhaEnc(coluna[col].faceDown);
            empilhaPilhaEnc(tempPilha, carta);
            //printf("%c | %d\n", carta.naipe, carta.valor);
            // if (carta.naipe == 'C' || carta.naipe == 'O') 
            // {
            //     attron(COLOR_PAIR(RED_PAIR)); // Ativando a cor vermelha
            // }
            // else
            // {
            //     attron(COLOR_PAIR(BLACK_PAIR)); // Ativando a cor preta
            // }
            //mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "I[%s%c]", valor_para_string(carta.valor), carta.naipe);
            
            attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "I[??]");
            row++;
        }
        // recupera pilha
        while (!vaziaPilhaEnc(tempPilha)) 
        {
            Carta carta = desempilhaPilhaEnc(tempPilha);
            empilhaPilhaEnc(coluna[col].faceDown, carta);
        }

        // Imprime cartas visivies
        while (!vaziaFilaEnc(coluna[col].faceUp)) 
        {
            Carta carta = desenfileiraFilaEnc(coluna[col].faceUp);
            enfileiraFilaEnc(tempFila, carta);
            if (carta.naipe == 'C' || carta.naipe == 'O') 
            {
                attron(COLOR_PAIR(RED_PAIR)); // Ativando a cor vermelha
            }
            else
            {
                attron(COLOR_PAIR(BLACK_PAIR)); // Ativando a cor preta
            }
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "V[%s%c]", valor_para_string(carta.valor), carta.naipe);
            row++;
        }
        // recupera fila
        while (!vaziaFilaEnc(tempFila)) 
        {
            Carta carta = desenfileiraFilaEnc(tempFila);
            enfileiraFilaEnc(coluna[col].faceUp, carta);
        }
    }
    refresh();  // Atualizando a tela para exibir as mudanças
    getch();
    endwin();


}

void initDisplay(void)
{
    // Inicializa a ncurses
    initscr();
    start_color();

    // Define uma cor cinza personalizada mais clara
    init_color(CUSTOM_GRAY, 500, 500, 500); // Define a cor 8 como cinza claro (RGB 700/1000)

    // Define pares de cores com fundo cinza
    init_pair(BLACK_PAIR, COLOR_BLACK, CUSTOM_GRAY); // Texto preto com fundo cinza
    init_pair(RED_PAIR, COLOR_RED, CUSTOM_GRAY);     // Texto vermelho com fundo cinza
    init_pair(WHITE_PAIR, COLOR_WHITE, CUSTOM_GRAY);     // Texto branco com fundo cinza


    // Limpa a tela
    clear();

    draw_rectangle(0, 0, MAX_LINES, MAX_COL);
    fill_rectangle(0, 0, MAX_LINES, MAX_COL);

    // Atualiza a tela
    refresh();


    // // Espera o usuário pressionar uma tecla
    // getch();

    // // Finaliza a ncurses
    // endwin();
}


