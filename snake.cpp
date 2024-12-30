#include <iostream>

// this would not at all be how it is done

using namespace std;

#define COLUMNS 5
#define ROWS 5

int grid[COLUMNS][ROWS] = {{0},{0}};

void displayGrid(int *snake) {
  
  for (int i=0; i<ROWS; i++) {
    for (int j=0; j<COLUMNS; j++) {
      if (grid[i][j] == snake[i]) {
        grid[i][j] = 1;
      }
      cout << grid[ROWS][COLUMNS];
    }
    cout << endl;
  }
}

void setSnake(int *&snake, int snake_size){
  snake = new int[snake_size];

  for (int i=0; i<snake_size; i++) {
    *(snake + i) = 1;
  }
}

int main(){

  int snake_size = 3;
  int *snake = new int[snake_size];

  setSnake(snake, snake_size);

  for (int i=0; i<snake_size+1; i++) {
    cout << *(snake + i);
  }
  cout << endl;

  displayGrid(snake); 
}
