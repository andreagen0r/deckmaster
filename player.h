#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>

typedef struct _player {
    char name[100];
    int score;
} Player;

// Inicializa dados do jogador
void player_init(Player *m_player, const char *m_name);

// Adiciona pontuação ao jogador
void player_add_score(Player *m_player, const int m_score);

// Subtrai pontuação do jogador com valor passado
void player_subtract_score(Player *m_player, const int m_score);

#endif // PLAYER_H
