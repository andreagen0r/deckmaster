#include <stdlib.h>
#include <locale.h>

#include "game.h"

#ifdef __APPLE__
void limparTela(void)
{
    system("clear");
}
#elif _WIN32
void limparTela()
{
    system("cls");
}
#endif

int main() {

    setlocale(LC_ALL, "");

    game_init();

    return 0;
}
