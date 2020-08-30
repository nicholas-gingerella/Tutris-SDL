#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
#include "tutris/tutris.h"
#include <SDL2/SDL.h>


class Piece
{
public:
  Piece();
  Piece(ns_Tutris::tetromino_shape shape);
  ~Piece();
  std::vector<ns_Tutris::block> getPiece();
  void setShape(ns_Tutris::tetromino_shape shape, ns_Tutris::piece_color = ns_Tutris::piece_color::piece_red);

private:
  std::vector<ns_Tutris::block> m_shape;
};

#endif