#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "point.h"
#include "board.h"
#include "pack.h"
#include "card.h"
#include "player.h"

// Protótipo da função para limpar tela que está no main.c
extern void limparTela(void);

// Inicializa o jogo
void game_init(void);

#endif // GAME_H
