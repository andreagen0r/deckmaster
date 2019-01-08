#ifndef POINT_H
#define POINT_H

#include <stdio.h>

enum Direction {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT,
    NO_DIRECTION
};

typedef struct _point {
    int x;
    int y;
} Point;

// Imprime as coordenadas x e y
void point_print(const Point m_position);

#endif // POINT_H
