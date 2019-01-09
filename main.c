#include <stdlib.h>
#include <locale.h>

#include "game.h"

#ifdef _WIN32
void clear_screen(void)
{
    system("cls");
}
#else
void clear_screen()
{
    system("clear");
}
#endif

int main() {

    setlocale(LC_ALL, "");

    game_init();

    return 0;
}
