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
int delay = 120;

void kill();

void CreateGrid();
void IncreaseSnake(int &snake_size);
void RenderSnake(int &snake_size, int &dir);

void SpawnFruit();
void RenderFruit();

bool failstate(SDL_Rect *Player, int &snake_size);

void ai(int &dir, int snake_size);

int main(int argc, char* args[]){
  
  srand(time(NULL));

  //            INITIALIZATION
  SDL_InitSubSystem(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Snake Game and hopefully some AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //             THE MAIN CHUNK

  char ai_activate = 'p';

  std::cout << "Play or let computer? (insert c to allow COM Player)";
  std::cin >> ai_activate;

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
 
  SpawnFruit();      

  if (ai_activate == 'c') {
    delay = 60;
    dir = 1;
  }

  while (GameLoop == 1) {
    while (SDL_PollEvent(&event)) {
      
      switch (event.type) {
        case SDL_QUIT:
          GameLoop = 0;
          break;
      }

      if (ai_activate != 'c') {
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
    }

      // RENDER LOOP
    SDL_RenderClear(renderer);

    CreateGrid();
    if (ai_activate == 'c') {
      ai(dir, snake_size);
    }

    RenderFruit();
    RenderSnake(snake_size, dir);
    if (failstate(Player, snake_size)) {
      score = 0;
    }

    SDL_SetRenderDrawColor(renderer, 80, 140, 120, 255);
    SDL_RenderPresent(renderer);
    
    SDL_Delay(delay);

  }
  
  kill();
 
  return 0;
}

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

void SpawnFruit(){
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
        //kill();
        //delete [] Player;
        snake_size = 1;
        return true;
      }
    }
    if (Player[0].x == Fruit.x && Player[0].y == Fruit.y) {
      snake_size += 1;
      score ++;
      IncreaseSnake(snake_size);
      SpawnFruit();
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

// The AI stuff
void turn_left(int &dir) {
  if (dir == 1) {
    dir = 4;
  }
  else if (dir == 2) {
    dir = 3;
  }
  else if (dir == 4) {
    dir = 2;
  }
  else if (dir == 3) {
    dir = 1;
  }

}

void turn_right(int &dir) {
  if (dir == 1) {
    dir = 3;
  }
  else if (dir == 2) {
    dir = 4;
  }
  else if (dir == 4) {
    dir = 1;
  }
  else if (dir == 3) {
    dir = 2;
  }
}

void turn_forward(int &dir) {
  dir = dir;
}

// STATE MACHINE? 
enum {
  TRY_FORWARD,
  TRY_LEFT,
  TRY_RIGHT,
};

int state(int move, int &dir, int snake_size){
  int reward = 0;

  int try_x = (*(Player + 0)).x;
  int try_y = (*(Player + 0)).y;

  switch (dir) {
    case 1:
      switch(move) {
        case TRY_FORWARD:
          try_y += -GRID_HEIGHT;
          break;
        case TRY_LEFT:
          try_x += -GRID_WIDTH;
          break;
        case TRY_RIGHT:
          try_x += GRID_WIDTH;
          break;
      }
      break;

    case 2: 
      switch(move) {
        case TRY_FORWARD:
          try_y += GRID_HEIGHT;
          break;
        case TRY_LEFT:
          try_x += GRID_HEIGHT;
          break;
        case TRY_RIGHT:
          try_x += -GRID_WIDTH;
          break;
      } 
      break;

    case 3: 
      switch(move) {
        case TRY_FORWARD:
          try_x += GRID_WIDTH;
          break;
        case TRY_LEFT:
          try_y += -GRID_HEIGHT;
          break;
        case TRY_RIGHT:
          try_y += GRID_HEIGHT;
          break;
      }
      break;
 
    case 4:
      switch(move) {
        case TRY_FORWARD:
          try_x += -GRID_WIDTH;
          break;
        case TRY_LEFT:
          try_y += GRID_HEIGHT;
          break;
        case TRY_RIGHT:
          try_y -= GRID_HEIGHT;
          break;
      }
      break;
  }
   
  //if (try_x == Fruit.x && try_y == Fruit.y) {
    //reward += 100;
  //}

  int diff_x = abs((*(Player + 0)).x - Fruit.x);
  int diff_y = abs((*(Player + 0)).y - Fruit.y); 
  int try_diff_x = abs(try_x - Fruit.x);
  int try_diff_y = abs(try_y - Fruit.y);
  
  if (try_diff_x < diff_x) {
    reward += 5;
  }
  if (try_diff_y < diff_y) {
    reward += 5;
  }

  // DETECT TAIL
  for (int i=0; i<snake_size; i++) {
    if (try_x == Player[i].x && try_y == Player[i].y) {
      reward += -100;
      //std::cout << "tail detected!";
    }
  }

  return reward;
}

void ai(int &dir, int snake_size) {
  int try_f = state(TRY_FORWARD, dir, snake_size); 
  int try_l = state(TRY_LEFT, dir, snake_size);
  int try_r = state(TRY_RIGHT, dir, snake_size);

  if (try_f >= try_l && try_f >= try_r) {
    // MOVE FORWARD
    turn_forward(dir);
  }
  else if (try_l >= try_f && try_l >= try_r) {
    // TURN LEFT
    turn_left(dir);
  }
  else if (try_r >= try_f && try_r >= try_l) {
    // TURN RIGHT
    turn_right(dir);
  }
}
