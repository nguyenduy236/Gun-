#include "game.h"
#include "utils.h"

game *Game = nullptr;

int main(int argc, char* argv[]){
    Game = new game();
    Game->init(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SDL_Event e;
    while(Game->isRunning()){
              Game->handleUserInput(e);
              Game->update();
              Game->renderGamePlay();
    }

    delete Game;
    Game = NULL;
    return 0;
}
