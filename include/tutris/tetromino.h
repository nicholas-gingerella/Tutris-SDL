#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
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
  std::vector<int> getPiece();
  void setShape(tutris::tetromino_shape shape);


private:
  int pos_x, pos_y;
  std::vector<int> shape;
  
};

#endif