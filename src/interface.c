#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include "interface.h"
#include <locale.h>

void exibeMsgStatus(const char *msg)
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    mvprintw(LINE_STATUS, COL_STATUS, "%*s", MAX_COL - COL_MSG_STATUS, " ");
    mvprintw(LINE_STATUS, COL_MSG_STATUS, "%s", msg);

    refresh();
}

void promptComando(void)
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    // Apaga a linha anterior
    mvprintw(LINE_PROMPT, COL_PROMPT, "%*s", MAX_COL - COL_CMD, " ");

    // Exibe a mensagem
    switch (obtemStatusJogo())
    {
    case MENU:
    case VENCIDO:
        mvprintw(LINE_PROMPT, COL_PROMPT, MSG_MENU);
        break;
    case EM_JOGO:
        mvprintw(LINE_PROMPT, COL_PROMPT, MSG_CMD);
        mvprintw(LINE_STATUS, COL_STATUS, MSG_STATUS);
        break;
    }

    move(LINE_PROMPT, COL_CMD);

    // Captura o comando do usuario
    char comando[MAX_CMD_SIZE]; // Tamanho do buffer pode ser ajustado conforme necessario
    getstr(comando);            // Le a string digitada pelo usuario

    // Processa o comando
    processaComando(comando);
}

void desenhaRetangulo(int start_y, int start_x, int height, int width)
{
    // Desenha as linhas horizontais
    mvhline(start_y, start_x, 0, width);              // Topo
    mvhline(start_y + height - 1, start_x, 0, width); // Base

    // Desenha as linhas verticais
    mvvline(start_y, start_x, 0, height);             // Esquerda
    mvvline(start_y, start_x + width - 1, 0, height); // Direita

    // Desenha os cantos
    mvaddch(start_y, start_x, ACS_ULCORNER);                          // Canto superior esquerdo
    mvaddch(start_y, start_x + width - 1, ACS_URCORNER);              // Canto superior direito
    mvaddch(start_y + height - 1, start_x, ACS_LLCORNER);             // Canto inferior esquerdo
    mvaddch(start_y + height - 1, start_x + width - 1, ACS_LRCORNER); // Canto inferior direito

    // Atualiza a tela
    refresh();
}

void preencheRetangulo(int start_y, int start_x, int height, int width)
{
    // Usa o par de cores GRAY_PAIR para preencher o retângulo
    attron(COLOR_PAIR(BLACK_PAIR));
    for (int y = start_y + 1; y < start_y + height - 1; y++)
    {
        mvhline(y, start_x + 1, ' ', width - 2);
    }
    attroff(COLOR_PAIR(BLACK_PAIR));

    // Atualiza a tela
    refresh();
}

// Função para converter o valor da carta em uma string legível
const char *obtemCharValor(char valor)
{
    switch (valor)
    {
    case 1:
        return "A";
    case 11:
        return "J";
    case 12:
        return "Q";
    case 13:
        return "K";
    default:
    {
        static char buffer[3];
        snprintf(buffer, sizeof(buffer), "%d", valor);
        return buffer;
    }
    }
}

// Função para converter o naipe da carta em sua representacao
const char *obtemCharNaipe(char naipe)
{
    switch (naipe)
    {
    case 'C':
        return "\u2665";
    case 'E':
        return "\u2660";
    case 'O':
        return "\u2666";
    case 'P':
        return "\u2663";
    default:
        return "ERRO";
    }
}

void limpaTela()
{
    preencheRetangulo(0, 0, MAX_LINES - 2, MAX_COL); // preenche retangulo, mas mantem linha de comando e status (-2)
}

void desenhaFundacoes(Fundacao fundacao[NUM_FUNDACOES])
{
    int i;

    for (i = 0; i < NUM_FUNDACOES; i++)
    {
        attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

        mvprintw(LINE_FUND - 1, COL_FUND + DISTANCE_SEVEN_COL * i, "Fund. %d", i);

        if (!vaziaPilhaEnc(fundacao[i].cartas))
        {
            // ha cartas na fundacao para serem exibidas -> exibe o topo

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
            mvprintw(LINE_FUND, COL_FUND + DISTANCE_SEVEN_COL * i, "[ %-2s%s ]", obtemCharValor(carta.valor), obtemCharNaipe(carta.naipe));
        }
        else
        {
            attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

            mvprintw(LINE_FUND, COL_FUND + DISTANCE_SEVEN_COL * i, "[     ]");
        }
    }

    refresh(); // Atualizando a tela para exibir as mudancas
}

