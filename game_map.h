#ifndef GAME_MAP_H_INCLUDED
#define GAME_MAP_H_INCLUDED

#include "utils.h"
#include "base_object.h"

class tileMap : public baseObject{
public:
       tileMap(){;}
       tileMap(SDL_Renderer* renderer, std::string fileName) : baseObject(renderer, fileName) {}
       ~tileMap(){;}
};

struct Map{
       int _current_x = 0, _current_y = 0;
       int tile[MAX_MAP_Y][MAX_MAP_X];
};

class gameMap{
public:
       gameMap(SDL_Renderer* renderer);
       ~gameMap();
       void dfs_generate_random_maze(int y, int x);
       bool connectGate(int y, int x);
       void loadTileMap();
       void draw(SDL_Renderer* renderer);
       Map getMap() const{return _game_map;}
       int getCurrentCameraX() const{return _game_map._current_x;}
       int getCurrentCameraY() const{return _game_map._current_y;}
       int getTileType(const int& x, const int &y) const {return _game_map.tile[y][x];}
       void setCurrentCamera(const int& x, const int& y);
       std::vector<SDL_Point> get_soil_pos() const{return _soil_pos;}
       SDL_Point getRandomSoilPos();
private:
       tileMap* allTileType[TOTAL_TILE_TYPE];
       std::vector<SDL_Point> _soil_pos; // where enemy born
       Map _game_map;
};

#endif // GAME_MAP_H_INCLUDED
