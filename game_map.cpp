#include "game_map.h"

gameMap::gameMap(SDL_Renderer* renderer){
       allTileType[ICE] = new tileMap(renderer, "ice.png");
       allTileType[SOIL] = new tileMap(renderer, "Soil.png");
       allTileType[STONE] = new tileMap(renderer, "stone.png");
}

gameMap::~gameMap(){
       for(int i = 0; i < TOTAL_TILE_TYPE; i ++){
              if(allTileType[i] != NULL){
                     delete allTileType[i];
                     allTileType[i] = NULL;
              }
       }
       _soil_pos.clear();
}

void gameMap::dfs_generate_random_maze(int y, int x){

       _game_map.tile[y][x] = SOIL;
       static int directionX[] = {1, -1, 0, 0};
       static int directionY[] = {0, 0, 1, -1};

       static int direct_order[] = {0, 1, 2, 3};
       std::random_shuffle(direct_order, direct_order + 4);

       for(int i : direct_order){

              int next_y = y + directionY[i], next_x = x + directionX[i];

              if(next_x < 0 || next_x > MAX_MAP_X) continue;
              if(next_y < 0 || next_y > MAX_MAP_Y) continue;
              if(_game_map.tile[next_y][next_x] == SOIL) continue;

              if(_game_map.tile[next_y][next_x] == STONE){
                     bool check_valid_cell = true;
                     for(int j : direct_order){
                            int beside_next_y = next_y + directionY[j],
                                beside_next_x = next_x + directionX[j];
                            if(beside_next_y == y && beside_next_x == x) continue;
                            if(_game_map.tile[beside_next_y][beside_next_x] == SOIL){
                                   check_valid_cell = false;
                                   break;
                            }
                     }
                     if(check_valid_cell){
                            dfs_generate_random_maze(next_y, next_x);
                     }
              }

       }

}

bool gameMap::connectGate(int y, int x){
       if(_game_map.tile[y][x] == SOIL) return true;

       _game_map.tile[y][x] = SOIL;

       if(rand() % 2) return connectGate(y - 1, x);
       return connectGate(y, x - 1);
}

void gameMap::loadTileMap(){

       for(int i = 0; i < MAX_MAP_Y; i ++){
              for(int j = 0; j < MAX_MAP_X; j ++){
                   _game_map.tile[i][j] = STONE;
              }
       }

       for(int j = 0; j < MAX_MAP_X; j ++) _game_map.tile[0][j] = _game_map.tile[MAX_MAP_Y - 1][j] = ICE;
       for(int i = 0; i < MAX_MAP_Y; i ++) _game_map.tile[i][0] = _game_map.tile[i][MAX_MAP_X - 1] = ICE;

       dfs_generate_random_maze(1, 1);

       connectGate(MAX_MAP_Y - 2, MAX_MAP_X - 2);

       for(int i = 0; i < MAX_MAP_Y; i ++){

              for(int j = 0; j < MAX_MAP_X; j ++){

                     if(_game_map.tile[i][j] == SOIL) _soil_pos.push_back({j * TILE_SIZE, i * TILE_SIZE});
              }
       }

}

SDL_Point gameMap::getRandomSoilPos(){
       int random_index = rand() % (int) _soil_pos.size();
       return _soil_pos[random_index];
}

void gameMap::draw(SDL_Renderer* renderer){
       int leftBound = _game_map._current_x  / TILE_SIZE,
           rightBound = (_game_map._current_x + SCREEN_WIDTH - 1) / TILE_SIZE,
           upBound = _game_map._current_y / TILE_SIZE,
           lowBound = (_game_map._current_y + SCREEN_HEIGHT - 1) / TILE_SIZE;

       for(int i = upBound; i <= lowBound; i ++){
              for(int j = leftBound; j <= rightBound; j ++){
                     int val = getTileType(j, i);

                     if(val == STONE){
                            allTileType[SOIL]->setRect(j * TILE_SIZE - _game_map._current_x, i * TILE_SIZE - _game_map._current_y);
                            allTileType[SOIL]->render(renderer);

                     }
                     allTileType[val]->setRect(j * TILE_SIZE - _game_map._current_x, i * TILE_SIZE - _game_map._current_y);
                     allTileType[val]->render(renderer);
              }
        }
}

void gameMap::setCurrentCamera(const int& x, const int& y){
       _game_map._current_x = x;
       _game_map._current_y = y;
}
