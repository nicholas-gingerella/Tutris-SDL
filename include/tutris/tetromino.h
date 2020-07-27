#ifndef TETROMINO_H
#define TETROMINO_H
#include "tutris/tutris.h"
#include <SDL2/SDL.h>


class Tetromino
{
public:
  Tetromino();
  Tetromino(tutris::tetromino_shape shape);
  ~Tetromino();
  void move();
  void rotate();
  void render(SDL_Renderer *renderer);
  void printPiece();

private:
  int pos_x, pos_y;
  int shape[8] = {0};
  
};

#endif