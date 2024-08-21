#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include "interface.h"

typedef struct
{
    char source;
    char destination;
    int source_index;
    int destination_index;
    int source_amount;
    int success;
    char cmd;
} ParseEntrada;

Coluna coluna[NUM_COLUNAS];

Fundacao fundacao[NUM_FUNDACOES];

MonteCompra monte;

Baralho *baralhoCartas;

int estadoJogo;

void acrescentaCartaVisivelColuna(Coluna *coluna, Carta carta);

void montaJogo(void);

void desmontaJogo(void);

enum statusGame obtemStatusJogo()
{
    return (estadoJogo);
}

void defineStatusJogo(enum statusGame status)
{
    estadoJogo = status;
}

void criaColunas(void)
{
    int i;

    for (i = 0; i < NUM_COLUNAS; i++)
    {
        coluna[i].faceDown = criaPilhaEnc();
        coluna[i].numCartasfaceDown = 0;

        coluna[i].faceUp = criaFilaEnc();
        coluna[i].numCartasfaceUp = 0;
    }
}

void destroiColunas(void)
{
    int i;

    for (i = 0; i < NUM_COLUNAS; i++)
    {
        destroiPilhaEnc(coluna[i].faceDown);
        destroiFilaEnc(coluna[i].faceUp);
    }
}

void criaFundacoes(void)
{
    int i;
    for (i = 0; i < NUM_FUNDACOES; i++)
    {
        fundacao[i].cartas = criaPilhaEnc();
        fundacao[i].naipe = 0;
        fundacao[i].numCartas = 0;
    }
}

void destroiFundacoes(void)
{
    int i;
    for (i = 0; i < NUM_FUNDACOES; i++)
    {
        destroiPilhaEnc(fundacao[i].cartas);
    }
}

void criaMonteCompra(Baralho *baralhoCartas)
{
    monte.oculto = criaFilaEnc();
    monte.visualizado = criaFilaEnc();
    monte.cartaVisivel.valor = 0;
    monte.numCartas = 0;

    // move baralho restante para monte oculto
    while (!vaziaFilaEnc(baralhoCartas))
    {
        Carta carta = desenfileiraFilaEnc(baralhoCartas);
        enfileiraFilaEnc(monte.oculto, carta);
        monte.numCartas++;
    }
}

void destroiMonteCompra()
{
    destroiFilaEnc(monte.oculto);
    destroiFilaEnc(monte.visualizado);
}

int compraCarta(void)
{
    if (monte.numCartas == 0)
    {
        return -1;
    }

    if (!vaziaFilaEnc(monte.oculto))
    {
        // ha cartas para exibir

        if (monte.cartaVisivel.valor != 0)
        {
            // ja ha carta visivel -> mover para monte visualizado
            enfileiraFilaEnc(monte.visualizado, monte.cartaVisivel);
        }

        // sera exibida a proxima carta
        monte.cartaVisivel = desenfileiraFilaEnc(monte.oculto);
    }
    else
    {
        // todas cartas exibidas

        // move baralho visualizado para monte oculto
        while (!vaziaFilaEnc(monte.visualizado))
        {
            Carta carta = desenfileiraFilaEnc(monte.visualizado);
            enfileiraFilaEnc(monte.oculto, carta);
        }

        // move carta visivel para monte oculto
        enfileiraFilaEnc(monte.oculto, monte.cartaVisivel);
        // sinaliza que nao ha carta visivel
        monte.cartaVisivel.valor = 0;
    }

    return 0;
}

void populaColunas(Baralho *baralhoCartas)
{
    int i, j, tamanhoColuna;
    Carta carta;

    for (i = 0; i < NUM_COLUNAS; i++)
    {
        tamanhoColuna = i + 1;
        for (j = 1; j < tamanhoColuna; j++) // comeca em 1 para desconsiderar carta visivel
        {
            carta = desenfileiraFilaEnc(baralhoCartas);
            empilhaPilhaEnc(coluna[i].faceDown, carta); // deve-se imaginar que o topo da pilha sera a primeira carta a ser exibida
            coluna[i].numCartasfaceDown++;
        }
        carta = desenfileiraFilaEnc(baralhoCartas);
        enfileiraFilaEnc(coluna[i].faceUp, carta);
        coluna[i].numCartasfaceUp++;
    }
}

