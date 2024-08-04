#include <stdio.h>
#include <string.h>
#include "baralho.h"
#include "fila_enc.h"
#include "pilha_enc.h"
#include "paciencia.h"

Coluna coluna[NUM_COLUNAS];

Fundacao fundacao;

Baralho *baralhoCartas;

void criaColunas(void)
{
    int i;

    for(i = 0; i < NUM_COLUNAS; i++)
    {
        coluna[i].faceDown = criaPilhaEnc();
        coluna[i].faceUp = criaFilaEnc();
    }
}

void criaFundacoes(void)
{
    fundacao.espadas = criaPilhaEnc();
    fundacao.paus = criaPilhaEnc();
    fundacao.copas = criaPilhaEnc();
    fundacao.ouros = criaPilhaEnc();
}

void populaColunas(Baralho *baralhoCartas)
{
    int i, j, tamanhoColuna;
    Carta carta;

    for(i = 0; i < NUM_COLUNAS; i++)
    {
        tamanhoColuna = i + 1;
        for(j = 1; j < tamanhoColuna; j++) //comeca em 1 para desconsiderar carta visivez
        {
            carta = desenfileiraFilaEnc(baralhoCartas);
            empilhaPilhaEnc(coluna[i].faceDown, carta);
        }
        carta = desenfileiraFilaEnc(baralhoCartas);
        enfileiraFilaEnc(coluna[i].faceUp, carta);
    }
}

void imprimeColuna(Coluna coluna) {
    PilhaEnc *tempPilha = criaPilhaEnc();
    FilaEnc *tempFila = criaFilaEnc();

    // Imprimindo cartas viradas para baixo (faceDown)
    printf("Cartas viradas para baixo:\n");
    while (!vaziaPilhaEnc(coluna.faceDown)) {
        Carta carta = desempilhaPilhaEnc(coluna.faceDown);
        empilhaPilhaEnc(tempPilha, carta);
        printf("%c | %d\n", carta.naipe, carta.valor);
    }

    while (!vaziaPilhaEnc(tempPilha)) {
        Carta carta = desempilhaPilhaEnc(tempPilha);
        empilhaPilhaEnc(coluna.faceDown, carta);
    }

    // Imprimindo cartas viradas para cima (faceUp)
    printf("Cartas viradas para cima:\n");
    while (!vaziaFilaEnc(coluna.faceUp)) {
        Carta carta = desenfileiraFilaEnc(coluna.faceUp);
        enfileiraFilaEnc(tempFila, carta);
        printf("%c | %d\n", carta.naipe, carta.valor);
    }

    while (!vaziaFilaEnc(tempFila)) {
        Carta carta = desenfileiraFilaEnc(tempFila);
        enfileiraFilaEnc(coluna.faceUp, carta);
    }

    // Limpando estruturas temporárias
    destroiPilhaEnc(tempPilha);
    destroiFilaEnc(tempFila);
}

void imprimeTodasColunas(Coluna colunas[], int numColunas) {
    for (int i = 0; i < numColunas; i++) {
        printf("Coluna %d:\n", i + 1);
        imprimeColuna(colunas[i]);
        printf("\n");
    }
}



void montaJogo(void)
{
    baralhoCartas = criaBaralho();

    criaColunas();

    criaFundacoes();

    embaralhaCartas(baralhoCartas);

    populaColunas(baralhoCartas);

    imprimeTodasColunas(coluna, NUM_COLUNAS);

}

