#include "menu.h"

Menu::Menu(SDL_Renderer* renderer){
       _background = new baseObject(renderer, "menu_background.png");

       _restart_button = new Button ();
       _restart_button->loadFromFile(renderer, "NewGame.png");

       _continue_button = new Button ();
       _continue_button->loadFromFile(renderer, "Continue.png");

       _exit_button = new Button ();
       _exit_button->loadFromFile(renderer, "Exit.png");

       _final_score = new Text ("MouldyCheeseRegular-WyMWG.ttf", 30);
       _highest_score = new Text ("MouldyCheeseRegular-WyMWG.ttf", 30);
}

Menu::~Menu(){

       if(_background != NULL){
              delete _background;
              _restart_button = NULL;
       }

       if(_restart_button != NULL){
              delete _restart_button;
              _restart_button = NULL;
       }

       if(_continue_button != NULL){
              delete _continue_button;
              _continue_button = NULL;
       }

       if(_exit_button != NULL){
              delete _exit_button;
              _exit_button = NULL;
       }

       if(_final_score != NULL){
              delete _final_score;
              _final_score = NULL;
       }

       if(_highest_score != NULL){
              delete _highest_score;
              _highest_score = NULL;
       }

}

gameStatus Menu::handleInputAction(const SDL_Event& event, gameStatus currentStatus, Cursor* mouse){

       _restart_button->handleInputAction(event, mouse);
       if(currentStatus != GAME_START && currentStatus != GAME_OVER) _continue_button->handleInputAction(event, mouse);
       _exit_button->handleInputAction(event, mouse);

       if(_restart_button->isClicked()){
              _restart_button->setClicked(false);
              return GAME_RESTART;
       }

       else if(_continue_button->isClicked()){
              _continue_button->setClicked(false);
              return GAME_PLAYING;
       }

       else if(_exit_button->isClicked()){
              _exit_button->setClicked(false);
              return GAME_QUIT;
       }
       return currentStatus;
}

void Menu::setFinalScore(SDL_Renderer* renderer, std::string text){
       _final_score->loadText(renderer, text);
}

void Menu::setHighestScore(SDL_Renderer* renderer, std::string text){
       _highest_score->loadText(renderer, text);
}

void Menu::update(gameStatus currentGameStatus){
       static const int MAX_ALPHA = 255;
       if(currentGameStatus != GAME_OVER) _alpha_mod = MAX_ALPHA;
       else{

              _alpha_mod ++;
              if(_alpha_mod > MAX_ALPHA){
                     _alpha_mod = MAX_ALPHA;
              }

       }

       if(_alpha_mod < MAX_ALPHA){

              _restart_button->activate(false);
              _continue_button->activate(false);
              _exit_button->activate(false);


       }
       else{

              _restart_button->activate(true);
              _continue_button->activate(true);
              _exit_button->activate(true);

       }

       _restart_button->setAlpha(_alpha_mod );
       _continue_button->setAlpha(_alpha_mod );
       _exit_button->setAlpha(_alpha_mod );
       _background->setAlpha(_alpha_mod );
       _final_score->setAlpha(_alpha_mod);
       _highest_score->setAlpha(_alpha_mod);

}

void Menu::draw(SDL_Renderer* renderer, gameStatus currentGameStatus){
       _background->setRect(0, 0);
       _background->render(renderer);

       if(currentGameStatus == GAME_START || currentGameStatus == GAME_OVER){

              _restart_button->setRect( ( SCREEN_WIDTH - _restart_button->getRect().w) / 2, 250 );
              _restart_button->render(renderer);

              _exit_button->setRect( ( SCREEN_WIDTH - _exit_button->getRect().w) / 2, 350 );
              _exit_button->render(renderer);

       }

       else if(currentGameStatus == GAME_PAUSING){

              _restart_button->setRect( ( SCREEN_WIDTH - _restart_button->getRect().w) / 2, 200 );
              _restart_button->render(renderer);

              _continue_button->setRect( ( SCREEN_WIDTH - _continue_button->getRect().w) / 2, 300 );
              _continue_button->render(renderer);

              _exit_button->setRect( ( SCREEN_WIDTH - _exit_button->getRect().w) / 2, 400 );
              _exit_button->render(renderer);

       }


       if(currentGameStatus == GAME_OVER){

              _final_score->draw(renderer, ( SCREEN_WIDTH - _final_score->getRect().w ) / 2, 180);
              _highest_score->draw(renderer, ( SCREEN_WIDTH - _highest_score->getRect().w ) / 2, 215);

       }
}