int insereCartaFundacao(Carta carta, int indiceFundacao)
{
    // verificaca se o indice da fundacao fora do intervalo permitido
    if (indiceFundacao < 0 || indiceFundacao >= NUM_FUNDACOES)
    {
        return -1; // Indice fora da faixa valida
    }

    // verifica se ja existe uma fundacao com o mesmo naipe, diferente da escolhida
    for (int i = 0; i < NUM_FUNDACOES; i++)
    {
        if (fundacao[i].naipe == carta.naipe && indiceFundacao != i)
        {
            return -1; // ja ha outra fundacao com o mesmo naipe
        }
    }

    // verifica se a fundacao escolhida nao esta vazia e tem um naipe diferente
    if (fundacao[indiceFundacao].naipe != 0 && fundacao[indiceFundacao].naipe != carta.naipe)
    {
        return -1; // nao pode empilhar carta de naipe diferente
    }

    // se a fundacao esta vazia, a primeira carta deve ser um 'A'
    if (fundacao[indiceFundacao].naipe == 0)
    {
        if (carta.valor != 1)
        {
            return -1; // a primeira carta deve ser um 'A'
        }

        fundacao[indiceFundacao].naipe = carta.naipe; // define o naipe na fundacao
    }
    else
    {
        // fundacao nao esta vazia, entao verifica se carta nova seguira sequencia crescente
        Carta cartaFund = desempilhaPilhaEnc(fundacao[indiceFundacao].cartas);
        if (carta.valor != cartaFund.valor + 1) // proxima carta a ser empilhada tem valor da carta ja empilhada + 1
        {
            // nao corresponde a sequencia correta, logo, nao pode ser colocada na fundacao

            empilhaPilhaEnc(fundacao[indiceFundacao].cartas, cartaFund); // reempilha ultima carta da fundacao

            return -1; // acusa erro
        }
    }

    // se chegou aqui, todas as condicoes de erro foram verificadas e validadas

    // empilha a carta na fundacao
    empilhaPilhaEnc(fundacao[indiceFundacao].cartas, carta);
    fundacao[indiceFundacao].numCartas++;

    return 0; // operacao bem-sucedida
}

void verificaAvancoJogo(void)
{
    int i, count;
    for (i = 0; i < NUM_FUNDACOES; i++)
    {
        count += fundacao[i].numCartas; // acumula o numero de cartas em cada fundacao
    }

    if (count == NUM_CARTAS_BARALHO) // verifica se o numero de cartas nas fundacoes corresponde ao numero de cartas do baralho
    {
        // todas as cartas foram organizadas nas fundacoes, logo, jogo vencido
        defineStatusJogo(VENCIDO);
    }
}

// Funcao que verifica se cartas formarao cores intercaladas
int seqValidaNaipes(char naipeOrigem, char naipeNovo)
{
    return ((((naipeOrigem == 'P') || (naipeOrigem == 'E')) &&
             ((naipeNovo == 'C') || (naipeNovo == 'O'))) ||
            (((naipeOrigem == 'C') || (naipeOrigem == 'O')) &&
             ((naipeNovo == 'P') || (naipeNovo == 'E'))));
}

int insereCartaColuna(Carta carta, int indiceColuna)
{
    if ((indiceColuna < 0) || (indiceColuna >= NUM_COLUNAS))
    {
        // indice fora da faixa valida
        return -1;
    }

    // verifica se coluna esta completamente vazia e se carta a ser movida eh rei
    if (((coluna[indiceColuna].numCartasfaceDown == 0) && (coluna[indiceColuna].numCartasfaceUp == 0)) && (carta.valor != 13))
    {
        // so pode mover rei para coluna vazia
        return -1;
    }

    if (coluna[indiceColuna].numCartasfaceUp == 0) // unica opcao possivel eh mover o rei
    {
        acrescentaCartaVisivelColuna(&coluna[indiceColuna], carta);
    }
    else // caso normal
    {
        // obtem ultima carta visivel da coluna
        Carta cartaFinal = finalFilaEnc(coluna[indiceColuna].faceUp);

        // verifica se cartas formam sequencia decrescente e cores intercaladas
        if ((cartaFinal.valor == carta.valor + 1) && seqValidaNaipes(cartaFinal.naipe, carta.naipe))
        {
            // cumpre requisitos de sequencia
            acrescentaCartaVisivelColuna(&coluna[indiceColuna], carta);
        }
        else
        {
            // nao cumpre requisitos de sequencia
            return -1;
        }
    }

    return 0;
}

