#include "utils.h"

void CommonFunction::logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void CommonFunction::initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
	int screenWidth, int screenHeight, const char* windowTitle)
{
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
              logSDLError(std::cout, "Init SDL_image", true);
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
              logSDLError(std::cout, "Init SDL_mixer", true);
    }

    if( TTF_Init() == -1 ){
              logSDLError(std::cout, "Init TTF", true);
    }
}


bool CommonFunction::checkCollision(const SDL_Rect& object1, const SDL_Rect& object2){

       if(checkPointInsideRect(object1.x, object1.y, object2)) return true;
       if(checkPointInsideRect(object1.x + object1.w - 1, object1.y, object2)) return true;
       if(checkPointInsideRect(object1.x, object1.y + object1.h - 1, object2)) return true;
       if(checkPointInsideRect(object1.x + object1.w - 1, object1.y + object1.h - 1, object2)) return true;

       if(checkPointInsideRect(object2.x, object2.y, object1)) return true;
       if(checkPointInsideRect(object2.x + object2.w - 1, object2.y, object1)) return true;
       if(checkPointInsideRect(object2.x, object2.y + object2.h - 1, object1)) return true;
       if(checkPointInsideRect(object2.x + object2.w - 1, object2.y + object2.h - 1, object1)) return true;

       return false;

}

bool CommonFunction::checkPointInsideRect(const int& x, const int& y, const SDL_Rect& rect){
       return x >= rect.x && y >= rect.y && x < rect.x + rect.w && y < rect.y + rect.h;
}

