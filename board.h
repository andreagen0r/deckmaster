#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "card.h"
#include "point.h"

#define BASE_GRID_SIZE 3
#define BOARD_WIDTH (CARD_WIDTH * BASE_GRID_SIZE) + (BASE_GRID_SIZE + 1)
#define BOARD_HEIGHT (CARD_HEIGHT * BASE_GRID_SIZE) + (BASE_GRID_SIZE + 1)

// Protótipo da função para limpar tela que está no main.c
extern void clear_screen(void);

typedef struct _board {
    Card position[BASE_GRID_SIZE][BASE_GRID_SIZE];
    bool is_busy[BASE_GRID_SIZE][BASE_GRID_SIZE];
    bool is_full;
} Board;

// Inicializa o tabuleiro com cartas default
void board_init(Board *m_board);

// Retorna "true" se todas as casas do tabuleiro estiverem ocupadas, senão retorna "false"
bool board_isFull(Board *m_board);

// Retorna a quantidade de cartas posicionadas no tabuleiro
size_t board_count(Board *m_board);

// Retorna "true" se a posição estiver vazia, senão retorna "false"
bool board_space_isEmpty(Board *m_board, const Point m_position);

// Retorna "true" se a carta for inserida no tabuleiro, senão retorna "false"
bool board_add_card(Board *m_board, Card card, const Point m_position);

// Retorna "true" se a carta for removida do tabuleiro, senão retorna "false"
bool board_remove_card(Board *m_board, const Point m_position);

void board_render(const Board *m_board, const unsigned int m_mode);

#endif // BOARD_H