void gameLoop(void)
{
    // Loop principal
    while (1)
    {
        switch (obtemStatusJogo())
        {
        case MENU:
            desenhaTelaMenu();

            break;

        case EM_JOGO:
            limpaTela();

            desenhaMonteCompra(&monte);

            desenhaColunas(coluna);

            desenhaFundacoes(fundacao);

            verificaAvancoJogo();

            break;

        case VENCIDO:
            limpaTela();

            desenhaTelaVencido();

            break;
        }

        promptComando();
    }
}

int moveCartaComprada(char tipoDestino, int indexDestino)
{
    // verifica se ha carta comprada para mover
    if (monte.cartaVisivel.valor == 0)
        return -1;

    // ha carta visivel para ser movida, entao procede com movimento
    switch (tipoDestino)
    {
    case 'c':
        if (insereCartaColuna(monte.cartaVisivel, indexDestino) != 0) // move carta
            return -1;
        break;

    case 'f':
        if (insereCartaFundacao(monte.cartaVisivel, indexDestino) != 0) // move carta
            return -1;
        break;

    default:
        return -1;
        break;
    }

    // invalida uso da carta ja movida
    monte.cartaVisivel.valor = 0;
    monte.cartaVisivel.naipe = 0;

    // verifica se ha mais cartas ja compradas para tornar visivel
    int tamanhoMonteVisualizado = tamanhoFilaEnc(monte.visualizado);

    if (tamanhoMonteVisualizado > 0)
    {
        // deve-se mover ultima carta que entrou no monte visualizado para a posicao de carta visivel
        FilaEnc *monteAux = criaFilaEnc(); // cria fila auxiliar

        // move-se todas as cartas do monte visualizado para uma fila auxiliar, exceto a ultima carta
        for (int i = 0; i < (tamanhoMonteVisualizado - 1); i++)
        {
            Carta carta = desenfileiraFilaEnc(monte.visualizado);

            enfileiraFilaEnc(monteAux, carta);
        }

        // torna a ultima carta visivel
        monte.cartaVisivel = desenfileiraFilaEnc(monte.visualizado);

        // recupera monte visualizado
        while (!vaziaFilaEnc(monteAux))
        {
            Carta carta = desenfileiraFilaEnc(monteAux);
            enfileiraFilaEnc(monte.visualizado, carta);
        }
    }

    return 0;
}

Carta removeCartaVisivelColuna(Coluna *coluna)
{
    Carta carta = desenfileiraFilaEnc(coluna->faceUp);
    coluna->numCartasfaceUp--;
    return carta;
}

Carta removeCartaOcultaColuna(Coluna *coluna)
{
    Carta carta = desempilhaPilhaEnc(coluna->faceDown);
    coluna->numCartasfaceDown--;
    return carta;
}

void acrescentaCartaVisivelColuna(Coluna *coluna, Carta carta)
{
    enfileiraFilaEnc(coluna->faceUp, carta);
    coluna->numCartasfaceUp++;
}

