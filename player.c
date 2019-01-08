#include "player.h"

void player_init(Player *m_player, const char *m_name)
{
    strcpy(m_player->name, m_name);
    m_player->score = 0;
}

void player_add_score(Player *m_player, const int m_score)
{
    m_player->score += m_score;
}

void player_subtract_score(Player *m_player, const int m_score)
{
    m_player->score -= m_score;

    if (m_player->score < 0)
    {
        m_player->score = 0;
    }
}
