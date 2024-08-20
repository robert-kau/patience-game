#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"
#include "interface.h"

typedef struct parsed_input
{
    char source;
    char destination;
    int source_index;
    int destination_index;
    int source_amount;
    int success;
    char cmd;
} parsed_input;

Coluna coluna[NUM_COLUNAS];

Fundacao fundacao[NUM_FUNDACOES];

MonteCompra monte;

Baralho *baralhoCartas;

void criaColunas(void)
{
    int i;

    for(i = 0; i < NUM_COLUNAS; i++)
    {
        coluna[i].faceDown = criaPilhaEnc();
        coluna[i].numCartasfaceDown = 0;

        coluna[i].faceUp = criaFilaEnc();
        coluna[i].numCartasfaceUp = 0;
    }
}

void criaFundacoes(void)
{
    int i;
    for(i = 0; i < NUM_FUNDACOES; i++)
    {
        fundacao[i].cartas = criaPilhaEnc();
        fundacao[i].naipe = 0;
        fundacao[i].numCartas = 0;
    }
}

void criaMonteCompra(Baralho *baralhoCartas)
{
    monte.oculto = criaFilaEnc();
    monte.visualizado = criaFilaEnc();
    monte.cartaVisivel.valor = 0;
    monte.numCartas = 0;

    //move baralho restante para monte oculto
    while (!vaziaFilaEnc(baralhoCartas)) 
    {
        Carta carta = desenfileiraFilaEnc(baralhoCartas);
        enfileiraFilaEnc(monte.oculto, carta);
        monte.numCartas++;
    }
}

int compraCarta(void)
{
    if(monte.numCartas == 0)
    {
        return -1;
    }

    if(!vaziaFilaEnc(monte.oculto))
    {
        //ha cartas para exibir

        if(monte.cartaVisivel.valor != 0)
        {
            //ja ha carta visivel -> mover para monte visualizado
            enfileiraFilaEnc(monte.visualizado, monte.cartaVisivel);
        }

        //sera exibida a proxima carta
        monte.cartaVisivel = desenfileiraFilaEnc(monte.oculto);    
    }
    else
    {
        //todas cartas exibidas

        //move baralho visualizado para monte oculto
        while (!vaziaFilaEnc(monte.visualizado)) 
        {
            Carta carta = desenfileiraFilaEnc(monte.visualizado);
            enfileiraFilaEnc(monte.oculto, carta);
        }

        //move carta visivel para monte oculto
        enfileiraFilaEnc(monte.oculto, monte.cartaVisivel);
        //sinaliza que nao ha carta visivel
        monte.cartaVisivel.valor = 0;
    }

    return 0;
}

void populaColunas(Baralho *baralhoCartas)
{
    int i, j, tamanhoColuna;
    Carta carta;

    for(i = 0; i < NUM_COLUNAS; i++)
    {
        tamanhoColuna = i + 1;
        for(j = 1; j < tamanhoColuna; j++) //comeca em 1 para desconsiderar carta visivel
        {
            carta = desenfileiraFilaEnc(baralhoCartas);
            empilhaPilhaEnc(coluna[i].faceDown, carta); //deve-se imaginar que o topo da pilha sera a primeira carta a ser exibida
            coluna[i].numCartasfaceDown++;
        }
        carta = desenfileiraFilaEnc(baralhoCartas);
        enfileiraFilaEnc(coluna[i].faceUp, carta);
        coluna[i].numCartasfaceUp++;
    }
}

// void imprimeColuna(Coluna coluna) {
//     PilhaEnc *tempPilha = criaPilhaEnc();
//     FilaEnc *tempFila = criaFilaEnc();

//     // Imprimindo cartas viradas para baixo (faceDown)
//     printf("Cartas viradas para baixo:\n");
//     while (!vaziaPilhaEnc(coluna.faceDown)) {
//         Carta carta = desempilhaPilhaEnc(coluna.faceDown);
//         empilhaPilhaEnc(tempPilha, carta);
//         printf("%c | %d\n", carta.naipe, carta.valor);
//     }

//     while (!vaziaPilhaEnc(tempPilha)) {
//         Carta carta = desempilhaPilhaEnc(tempPilha);
//         empilhaPilhaEnc(coluna.faceDown, carta);
//     }

//     // Imprimindo cartas viradas para cima (faceUp)
//     printf("Cartas viradas para cima:\n");
//     while (!vaziaFilaEnc(coluna.faceUp)) {
//         Carta carta = desenfileiraFilaEnc(coluna.faceUp);
//         enfileiraFilaEnc(tempFila, carta);
//         printf("%c | %d\n", carta.naipe, carta.valor);
//     }

//     while (!vaziaFilaEnc(tempFila)) {
//         Carta carta = desenfileiraFilaEnc(tempFila);
//         enfileiraFilaEnc(coluna.faceUp, carta);
//     }

//     // Limpando estruturas temporárias
//     destroiPilhaEnc(tempPilha);
//     destroiFilaEnc(tempFila);
// }

// void imprimeTodasColunas(Coluna colunas[], int numColunas) {
//     for (int i = 0; i < numColunas; i++) {
//         printf("Coluna %d:\n", i + 1);
//         imprimeColuna(colunas[i]);
//         printf("\n");
//     }
// }

