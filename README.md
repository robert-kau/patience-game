
# Klondike Solitaire Game

## Descrição

Este é um jogo de Paciência na versão Klondike, desenvolvido em C. O objetivo do jogo é mover todas as cartas para as fundações (piles) com base em suas cores e valores, começando do Ás até o Rei.

Este código foi desenvolvido como trabalho final da disciplina de Estrutura de Dados e tem como objetivo exercitar os conceitos como filas e pilhas. Essas estruturas são fundamentais para a lógica do jogo, especialmente no gerenciamento das cartas no monte, colunas e fundações.

## Funcionalidades

- **Modo Clássico**: Jogue a versão clássica do Klondike com todas as regras tradicionais.
- **Interface de Texto**: O jogo é jogado através de uma interface de linha de comando usando a biblioteca ncurses.


## Como Jogar

1. **Inicie o jogo**: Execute o arquivo executável gerado pelo código C.
2. **Movimente as cartas**: Utilize comandos de teclado para mover as cartas entre as colunas, para as fundações ou para o estoque.
3. **Objetivo**: Complete as fundações de cada naipe, movendo todas as cartas do Ás ao Rei.

### Comandos do Jogo

- **Compra de carta**: `c`
- **Movimentação**:
  - `m` + [ORIGEM]: `'m'` (para monte) ou `'c<i>'` (para coluna) + [DESTINO]: `'f<j>'` (para fundação) ou `'c<k>'` (para coluna)
- **Movimentação em massa (entre colunas)**:
  - `'m'` + [NUM CARTAS]: `'<n>'` + [ORIGEM]: `'c<i>'` (para coluna) + [DESTINO]: `'c<k>'` (para coluna)
- **Iniciar o jogo**: Digite `"start"`
- **Sair do jogo**: Digite `"exit"`

## Requisitos

- Compilador C (GCC ou equivalente)
- Sistema operacional Unix-like (Linux, macOS) ou Windows com suporte ao compilador C
- **ncurses**: A biblioteca ncurses deve estar instalada no sistema

## Como Compilar e Executar

1. Clone este repositório:
    ```bash
    git clone https://github.com/robert-kau/patience-game
    ```
2. Navegue até o diretório do projeto:
    ```bash
    cd klondike-solitaire
    ```
3. Compile o código utilizando o Makefile:
    ```bash
    make
    ```
4. Execute o jogo:
    ```bash
    ./patience_game
    ```

## Estrutura do Projeto

```bash
├── README.md           # Documentação do projeto
├── makefile            # Arquivo para automação da compilação
├── src/                # Código fonte do jogo
│   ├── main.c
│   ├── interface.c
│   ├── baralho.c
│   ├── fila_enc.c
│   ├── pilha_enc.c
│   └── paciencia.c
├── include/            # Arquivos de cabeçalho
│   ├── baralho.h
│   ├── fila_enc.h
│   ├── interface.h
│   ├── paciencia.h
│   └── pilha_enc.h
├── build/              # Arquivos compilados
```


## Regras do Jogo

Klondike é um jogo de paciência (ou solitário) que utiliza um baralho padrão de 52 cartas. O objetivo do jogo é construir quatro fundações, uma para cada naipe, do Ás ao Rei.

### Configuração Inicial

- **Monte**: Um monte de cartas viradas para baixo, com uma carta virada para cima.
- **Colunas**: Sete colunas de cartas, com a primeira coluna contendo uma carta virada para cima, a segunda coluna contendo uma carta virada para cima seguida de uma carta virada para baixo, e assim por diante até a sétima coluna.
- **Fundações**: Quatro espaços vazios onde as cartas serão movidas para construir os naipes completos de Ás a Rei.

### Regras Básicas

1. **Movendo Cartas**: 
   - Cartas podem ser movidas entre colunas, para as fundações ou do monte para uma coluna ou fundação.
   - Apenas o Rei pode ser movido para uma coluna vazia.
   - Uma carta ou sequência de cartas pode ser movida para uma coluna se a carta no topo da sequência for de valor imediatamente inferior e de naipe oposto (vermelho sobre preto, preto sobre vermelho).

2. **Construindo as Fundações**:
   - As fundações são construídas em ordem crescente, começando com o Ás e terminando com o Rei, todas do mesmo naipe.

3. **Comprando Cartas**:
   - Você pode comprar cartas do monte para tentar jogá-las em uma das colunas ou fundações. Se não houver mais jogadas possíveis com as cartas no monte, ele pode ser revirado para continuar o jogo.

4. **Objetivo**:
   - O jogo é ganho quando todas as cartas foram movidas para as fundações, organizadas por naipe do Ás ao Rei.

Essas são as regras básicas do Klondike Solitaire. O jogo pode exigir tanto habilidade quanto sorte, e o desafio está em planejar os movimentos para descobrir as cartas escondidas e construir as fundações com sucesso.

## Estruturas Principais

O jogo utiliza várias estruturas de dados para gerenciar as cartas e a lógica do jogo. As principais estruturas são:

```c
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

typedef struct
{
    char naipe;
    int valor;
} Carta;
```

- **Coluna**: Representa uma coluna no jogo, onde as cartas podem estar viradas para baixo (`faceDown`) ou para cima (`faceUp`). Cada coluna tem um número específico de cartas viradas para baixo e para cima.

- **Fundacao**: Representa uma das quatro fundações onde as cartas serão organizadas por naipe do Ás ao Rei. Cada fundação possui um naipe associado e o número de cartas nela.

- **MonteCompra**: Representa o monte de compra, que contém cartas ainda não jogadas (ocultas) e cartas já visualizadas. A estrutura mantém um controle da carta atualmente visível e o número total de cartas no monte.

- **Carta**: Representa uma única carta no jogo, com seu respectivo naipe e valor.

Essas estruturas são fundamentais para o funcionamento do jogo, permitindo que ele manipule as cartas de acordo com as regras do Klondike Solitaire.