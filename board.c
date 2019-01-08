#include "board.h"

#include <stdio.h>

#define BoardSizeGrid 3

void empty_space(void);
void empty_line(const Board *m_board, const size_t m_row, const size_t m_mode);
void board_render_columns(const Board *m_board, const size_t m_row, const size_t m_mode);


void board_init(Board *m_board)
{
    m_board->is_full = false;

    Card card = card_new_default();

    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        for (size_t col = 0; col < BoardSizeGrid; ++col)
        {
            m_board->position[row][col] = card;
            m_board->is_busy[row][col] = true;
        }
    }
}

void board_log(Board *m_board)
{
    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        for (size_t col = 0; col < BoardSizeGrid; ++col)
        {
            printf("CARTA: [%zu, %zu] - %s \n", row, col, m_board->is_busy[row][col] ? "VAZIO" : "OCUPADO");
            card_log(m_board->position[row][col]);
        }
    }
}

bool board_isFull(Board *m_board)
{
    size_t board_count = 0;

    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        for (size_t col =0; col < BoardSizeGrid; ++col)
        {
            if (!m_board->is_busy[row][col])
            {
                board_count++;
            }
        }
    }

    if (board_count >= 9)
    {
        m_board->is_full = true;
        return  true;
    }

    return  false;
}

size_t board_get_count(Board *m_board)
{
    size_t board_count = 0;

    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        for (size_t col = 0; col < BoardSizeGrid; ++col)
        {
            if (!m_board->is_busy[row][col])
            {
                board_count++;
            }
        }
    }

    return board_count;
}

bool board_space_isEmpty(Board *m_board, const Point m_position)
{
    return (m_board->is_busy[m_position.x][m_position.y]);
}

bool board_add_card(Board *m_board, Card card, const Point m_position)
{
    if (board_space_isEmpty(m_board, m_position) || board_isFull(m_board) )
    {
        m_board->position[m_position.x][m_position.y] = card;
        m_board->is_busy[m_position.x][m_position.y] = false; // Não está mais vazio
        return true;
    }

    return false;
}

bool board_remove_card(Board *m_board, const Point m_position)
{
    if (!board_space_isEmpty(m_board, m_position) )
    {
        Card card = card_new_default();

        m_board->position[m_position.x][m_position.y] = card;
        m_board->is_busy[m_position.x][m_position.y] = true;

        return true;
    }

    return false;
}

void board_render(const Board *m_board, const unsigned int m_mode)
{
    limparTela();

    puts("");

    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        puts("----------------------------------------------");
        board_render_columns(m_board, row, m_mode);
    }

    puts("----------------------------------------------");

    // Imprime o número da coluna
    for (size_t row = 0; row < BoardSizeGrid; ++row)
    {
        printf("%8zu       ", row);
    }

    puts("\n");
}

void empty_space()
{
    char space[1] = "";
    printf("|"); // Borda esquerda
    printf("%14s", space);
}

void empty_line(const Board *m_board, const size_t m_row, const size_t m_mode)
{
    for (size_t col = 0; col < BoardSizeGrid; ++col)
    {
        char space[1] = "";
        char color_reset[8] = "";
        char color_card[16] = "";

        if (!m_board->is_busy[m_row][col])
        {
            if (m_mode == COLOR_MODE)
            {
                memcpy(color_reset, "\x1b[0m", sizeof(color_reset));

                // Obtem a cor da carta de acordo com o tipo do elemento ou status
                if (m_board->position[m_row][col].status == NO_STATUS)
                {
                    memcpy(color_card, card_get_color_element(m_board->position[m_row][col].element), sizeof (color_card));
                }
                else
                {
                    memcpy(color_card, card_get_color_status(m_board->position[m_row][col].status), sizeof (color_card));
                }
            }

            printf("|"); // Borda esquerda

            // Imprime a linha
            printf("%s%s%14s%s" , color_reset, color_card, space, color_reset);
        }
        else
        {
            empty_space();
        }
    }
}

