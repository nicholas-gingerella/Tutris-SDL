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
  std::vector<tutris::block> getPiece();
  void setShape(tutris::tetromino_shape shape, tutris::block_color = tutris::COLOR_RED);


private:
  std::vector<tutris::block> m_shape;
  // tutris::block_color getColorStruct(tutris::Color);
};

#endif