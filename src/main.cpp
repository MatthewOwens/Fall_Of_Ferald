#include "Game.h"
#ifdef __WIN32__
#include <direct.h>
#include <Windows.h>
#endif

int main()
{
    Game game;
    game.run();
    return 0;
}
