#include "card.h"

Card card_new_random()
{
    Card card;

    card.status = NO_STATUS; // Defne status default
    card.power = (rand() % 5) + 1; // Poder entre 1-5
    card.element = rand() % 4 ; // Elemento entre 0-3 (0)SEM_ELEMENTO (1)FOGO, (2)GELO, (3)VENENO

    // Reset direção do ataque
    card.arrows[0] = false;
    card.arrows[1] = false;
    card.arrows[2] = false;
    card.arrows[3] = false;
    card.arrows[4] = false;

    short position_qty = rand() % 5; // Define quantas direções a carta terá entre 0-4

    // Atribui true para as direções aleatórias
    short contar = 0;
    while (contar != position_qty)
    {
        // Define a direção aleatória que a carta receberá uma seta de ataque
        int direction = rand() % 4;
        if (card.arrows[direction] == false)
        {
            card.arrows[direction] = true;
            contar++;
        }
    }

    return card;
}

Card card_new_default(void)
{
    Card card;

    card.power = 0;
    card.element = NO_ELEMENT;
    card.status = NO_STATUS;

    // Reset direção do ataque
    card.arrows[0] = false;
    card.arrows[1] = false;
    card.arrows[2] = false;
    card.arrows[3] = false;
    card.arrows[4] = false;

    return card;
}

Card card_fusion(const Card m_next_card, const Card m_old_card)
{
    Card new = m_next_card;
    new.power += m_old_card.power;

    return new;
}

char* card_get_status(const size_t m_status)
{
    char *out = NULL;

    switch (m_status) {
    case BURNED:
        out = "  INCENDIADA  ";
        break;
    case FROZEN:
        out = "  CONGELADA   ";
        break;
    case POISONED:
        out = "  ENVENENADA  ";
        break;
    default:
        out = "|             ";
        break;
    }

    return out;
}

char* card_get_element(const size_t m_element)
{
    char *out = NULL;
    switch (m_element) {
    case FIRE:
        out = "     FOGO     ";
        break;
    case ICE:
        out = "     GELO     ";
        break;
    case POISON:
        out = "    VENENO    ";
        break;
    default:
        out = "              ";
        break;
    }

    return out;
}

char* card_get_color_status(const size_t m_status)
{
    char *color = NULL;

    switch (m_status) {
    case BURNED:
        color = "\x1b[101m\x1b[30m";
        break;
    case FROZEN:
        color = "\x1b[106m\x1b[30m";
        break;
    case POISONED:
        color = "\x1b[102m\x1b[30m";
        break;
    default:
        color = "\x1b[0m";
        break;
    }

    return color;
}

char* card_get_color_element(const size_t m_element)
{
    char *color = NULL;

    switch (m_element) {
    case FIRE:
        color = "\x1b[41m\x1b[30m";
        break;
    case ICE:
        color = "\x1b[46m\x1b[30m";
        break;
    case POISON:
        color = "\x1b[42m\x1b[30m";
        break;
    default:
        color = "\x1b[100;2m\x1b[30m";
        break;
    }

    return color;
}

void card_log(const Card m_card)
{
    char *m_element = NULL;
    char *m_status = NULL;

    switch (m_card.element) {
    case FIRE:
        m_element = "..FOGO";
        break;
    case ICE:
        m_element = "..GELO";
        break;
    case POISON:
        m_element = "VENENO";
        break;
    default:
        m_element = "SEM ELEMENTO";
        break;
    }

    switch (m_card.status) {
    case BURNED:
        m_status = "INCENDIADO";
        break;
    case FROZEN:
        m_status = "CONGELADO";
        break;
    case POISONED:
        m_status = "ENVENENADO";
        break;
    default:
        m_status = "SEM STATUS";
        break;
    }

    printf("Poder:..... %d\n", m_card.power);
    printf("Elemento:.. %s\n", m_element);
    printf("Status:.... %s\n", m_status);
    printf("%s", m_card.arrows[TOP] ? "Seta:...... CIMA\n" : "");
    printf("%s", m_card.arrows[RIGHT] ? "Seta:...... DIREITA\n" : "");
    printf("%s", m_card.arrows[BOTTOM] ? "Seta:...... BAIXO\n" : "");
    printf("%s", m_card.arrows[LEFT] ? "Seta:...... ESQUERDA\n" : "");
    printf("%s", m_card.arrows[NO_DIRECTION] ? "Seta:...... SEM DIREÇÃO\n" : "");
    printf("\n");

}