void board_render_columns(const Board *m_board, const size_t m_row, const size_t m_mode)
{
    char *m_element = NULL;
    char *m_status = NULL;

    char arrow_top[8] = "";
    char arrow_right[8] = "";
    char arrow_bottom[8] = "";
    char arrow_left[8] = "";

    char color_reset[8] = "";
    char *color_card = color_reset;

    if (m_mode == COLOR_MODE)
    {
        memcpy(color_reset, "\x1b[0m", sizeof(color_reset));
    }

    // CARTA LINHA 0
    for (size_t col = 0; col < BoardSizeGrid; ++col)
    {
        if (!m_board->is_busy[m_row][col])
        {
            if (m_mode == COLOR_MODE)
            {
                // Obtem a cor da carta de acordo com o tipo do elemento ou status
                if (m_board->position[m_row][col].status == NO_STATUS)
                {
                    color_card = card_get_color_element(m_board->position[m_row][col].element);
                }
                else
                {
                    color_card = card_get_color_status(m_board->position[m_row][col].status);
                }

                memcpy(arrow_top, m_board->position[m_row][col].arrows[TOP]? "⬆︎" : " ", sizeof(arrow_top));
            }
            else
            {
                memcpy(arrow_top, m_board->position[m_row][col].arrows[TOP]? "^" : " ", sizeof(arrow_top));
            }

            printf("|"); // Borda esquerda

            // Imprime a linha
            printf("%s%s      %s       %s",color_reset, color_card, arrow_top, color_reset);
        }
        else
        {
            empty_space();
        }
    }
    puts("|"); // Borda direita

    // CARTA LINHA 1
    empty_line(m_board, m_row, m_mode);
    puts("|"); // Borda direita

    // CARTA LINHA 2
    empty_line(m_board, m_row, m_mode);
    puts("|"); // Borda direita

    // CARTA LINHA 3
    for (size_t col = 0; col < BoardSizeGrid; ++col)
    {
        int power = m_board->position[m_row][col].power;

        if (m_mode == COLOR_MODE)
        {
            // Obtem a cor da carta de acordo com o tipo do elemento ou status
            if (m_board->position[m_row][col].status == NO_STATUS)
            {
                color_card = card_get_color_element(m_board->position[m_row][col].element);
            }
            else
            {
                color_card = card_get_color_status(m_board->position[m_row][col].status);
            }

            memcpy(arrow_right, m_board->position[m_row][col].arrows[RIGHT]? "➡︎" : " ", sizeof(arrow_right));
            memcpy(arrow_left, m_board->position[m_row][col].arrows[LEFT]? "⬅︎" : " ", sizeof(arrow_left));
        }
        else
        {
            memcpy(arrow_right, m_board->position[m_row][col].arrows[RIGHT]? ">" : " ", sizeof(arrow_right));
            memcpy(arrow_left, m_board->position[m_row][col].arrows[LEFT]? "<" : " ", sizeof(arrow_left));
        }


        if (!m_board->is_busy[m_row][col])
        {
            if (power > 0)
            {
                // Verifica o valor e imprime com o espaço correto para não deformar o desenho do tabuleiro
                if (power < 10)
                {
                    printf("|"); // Borda esquerda

                    // Imprime setas e poder como unidade
                    printf("%s%s %s    %d     %s %s", color_reset, color_card, arrow_left, power, arrow_right, color_reset);
                }
                else
                {
                    printf("|"); // Borda esquerda

                    // Imprime setas e poder como dezena
                    printf("%s%s %s    %d    %s %s", color_reset, color_card, arrow_left, power, arrow_right, color_reset);
                }
            }
            else
            {
                empty_line(m_board, m_row, m_mode);
            }
        }
        else
        {
            empty_space();
        }
    }
    puts("|"); // Borda direita

    // CARTA LINHA 4 - NÚMERO DA LINHA
    empty_line(m_board, m_row, m_mode);
    printf("|%2zu\n", m_row); // Borda direita

    // CARTA LINHA 5
    for (size_t col = 0; col < BoardSizeGrid; ++col)
    {
        if (m_mode == COLOR_MODE)
        {
            // Obtem a cor da carta de acordo com o tipo do elemento ou status
            if (m_board->position[m_row][col].status == NO_STATUS)
            {
                color_card = card_get_color_element(m_board->position[m_row][col].element);
            }
            else
            {
                color_card = card_get_color_status(m_board->position[m_row][col].status);
            }
        }

        if (!m_board->is_busy[m_row][col])
        {
            // Imprime o STATUS ou ELEMENTO da carta
            if ((m_board->position[m_row][col].status != NO_STATUS) && (!m_board->is_busy[m_row][col]))
            {
                m_status = card_get_status(m_board->position[m_row][col].status);
                printf("|"); // Borda esquerda
                printf("%s%s%s%s", color_reset, color_card, m_status, color_reset);
            }
            else
            {
                m_element = card_get_element(m_board->position[m_row][col].element);
                printf("|"); // Borda esquerda
                printf("%s%s%s%s", color_reset, color_card, m_element, color_reset);
            }
        }
        else
        {
            empty_space();
        }

    }
    puts("|"); // Borda direita

    // CARTA LINHA 6
    empty_line(m_board, m_row, m_mode);
    puts("|"); // Borda direita

    // CARTA LINHA 7
    for (size_t col = 0; col < BoardSizeGrid; ++col)
    {
        if (!m_board->is_busy[m_row][col])
        {
            if (m_mode == COLOR_MODE)
            {
                // Obtem a cor da carta de acordo com o tipo do elemento ou status
                if (m_board->position[m_row][col].status == NO_STATUS)
                {
                    color_card = card_get_color_element(m_board->position[m_row][col].element);
                }
                else
                {
                    color_card = card_get_color_status(m_board->position[m_row][col].status);
                }

                memcpy(arrow_bottom, m_board->position[m_row][col].arrows[BOTTOM]? "⬇︎" : " ", sizeof(arrow_top));
            }
            else
            {
                memcpy(arrow_bottom, m_board->position[m_row][col].arrows[BOTTOM]? "v" : " ", sizeof(arrow_top));
            }

            printf("|"); // Borda esquerda

            // Imprime a linha
            printf("%s%s      %s       %s",color_reset, color_card, arrow_bottom, color_reset);
        }
        else
        {
            empty_space();
        }
    }
    puts("|"); // Borda direita
}