int insereCartaFundacao(Carta carta, int indiceFundacao)
{
    //TODO: adicionar regra da progressao de valor

    if((indiceFundacao < 0) || (indiceFundacao >= NUM_FUNDACOES))
    {
        //indice fora da faixa valida
        return -1;
    }

    // Verifica se ja existe uma fundacao com o mesmo naipe diferente da escolhida
    for(int i = 0; i < NUM_FUNDACOES; i++)
    {
        if(fundacao[i].naipe == carta.naipe && indiceFundacao != i)
        {
            return -1; // Ja ha outra fundacao com o mesmo naipe
        }
    }

    // Verifica se a fundacao escolhida esta vazia ou possui o mesmo naipe
    if(fundacao[indiceFundacao].naipe == 0 || fundacao[indiceFundacao].naipe == carta.naipe)
    {
        //verifica se nao ha naipe definido para a fundacao
        if(fundacao[indiceFundacao].naipe == 0)
        {
            //nao ha naipe definido ainda

            if(carta.valor != 1) //primeira carta deve ser 'A'
                return -1;

            fundacao[indiceFundacao].naipe = carta.naipe; // Define o naipe na fundacao se estiver vazia
        }

        empilhaPilhaEnc(fundacao[indiceFundacao].cartas, carta); // Empilha a carta
        
        fundacao[indiceFundacao].numCartas++;

        return 0;
    }
    else
    {
        return -1; // Nao pode empilhar carta de naipe diferente
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
    if((indiceColuna < 0) || (indiceColuna >= NUM_COLUNAS))
    {
        //indice fora da faixa valida
        return -1;
    }

    //verifica se coluna esta completamente vazia
    if(vaziaPilhaEnc(coluna[indiceColuna].faceDown) && vaziaFilaEnc(coluna[indiceColuna].faceUp))
    {
        //so pode mover rei para coluna vazia
        if(carta.valor != 13)
        {
            return -1;
        }
    }
    else //caso normal
    {
        //obtem ultima carta visivel da coluna
        Carta cartaFinal = finalFilaEnc(coluna[indiceColuna].faceUp);

        // verifica se cartas formam sequencia decrescente e cores intercaladas
        if ((cartaFinal.valor == carta.valor + 1) && seqValidaNaipes(cartaFinal.naipe, carta.naipe))
        {
            //cumpre requisitos de sequencia
            enfileiraFilaEnc(coluna[indiceColuna].faceUp, carta);
            coluna[indiceColuna].numCartasfaceUp++;
        }
        else
        {
            //nao cumpre requisitos de sequencia
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
        desenhaMonteCompra(&monte);

        desenhaColunas(coluna);

        desenhaFundacoes(fundacao);

        promptComando();
    }
    
    endwin(); // Finaliza a ncurses
}

int moveCartaComprada(char tipoDestino, int indexDestino)
{
    //verifica se ha carta comprada para mover
    if(monte.cartaVisivel.valor == 0)
        return -1;

    //ha carta visivel para ser movida, entao procede com movimento
    switch (tipoDestino)
    {
    case 'c':
        if(insereCartaColuna(monte.cartaVisivel, indexDestino) != 0) //move carta
            return -1;
        break;
    
    case 'f':
        if(insereCartaFundacao(monte.cartaVisivel, indexDestino) != 0) //move carta
            return -1;
        break;

    default:
        return -1;
        break;
    
    }

    //invalida uso da carta ja movida
    monte.cartaVisivel.valor = 0;

    //verifica se ha mais cartas ja compradas para tornar visivel
    if(!vaziaFilaEnc(monte.visualizado)) 
    {
        //entao torna visivel a proxima
        monte.cartaVisivel = desenfileiraFilaEnc(monte.visualizado);
    }

    return 0;
}

parsed_input parse_input(char *command)
{
    parsed_input parsed;
    parsed.success = 1;
    parsed.source_amount = 1;

    // parser patterns
    char *pattern_multi_move = "m%dc%d c%d"; // multiplas cartas entre colunas
    char *pattern_single_move = "mc%d %c%d"; // uma carta da coluna para qualquer
    char *pattern_pile_move = "mm%c%d";     // move da pilha de compra para qualquer
    char *pattern_pile = "c";              // compra carta
    char *pattern_exit = "exit";             // compra carta

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
    else
    {
        parsed.success = 0;
    }
    return parsed;
}

void processaComando(char* comando) 
{
    parsed_input result = parse_input(comando);

    if (result.success) 
    {
        if (result.cmd == 'e')
        {
            endwin(); // Finaliza a ncurses
            exit(0);
        }

        //verifica destinos validos
        else if((result.destination == 'c' && ((result.destination_index >= NUM_COLUNAS) || (result.destination_index < 0)))
            || (result.destination == 'f' && ((result.destination_index >= NUM_FUNDACOES) || (result.destination_index < 0))))
        {
            exibeMsgStatus("Coluna destino fora da faixa.");

        }

        //verifica fontes validas
        else if(result.source == 'c' && ((result.source_index >= NUM_COLUNAS) || (result.source_index < 0))){
            exibeMsgStatus("Coluna fonte fora da faixa.");
        }

        else if(result.source == 'p') //compra carta
        {
            if(compraCarta() == 0)
                exibeMsgStatus("Carta comprada");

        }
        else if(result.source == 'm') //move carta comprada
        {
            if(moveCartaComprada(result.destination, result.destination_index) == 0)
                exibeMsgStatus("Carta movida");
            else
                exibeMsgStatus("Erro ao mover carta");
        }

    } 
    else 
    {
        exibeMsgStatus("Comando desconhecido.");
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