int moveCartaColuna(int numCartas, int indexColunaOrigem, char tipoDestino, int indexDestino)
{
    int i;
    Carta cartaMovida;

    if (numCartas <= 0 || (numCartas > coluna[indexColunaOrigem].numCartasfaceUp)) // numero de cartas invalido ou solicitado para mover mais cartas que as disponiveis
        return -1;

    if (tipoDestino != 'c' && tipoDestino != 'f') // so eh permitido mover de uma coluna para outra coluna ou fundacao
        return -1;

    if (numCartas > coluna[indexColunaOrigem].numCartasfaceUp) // verifica se a solicitacao ultrapassa o numero de cartas disponiveis
        return -1;

    if (numCartas > 1 && tipoDestino != 'c') // mover mais de uma carta so tem sentido entre colunas
        return -1;

    int numCartasIntactas = coluna[indexColunaOrigem].numCartasfaceUp - numCartas; // calcula o numero de cartas visiveis que permanecerao na coluna

    FilaEnc *filaCartasIntactas = criaFilaEnc(); // cria fila para guardar cartas que devem ficar na coluna

    for (i = 0; i < numCartasIntactas; i++) // aloca para outra fila cartas que devem ficar na coluna
    {
        Carta carta = removeCartaVisivelColuna(&coluna[indexColunaOrigem]);
        enfileiraFilaEnc(filaCartasIntactas, carta);
    }

    // aqui so restam na coluna visivel as cartas que devem ser movidas para o destino

    if (numCartas == 1) // verifica se apenas uma carta deve ser movida
    {
        cartaMovida = removeCartaVisivelColuna(&coluna[indexColunaOrigem]);

        switch (tipoDestino)
        {
        case 'c':                                                  // move para outra coluna
            if (insereCartaColuna(cartaMovida, indexDestino) != 0) // move carta para outra coluna
            {
                // operacao malsucedida, entao deve retornar todas as cartas para a fila

                while (!vaziaFilaEnc(filaCartasIntactas)) // primeiro as cartas da fila auxiliar que deve ficar na fila
                {
                    Carta carta = desenfileiraFilaEnc(filaCartasIntactas);
                    acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], carta);
                }

                acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], cartaMovida); // por fim, a carta que seria movida

                return -1;
            }
            break;

        case 'f': // move para uma fundacao

            if (insereCartaFundacao(cartaMovida, indexDestino) != 0) // move carta para uma fundacao
            {
                // operacao malsucedida, entao deve retornar todas as cartas para a fila

                while (!vaziaFilaEnc(filaCartasIntactas)) // primeiro as cartas da fila auxiliar que deve ficar na fila
                {
                    Carta carta = desenfileiraFilaEnc(filaCartasIntactas);
                    acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], carta);
                }

                acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], cartaMovida); // por fim, a carta que seria movida

                return -1;
            }
            break;
        }
    }
    else // deve movimentar multiplas cartas
    {
        // nesse ponto, certamente destino sera outra coluna

        cartaMovida = removeCartaVisivelColuna(&coluna[indexColunaOrigem]); // obtem primeira carta a ser movida

        if (insereCartaColuna(cartaMovida, indexDestino) == 0) // insere a primeira carta na coluna de destino
        {
            // operacao bem-sucedida
            // nesse caso, nao havera problema de combinacao de naipes para as proximas, pois ja estavam em sequencia na mesma coluna

            for (int i = 0; i < numCartas - 1; i++) // move as cartas restantes, descontando a primeira ja movida
            {
                cartaMovida = removeCartaVisivelColuna(&coluna[indexColunaOrigem]); // obtem proxima carta a ser movida
                if (insereCartaColuna(cartaMovida, indexDestino) != 0)
                {
                    // nao devem ocorrer erros
                }
            }
        }
        else
        {
            // operacao malsucedida, entao deve retornar todas as cartas para a fila

            // movimenta para fila de cartas intactas as cartas que nao serao mais movidas

            // primeiro a primeira carta ja removida da fila original, usada para teste
            enfileiraFilaEnc(filaCartasIntactas, cartaMovida);

            // em seguida, as cartas na sequencia dela, que tambem seriam movidas

            for (i = 0; i < numCartas - 1; i++)
            {
                Carta carta = removeCartaVisivelColuna(&coluna[indexColunaOrigem]);
                enfileiraFilaEnc(filaCartasIntactas, carta);
            }

            // aqui a fila de cartas visiveis da coluna origem deve estar vazia e copiada para a fila de cartas intactas
            // move-se as cartas intactas para a coluna original

            while (!vaziaFilaEnc(filaCartasIntactas))
            {
                Carta carta = desenfileiraFilaEnc(filaCartasIntactas);
                acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], carta);
            }

            return -1;
        }
    }

    // recupera para a coluna origem as cartas que nao foram movidas
    while (!vaziaFilaEnc(filaCartasIntactas))
    {
        Carta carta = desenfileiraFilaEnc(filaCartasIntactas);
        acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], carta);
    }

    // verifica se cartas visiveis da coluna terminaram e se ha cartas ocultas para exibir
    if (coluna[indexColunaOrigem].numCartasfaceUp == 0 && coluna[indexColunaOrigem].numCartasfaceDown > 0)
    {
        // exibe a carta seguinte
        Carta carta = removeCartaOcultaColuna(&coluna[indexColunaOrigem]);
        acrescentaCartaVisivelColuna(&coluna[indexColunaOrigem], carta);
    }

    return 0;
}

