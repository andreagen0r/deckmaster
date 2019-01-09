#include "board.h"

#include <stdio.h>

void draw_divider(void);
void draw_board(void);
void draw_ruler(void);
void draw_cards(const Board *m_board, const unsigned int m_mode);
void print_card_line(const Board *m_board, const unsigned int m_mode, const int m_row, const int m_col, const int line);
const char* get_color_card(const Board *m_board, const unsigned int m_mode, const int m_row, const int m_col);

void set_cursor_position(const int m_x, const int m_y)
{
#ifdef _WIN32
    HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position={m_x, m_y};
    SetConsoleCursorPosition(Screen, Position);
#else
    printf("\033[%d;%dH", m_y, m_x);
#endif
}

void board_init(Board *m_board)
{
    m_board->is_full = false;

    Card card = card_new_default();

    for (size_t row = 0; row < BASE_GRID_SIZE; ++row)
    {
        for (size_t col = 0; col < BASE_GRID_SIZE; ++col)
        {
            m_board->position[row][col] = card;
            m_board->is_busy[row][col] = true;
        }
    }
}

bool board_isFull(Board *m_board)
{
    if (board_count(m_board) >= (BASE_GRID_SIZE * BASE_GRID_SIZE))
    {
        m_board->is_full = true;
        return  true;
    }

    return  false;
}

size_t board_count(Board *m_board)
{
    size_t board_count = 0;

    for (size_t row = 0; row < BASE_GRID_SIZE; ++row)
    {
        for (size_t col = 0; col < BASE_GRID_SIZE; ++col)
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
    clear_screen();

    draw_board();

    draw_ruler();

    draw_cards(m_board, m_mode);
}


void draw_divider()
{
    for (int i = 0; i < BOARD_WIDTH; ++i)
    {
        printf("-");
    }

    puts("");
}

void draw_board()
{
    for(int i = 0; i < BASE_GRID_SIZE; ++i)
    {
        draw_divider();

        for(int j = 0; j < CARD_HEIGHT; j++)
        {
            for (int k = 0; k < BASE_GRID_SIZE; ++k)
            {
                printf("|");
                for (int x = 0; x < CARD_WIDTH; ++x)
                {
                    printf(" ");
                }
            }

            puts("|"); // Right edge
        }
    }

    draw_divider();
}

void draw_ruler()
{
    // Print row number
    int posY = 6;
    for (int row = 0; row < BASE_GRID_SIZE; ++row)
    {
        set_cursor_position(BOARD_WIDTH + 2, posY);
        posY += CARD_HEIGHT + 1;
        printf("%d", row);
    }

    // Print column number
    int posX = 9;
    for (int col = 0; col < BASE_GRID_SIZE; ++col)
    {
        set_cursor_position(posX, BOARD_HEIGHT + 1);
        posX += CARD_WIDTH + 1;
        printf("%d", col);
    }
    puts("\n");

    set_cursor_position(0, BOARD_HEIGHT + 3);
}


void draw_cards(const Board *m_board, const unsigned int m_mode)
{
    int card_row_offset = 0;
    int card_col_offset = 0;

    for (int row = 0; row < BASE_GRID_SIZE; ++row)
    {
        for (int col = 0; col < BASE_GRID_SIZE; ++col)
        {
            for (int card_line = 0; card_line < CARD_HEIGHT; ++card_line)
            {
                set_cursor_position(2 + card_col_offset + col, 2 + card_line + card_row_offset );

                printf("%s", get_color_card(m_board, m_mode, row, col));

                print_card_line(m_board, m_mode, row, col, card_line);
            }

            puts("\x1b[0m ");
            card_col_offset += CARD_WIDTH;
        }

        card_row_offset += CARD_HEIGHT + 1;
        card_col_offset = 0;
    }

    set_cursor_position(0, BOARD_HEIGHT + 3);

}

void print_card_line(const Board *m_board, const unsigned int m_mode, const int m_row, const int m_col, const int line)
{
    if (!m_board->is_busy[m_row][m_col])
    {
        switch (line) {
        case 0: {
            char arrow_top[8] = "";

            if (m_mode == COLOR_MODE)
            {
                memcpy(arrow_top, m_board->position[m_row][m_col].arrows[TOP] ? "⬆︎" : " ", sizeof(arrow_top));
            }
            else
            {
                memcpy(arrow_top, m_board->position[m_row][m_col].arrows[TOP] ? "^" : " ", sizeof(arrow_top));
            }

            printf("       %s       ", arrow_top);
            break;
        }


        case 4: {
            char arrow_right[8] = "";
            char arrow_left[8] = "";

            int power = m_board->position[m_row][m_col].power;

            if (m_mode == COLOR_MODE)
            {
                memcpy(arrow_right, m_board->position[m_row][m_col].arrows[RIGHT]? "➡︎" : " ", sizeof(arrow_right));
                memcpy(arrow_left, m_board->position[m_row][m_col].arrows[LEFT]? "⬅︎" : " ", sizeof(arrow_left));
            }
            else
            {
                memcpy(arrow_right, m_board->position[m_row][m_col].arrows[RIGHT]? ">" : " ", sizeof(arrow_right));
                memcpy(arrow_left, m_board->position[m_row][m_col].arrows[LEFT]? "<" : " ", sizeof(arrow_left));
            }


            if (power < 10)
            {
                printf(" %s     %d     %s ", arrow_left, power, arrow_right);
            }
            else
            {
                printf(" %s     %d    %s ", arrow_left, power, arrow_right);
            }

            break;
        }


        case 6: {
            char *m_element = NULL;
            char *m_status = NULL;

            if ((m_board->position[m_row][m_col].status != NO_STATUS) && (!m_board->is_busy[m_row][m_col]))
            {
                m_status = card_get_status(m_board->position[m_row][m_col].status);
                printf("%s", m_status);
            }
            else
            {
                m_element = card_get_element(m_board->position[m_row][m_col].element);
                printf("%s", m_element);
            }

            break;
        }


        case 8: {
            char arrow_bottom[8] = "";
            if (m_mode == COLOR_MODE)
            {
                memcpy(arrow_bottom, m_board->position[m_row][m_col].arrows[BOTTOM]? "⬇︎" : " ", sizeof(arrow_bottom));
            }
            else
            {
                memcpy(arrow_bottom, m_board->position[m_row][m_col].arrows[BOTTOM]? "v" : " ", sizeof(arrow_bottom));
            }
            printf("       %s       ", arrow_bottom);
            break;
        }

        default:
            printf("               ");
            break;

        }
    }
}

const char* get_color_card(const Board *m_board, const unsigned int m_mode, const int m_row, const int m_col)
{
    char color_reset[8] = "";
    char *color_card = color_reset;

    if (m_mode == COLOR_MODE)
    {
        memcpy(color_reset, "\x1b[0m", sizeof(color_reset));

        if (m_board->position[m_row][m_col].status == NO_STATUS)
        {
            color_card = card_get_color_element(m_board->position[m_row][m_col].element);
        }
        else
        {
            color_card = card_get_color_status(m_board->position[m_row][m_col].status);
        }
    }

    return color_card;
}
