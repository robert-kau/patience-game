#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include <locale.h>


#define BLACK_PAIR 1
#define RED_PAIR 2
#define CUSTOM_GRAY 8
#define WHITE_PAIR 3


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

#define LINE_PROMPT  MAX_LINES-3
#define COL_PROMPT  2
#define COL_CMD  (int)(COL_PROMPT + strlen(MSG_CMD))


#define MSG_CMD "Digite um movimento: "
#define MSG_STATUS "Status: "

#define LINE_STATUS (LINE_PROMPT + 1)
#define COL_STATUS COL_PROMPT
#define COL_MSG_STATUS (int)(COL_PROMPT + strlen(MSG_STATUS))

#define MAX_CMD_SIZE 50


void exibeMsgStatus(const char* msg)
{
    mvprintw(LINE_STATUS, COL_MSG_STATUS, "%*s", MAX_COL - COL_MSG_STATUS, " "); 
    mvprintw(LINE_STATUS, COL_MSG_STATUS, "%s", msg);

    refresh();
}

void promptComando(void) 
{
    // Apaga a linha anterior
    mvprintw(LINE_PROMPT, COL_PROMPT, "%*s", MAX_COL - COL_PROMPT, " ");

    // Exibe a mensagem 
    mvprintw(LINE_PROMPT, COL_PROMPT, MSG_CMD);

    mvprintw(LINE_STATUS, COL_STATUS, MSG_STATUS);
    
    move(LINE_PROMPT, COL_CMD);

    // Captura o comando do usuario
    char comando[MAX_CMD_SIZE]; // Tamanho do buffer pode ser ajustado conforme necessario
    getstr(comando);  // Le a string digitada pelo usuario

    // Processa o comando
    processaComando(comando);
}

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

    // Atualiza a tela
    refresh();
}

void fill_rectangle(int start_y, int start_x, int height, int width) 
{
    // Usa o par de cores GRAY_PAIR para preencher o retângulo
    attron(COLOR_PAIR(BLACK_PAIR));
    for (int y = start_y + 1; y < start_y + height - 1; y++) {
        mvhline(y, start_x + 1, ' ', width - 2);
    }
    attroff(COLOR_PAIR(BLACK_PAIR));

    // Atualiza a tela
    refresh();
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

// Função para converter o naipe da carta em sua representacao 
const char* caracter_para_naipe(char naipe) {
    switch (naipe) {
        case 'C': return "\u2665";
        case 'E': return "\u2660";
        case 'O': return "\u2666";
        case 'P': return "\u2663";
        default: return "ERRO";
    }
}

void desenhaFundacoes(Fundacao fundacao[NUM_FUNDACOES])
{
    int i;

    for(i = 0; i < NUM_FUNDACOES; i++)
    {
        if(!vaziaPilhaEnc(fundacao[i].cartas))
        {
            //ha cartas na fundacao para serem exibidas -> exibe o topo

            Carta carta = desempilhaPilhaEnc(fundacao[i].cartas);

            empilhaPilhaEnc(fundacao[i].cartas, carta);

            if (carta.naipe == 'C' || carta.naipe == 'O') 
            {
                attron(COLOR_PAIR(RED_PAIR)); // Ativando a cor vermelha
            }
            else
            {
                attron(COLOR_PAIR(BLACK_PAIR)); // Ativando a cor preta
            }
            mvprintw(LINE_FUND, COL_FUND + DISTANCE_SEVEN_COL * i, "[ %-2s%s ]", valor_para_string(carta.valor), caracter_para_naipe(carta.naipe));
 
        }
        else
        {
            attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

            mvprintw(LINE_FUND, COL_FUND + DISTANCE_SEVEN_COL * i, "[     ]");

        }
    }

    refresh();  // Atualizando a tela para exibir as mudancas
}

// Função para exibir as colunas no terminal
void desenhaMonteCompra(MonteCompra *monte) 
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    if(!vaziaFilaEnc(monte->oculto))
    {
        mvprintw(LINE_STOCK, COL_STOCK, "[  ?  ]");
    }
    else
    {
        mvprintw(LINE_STOCK, COL_STOCK, "[     ]");
    }

    if(monte->cartaVisivel.valor != 0) //ha carta visivel para ser exibida
    {
        if (monte->cartaVisivel.naipe == 'C' || monte->cartaVisivel.naipe == 'O') 
        {
            attron(COLOR_PAIR(RED_PAIR)); // Ativando a cor vermelha
        }
        else
        {
            attron(COLOR_PAIR(BLACK_PAIR)); // Ativando a cor preta
        }
        mvprintw(LINE_STOCK, COL_STOCK + DISTANCE_SEVEN_COL, "[ %-2s%s ]", valor_para_string(monte->cartaVisivel.valor), caracter_para_naipe(monte->cartaVisivel.naipe));
    }
    else
    {
        //desenha monte visualizado vazio
        attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca
        mvprintw(LINE_STOCK, COL_STOCK + DISTANCE_SEVEN_COL, "[     ]");
    }

    refresh();  // Atualizando a tela para exibir as mudancas

}

// Função para exibir as colunas no terminal
void desenhaColunas(Coluna coluna[NUM_COLUNAS]) 
{
    // PilhaEnc *tempPilha = criaPilhaEnc();
    FilaEnc *tempFila = criaFilaEnc();

    int row, col;

    // Desenhando as colunas
    for (col = 0; col < NUM_COLUNAS; col++) 
    {
        row = 0;

        // Imprime cartas ocultas

        attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

        for(row = 0; row < coluna[col].numCartasfaceDown; row++)
        {
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[  ?  ]");
        }

        // while (!vaziaPilhaEnc(coluna[col].faceDown)) 
        // {
        //     //TODO: otimiza contando apenas numero de tamanho da pilha
        //     Carta carta = desempilhaPilhaEnc(coluna[col].faceDown);
        //     empilhaPilhaEnc(tempPilha, carta);

        //     attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca
        //     mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[  ?  ]");
        //     row++;
        // }
        // // recupera pilha
        // while (!vaziaPilhaEnc(tempPilha)) 
        // {
        //     Carta carta = desempilhaPilhaEnc(tempPilha);
        //     empilhaPilhaEnc(coluna[col].faceDown, carta);
        // }

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
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[ %-2s%s ]", valor_para_string(carta.valor), caracter_para_naipe(carta.naipe));
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
}

void initDisplay(void)
{
    setlocale(LC_CTYPE, "");
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

}

