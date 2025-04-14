#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "utils.h"
#include "base_object.h"
#include "button.h"
#include "text.h"

class Menu{
public:
       Menu(SDL_Renderer* renderer);
       ~Menu();
       gameStatus handleInputAction(const SDL_Event& event, gameStatus currentGameStatus, Cursor* mouse);
       void update(gameStatus currentGameStatus);
       void draw(SDL_Renderer* renderer, gameStatus currentGameStatus);
       void setAlphaMod(const int& a) {_alpha_mod = a;}
       void setFinalScore(SDL_Renderer* renderer, std::string text);
       void setHighestScore(SDL_Renderer* renderer, std::string text);
       Button* getRestart() const {return _restart_button;}
       Button* getResume() const {return _continue_button;}
       Button* getExit() const {return _exit_button;}
private:

       int _alpha_mod;
       baseObject* _background;
       Button* _restart_button;
       Button* _continue_button;
       Button* _exit_button;

       Text* _final_score;
       Text* _highest_score;
};

#endif // MENU_H_INCLUDED
