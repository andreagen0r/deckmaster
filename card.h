#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "point.h"

#define CARD_WIDTH 15
#define CARD_HEIGHT 9

enum Status {
    NO_STATUS,
    BURNED,
    FROZEN,
    POISONED
};

enum Element {
    NO_ELEMENT,
    FIRE,
    ICE,
    POISON,
};

enum Render_Mode {
    TEXT_MODE,
    COLOR_MODE
};

typedef struct _card {
    int power;
    bool arrows[5];
    enum Element element;
    enum Status status;
} Card;

// Retorna uma nova carta com dados aleatórios
Card card_new_random(void);

// Retorna uma carta padrão. (SEM STATUS, SEM ELEMENTO, SEM SETAS e PODER 0)
Card card_new_default(void);

// Faz a fusão entre as cartas copiando a carta antiga para a nova e somando os poderes
Card card_fusion(const Card m_next_card, const Card m_old_card);

// Retorna o elemento formatado para imprimir na carta
char* card_get_element(const size_t m_element);

// Retorna o status formatado para imprimir na carta
char* card_get_status(const size_t m_status);

// Retorna uma cor de acordo com o elemento passado
char* card_get_color_element(const size_t m_element);

// Retorna uma cor de acordo com o status passado
char* card_get_color_status(const size_t m_status);

// Imprime dados da carta
void card_log(const Card m_card);

#endif // CARD_H