// Função para exibir as colunas no terminal
void desenhaMonteCompra(MonteCompra *monte)
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    mvprintw(LINE_STOCK - 1, COL_STOCK * 2, "Monte");

    if (!vaziaFilaEnc(monte->oculto))
    {
        mvprintw(LINE_STOCK, COL_STOCK, "[  ?  ]");
    }
    else
    {
        mvprintw(LINE_STOCK, COL_STOCK, "[     ]");
    }

    if (monte->cartaVisivel.valor != 0) // ha carta visivel para ser exibida
    {
        if (monte->cartaVisivel.naipe == 'C' || monte->cartaVisivel.naipe == 'O')
        {
            attron(COLOR_PAIR(RED_PAIR)); // Ativando a cor vermelha
        }
        else
        {
            attron(COLOR_PAIR(BLACK_PAIR)); // Ativando a cor preta
        }
        mvprintw(LINE_STOCK, COL_STOCK + DISTANCE_SEVEN_COL, "[ %-2s%s ]", obtemCharValor(monte->cartaVisivel.valor), obtemCharNaipe(monte->cartaVisivel.naipe));
    }
    else
    {
        // desenha monte visualizado vazio
        attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca
        mvprintw(LINE_STOCK, COL_STOCK + DISTANCE_SEVEN_COL, "[     ]");
    }

    refresh(); // Atualizando a tela para exibir as mudancas
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

        mvprintw(START_LINE_SEVEN_COL - 1, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "Col. %d", col);

        attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

        if (coluna[col].numCartasfaceDown == 0 && coluna[col].numCartasfaceUp == 0) // verifica se coluna esta vazia
        {
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[     ]");
            continue;
        }

        for (row = 0; row < coluna[col].numCartasfaceDown; row++)
        {
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[  ?  ]");
        }

        if (coluna[col].numCartasfaceUp == 0) // verifica se fila de cartas visiveis esta vazia (nao deveria entrar aqui, pois isso so pode ocorrer quando toda coluna vazia)
        {
            continue;
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
            mvprintw(START_LINE_SEVEN_COL + row, START_COL_SEVEN_COL + col * DISTANCE_SEVEN_COL, "[ %-2s%s ]", obtemCharValor(carta.valor), obtemCharNaipe(carta.naipe));
            row++;
        }
        // recupera fila
        while (!vaziaFilaEnc(tempFila))
        {
            Carta carta = desenfileiraFilaEnc(tempFila);
            enfileiraFilaEnc(coluna[col].faceUp, carta);
        }
    }
    refresh(); // Atualizando a tela para exibir as mudanças
}

void desenhaTelaVencido(void)
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    mvprintw(MAX_LINES - (20), (MAX_COL - strlen("  Paciência (Klondike)   ")) / 2, "\u2665  Paciência (Klondike) \u2666");

    mvprintw(MAX_LINES - 14, (MAX_COL - strlen("Você venceu o jogo! Parabéns!")) / 2, "Você venceu o jogo! Parabéns!");

    mvprintw(MAX_LINES - 6, (MAX_COL - strlen("Digite \"exit\" para sair")) / 2, "Digite \"exit\" para sair");
}

void desenhaTelaMenu(void)
{
    attron(COLOR_PAIR(WHITE_PAIR)); // Ativando a cor branca

    mvprintw(MAX_LINES - (20), (MAX_COL - strlen("  Paciência (Klondike)   ")) / 2, "\u2665  Paciência (Klondike) \u2666");

    mvprintw(MAX_LINES - 18, 2, "Comandos:");
    mvprintw(MAX_LINES - 16, 2, "Compra de carta: 'c'");
    mvprintw(MAX_LINES - 14, 2, "Movimentação: 'm' + [ORIGEM]: 'm' (para monte) ou 'c<i>' (para coluna) +");
    mvprintw(MAX_LINES - 13, 2, "                 [DESTINO]: 'f<j>' (para fundação) ou 'c<k>' (para coluna)");
    mvprintw(MAX_LINES - 11, 2, "Movimentação em massa (entre colunas): 'm' + [NUM CARTAS]: '<n>' + ");
    mvprintw(MAX_LINES - 10, 2, "                                       [ORIGEM]: 'c<i>' (para coluna) + ");
    mvprintw(MAX_LINES - 9, 2, "                                       [DESTINO]: 'c<k>' (para coluna)");

    mvprintw(MAX_LINES - 5, 2, "Digite \"start\" parta começar ou \"exit\" para sair");

    refresh(); // Atualizando a tela para exibir as mudancas
}

void initInterface(void)
{
    setlocale(LC_CTYPE, "");
    // Inicializa a ncurses
    initscr();
    start_color();

    // Define uma cor cinza personalizada mais clara
    init_color(CUSTOM_GRAY, 500, 500, 500);

    // Define pares de cores com fundo cinza
    init_pair(BLACK_PAIR, COLOR_BLACK, CUSTOM_GRAY); // Texto preto com fundo cinza
    init_pair(RED_PAIR, COLOR_RED, CUSTOM_GRAY);     // Texto vermelho com fundo cinza
    init_pair(WHITE_PAIR, COLOR_WHITE, CUSTOM_GRAY); // Texto branco com fundo cinza

    // Limpa a tela
    clear();

    desenhaRetangulo(0, 0, MAX_LINES, MAX_COL);
    preencheRetangulo(0, 0, MAX_LINES, MAX_COL);
}