ParseEntrada parse_input(char *command)
{
    ParseEntrada parsed;
    parsed.success = 1;
    parsed.source_amount = 1;

    // parser patterns
    char *pattern_multi_move = "m%dc%d c%d"; // multiplas cartas entre colunas
    char *pattern_single_move = "mc%d %c%d"; // uma carta da coluna para qualquer
    char *pattern_pile_move = "mm%c%d";      // move da pilha de compra para qualquer
    char *pattern_pile = "c";                // compra carta
    char *pattern_exit = "exit";             // compra carta
    char *pattern_start = "start";           // compra carta

    if (sscanf(command, pattern_multi_move, &parsed.source_amount,
               &parsed.source_index, &parsed.destination_index) == 3)
    {
        parsed.source = 'c';
        parsed.destination = 'c';
    }
    else if (sscanf(command, pattern_single_move, &parsed.source_index,
                    &parsed.destination, &parsed.destination_index) == 3)
    {
        parsed.source = 'c';
    }
    else if (sscanf(command, pattern_pile_move, &parsed.destination,
                    &parsed.destination_index) == 2)
    {
        parsed.source = 'm';
    }
    else if (strcmp(command, pattern_pile) == 0)
    {
        parsed.source = 'p';
    }
    else if (strcmp(command, pattern_exit) == 0)
    {
        parsed.cmd = 'e';
    }
    else if (strcmp(command, pattern_start) == 0)
    {
        parsed.cmd = 's';
    }
    else
    {
        parsed.success = 0;
    }
    return parsed;
}

void processaComando(char *comando)
{
    ParseEntrada result = parse_input(comando);

    switch (obtemStatusJogo())
    {
    case MENU:
        if (result.cmd == 'e')
        {
            endwin(); // Finaliza a ncurses
            exit(0);
        }

        if (result.cmd == 's')
        {
            montaJogo();
            defineStatusJogo(EM_JOGO);
        }
        break;

    case VENCIDO:
        if (result.cmd == 'e')
        {
            desmontaJogo();
            endwin(); // Finaliza a ncurses
            exit(0);
        }
        break;

    case EM_JOGO:

        if (result.success)
        {
            if (result.cmd == 'e')
            {
                desmontaJogo();
                endwin(); // Finaliza a ncurses
                exit(0);
            }

            // verifica destinos validos
            else if ((result.destination == 'c' && ((result.destination_index >= NUM_COLUNAS) || (result.destination_index < 0))) || (result.destination == 'f' && ((result.destination_index >= NUM_FUNDACOES) || (result.destination_index < 0))))
            {
                exibeMsgStatus("Coluna destino fora da faixa.");
            }

            // verifica fontes validas
            else if (result.source == 'c' && ((result.source_index >= NUM_COLUNAS) || (result.source_index < 0)))
            {
                exibeMsgStatus("Coluna fonte fora da faixa.");
            }

            else if (result.source == 'p') // compra carta
            {
                if (compraCarta() == 0)
                    exibeMsgStatus("Carta comprada");
            }
            else if (result.source == 'm') // move carta comprada
            {
                if (moveCartaComprada(result.destination, result.destination_index) == 0)
                    exibeMsgStatus("Carta movida");
                else
                    exibeMsgStatus("Erro ao mover carta");
            }
            else if (result.source == 'c') // move carta de uma coluna
            {
                if (moveCartaColuna(result.source_amount, result.source_index, result.destination, result.destination_index) == 0)
                    exibeMsgStatus("Carta movida");
                else
                    exibeMsgStatus("Erro ao mover carta");
            }
        }
        else
        {
            exibeMsgStatus("Comando desconhecido.");
        }

        break;
    }
}

void montaJogo(void)
{
    baralhoCartas = criaBaralho();

    criaColunas();

    criaFundacoes();

    embaralhaCartas(baralhoCartas);

    populaColunas(baralhoCartas);

    criaMonteCompra(baralhoCartas);
}

void desmontaJogo(void)
{
    destroiBaralho(baralhoCartas);

    destroiColunas();

    destroiFundacoes();

    destroiMonteCompra();
}

void initGame(void)
{
    defineStatusJogo(MENU);
}
