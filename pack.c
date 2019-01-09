#include "pack.h"

#define PackSize 30

struct _pack {
    Card card;
    int _padding;
    struct _pack *next;
};

static int pack_count = 0;

Pack *pack_init()
{
    Pack *pack = NULL;
    for (int i = 0; i < PackSize; ++i)
    {
        pack_push(&pack, card_new_random());
    }
    return pack;
}

Pack *pack_newNode(const Card card)
{
    Pack *pack = (Pack*) malloc(sizeof(Pack));
    pack->card = card;
    pack->next = NULL;
    return pack;
}

bool pack_push(Pack **root, const Card card)
{
    if (pack_size() < PackSize)
    {
        Pack *pack = pack_newNode(card);
        pack->next = *root;
        *root = pack;
        pack_count++;
        return true;
    }

    return false;
}

bool pack_pop(Pack **root)
{
    if (pack_isEmpty(*root)) {
        return false;
    }

    Pack *temp = *root;
    *root = (*root)->next;
    free(temp);
    pack_count--;

    return true;
}

bool pack_isEmpty(Pack *root)
{
    return !root;
}

Card pack_top(Pack *root)
{
    return root->card;
}

int pack_size()
{
    return pack_count;
}

void pack_print_next_card(Pack *m_pack, int m_mode)
{
    if (pack_size() <= 0)
    {
        return;
    }

    Card m_card = pack_top(m_pack);

    char *m_element = NULL;

    m_element = card_get_element(m_card.element);

    char space[] = "               ";
    char border_top_bottom[16] = "----------------";
    char border_side[2] = "|";

    char color_reset[8] = "";
    char color_card[16] = "";

    char arrow_top[8] = "";
    char arrow_right[8] = "";
    char arrow_bottom[8] = "";
    char arrow_left[8] = "";

    memcpy(arrow_top, m_card.arrows[TOP]? "^" : " ", sizeof(arrow_top));
    memcpy(arrow_right, m_card.arrows[RIGHT]? ">" : " ", sizeof(arrow_right));
    memcpy(arrow_bottom, m_card.arrows[BOTTOM]? "v" : " ", sizeof(arrow_bottom));
    memcpy(arrow_left, m_card.arrows[LEFT]? "<" : " ", sizeof(arrow_left));

    if (m_mode == COLOR_MODE)
    {
        memcpy(color_reset, "\x1b[0m", sizeof(color_reset));
        strcpy(color_card, card_get_color_element(m_card.element));

        memcpy(arrow_top, m_card.arrows[TOP]? "⬆︎" : " ", sizeof(arrow_top));
        memcpy(arrow_right, m_card.arrows[RIGHT]? "➡︎" : " ", sizeof(arrow_right));
        memcpy(arrow_bottom, m_card.arrows[BOTTOM]? "⬇︎" : " ", sizeof(arrow_bottom));
        memcpy(arrow_left, m_card.arrows[LEFT]? "⬅︎" : " ", sizeof(arrow_left));

        memcpy(border_top_bottom, "", sizeof(border_top_bottom));
        memcpy(border_side, "", sizeof(border_side));
    }

    printf("\n");

    printf("%s%s\n", space, border_top_bottom);
    printf("%s%s%s%s      %s       %s%s\n", color_reset, space, border_side, color_card, arrow_top, color_reset, border_side);
    printf("%s%s%s%s              %s%s\n" , color_reset, space, border_side, color_card, color_reset, border_side);
    printf("%s%s%s%s              %s%s\n" , color_reset, space, border_side, color_card, color_reset, border_side);
    printf("%s%s%s%s %s    %d     %s %s%s\n", color_reset, space, border_side, color_card, arrow_left, m_card.power, arrow_right, color_reset, border_side);
    printf("%s   PROXIMA     " "%s%s              %s%s\n", color_reset, border_side, color_card, color_reset, border_side);
    printf("%s    CARTA      " "%s%s%s%s%s\n", color_reset, border_side, color_card, m_element, color_reset, border_side);
    printf("%s%s%s%s              %s%s\n"      , color_reset, space, border_side, color_card, color_reset, border_side);
    printf("%s%s%s%s      %s       %s%s\n"  , color_reset, space, border_side, color_card, arrow_bottom, color_reset, border_side);
    printf("%s%s\n", space, border_top_bottom);

    printf("\n");
}

