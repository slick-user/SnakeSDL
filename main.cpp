#include <iostream>
#include <time.h>

#include "Include/SDL2/SDL.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define ROWS 8
#define COLUMNS 8
#define GRID_WIDTH 50
#define GRID_HEIGHT 50

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect *Player;
SDL_Rect Fruit;

int score;

void kill();

void CreateGrid() {

  for (int i=0; i < ROWS; i++) {
    for (int j=0; j < COLUMNS; j++) {
      SDL_Rect Grid = (SDL_Rect) {j*GRID_WIDTH, i*GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawRect(renderer, &Grid);
    }
  }
}

void IncreaseSnake(int &snake_size) {
 
  SDL_Rect *newPlayer = new SDL_Rect[snake_size];                       //(SDL_Rect) {0, 0, 50, 50};
 
  for (int i=0; i<snake_size; i++) {
    if (i < snake_size-1) {
      newPlayer[i] = Player[i];
    }
    else {
      newPlayer[i] = Player[i-1];
      newPlayer[i].x = Player[i-1].x + -50;
      newPlayer[i].y = Player[i-1].y + 50;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &newPlayer[i]);
    //SDL_Delay(1000);
  }

  delete[] Player;

  Player = newPlayer; 
}

void RenderSnake(int &snake_size, int &dir) {

  int prev_posx = (*(Player + 0)).x;
  int prev_posy = (*(Player + 0)).y; 

  if (dir == 1) {
    (*(Player + 0)).y -= 50;
  }
  if (dir == 2) {
    (*(Player + 0)).y += 50;
  }
  if (dir == 3) {
    (*(Player + 0)).x += 50;
  }
  if (dir == 4) {
    (*(Player + 0)).x -= 50;
  }

  // SCREEN WRAPPING
  if ((*(Player + 0)).x > COLUMNS * GRID_WIDTH - GRID_WIDTH) {
    (*(Player + 0)).x = 0;
  } 
  else if ((*(Player + 0)).x < 0) {
    (*(Player + 0)).x = COLUMNS * GRID_WIDTH - GRID_WIDTH;
  }

  if ((*(Player + 0)).y > ROWS * GRID_HEIGHT - GRID_HEIGHT) {
    (*(Player + 0)).y = 0;
  }
  else if ((*(Player + 0)).y < 0) {
    (*(Player + 0)).y = ROWS * GRID_HEIGHT - GRID_HEIGHT;
  } 

  for (int i=0; i<snake_size; i++) {
    if (i != 0) {
      int tempx = Player[i].x;
      Player[i].x = prev_posx;
      prev_posx = tempx;
      int tempy = Player[i].y;
      Player[i].y = prev_posy;
      prev_posy = tempy;
    } 
    SDL_RenderFillRect(renderer, &Player[i]);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }

}

void spawnFruit(){
  int posx = (rand() % ROWS) * 50;
  int posy = (rand() % COLUMNS) * 50;
  
  while (posx == Player[0].x && posy == Player[0].y) {
    posx = (rand() % ROWS) * 50;
    posy = (rand() % COLUMNS) * 50;
  }
  
  Fruit.x = posx;
  Fruit.y = posy;
  Fruit.w = 50;
  Fruit.h = 50;
  SDL_RenderFillRect(renderer, &Fruit);
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
}

void RenderFruit() {
  SDL_RenderFillRect(renderer, &Fruit);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
}

bool failstate(SDL_Rect *Player, int &snake_size) {
  for (int i=0; i<snake_size; i++) {
    for (int j=i+1; j<snake_size - 1; j++) {      
      if (Player[j].x == Player[i].x && Player[j].y == Player[i].y) {
        std::cout << "Game Over: ";
        std::cout << score;
        SDL_Delay(1000);
        kill();
        return true;
      }
    }
    if (Player[0].x == Fruit.x && Player[0].y == Fruit.y) {
      snake_size += 1;
      score ++;
      IncreaseSnake(snake_size);
      spawnFruit();
    }
  }
  return false;
}

void kill() {
 
  //            DEALLOCATION
  delete[] Player;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

}

int main(int argc, char* args[]){
  
  srand(time(NULL));
  
  //            INITIALIZATION
  SDL_InitSubSystem(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Snake Game and hopefully some AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //             THE MAIN CHUNK

  int GameLoop = 1;
  SDL_Event event;

  
  int snake_size = 1;
  Player = new SDL_Rect[snake_size];                       //(SDL_Rect) {0, 0, 50, 50};
  int dir;
  //CreateSnake(snake_size); 
  for (int i=0; i<snake_size; i++) {
    (*(Player + i)).x = i*50;
    (*(Player + i)).y = i*50;
    (*(Player + i)).w = 50;
    (*(Player + i)).h = 50;

    SDL_RenderFillRect(renderer, &Player[i]);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  }
 
  spawnFruit();      

  while (GameLoop == 1) {
    while (SDL_PollEvent(&event)) {
      
      switch (event.type) {
        case SDL_QUIT:
          GameLoop = 0;
          break;
      }

      switch(event.key.keysym.sym) {
        case SDLK_UP:
          dir = 1;
          break;

        case SDLK_DOWN:
          dir = 2;
          break;

        case SDLK_RIGHT:
          dir = 3;
          break;

        case SDLK_LEFT:
          dir = 4;
          break;
      }
    } 

      // RENDER LOOP
    SDL_RenderClear(renderer);

    CreateGrid();
    RenderFruit();
    RenderSnake(snake_size, dir);
    if (failstate(Player, snake_size)) {
      return 0;
    }

    SDL_SetRenderDrawColor(renderer, 80, 140, 120, 255);
    SDL_RenderPresent(renderer);
    
    SDL_Delay(120);

  }
  
  kill();
 
  return 0;
}
