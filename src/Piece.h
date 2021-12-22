#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
#include "tutris.h"
#include <SDL2/SDL.h>


class Piece
{
public:
  Piece();
  Piece(Tutris::PieceShape shape);
  ~Piece();
  std::vector<Tutris::Block> getPiece();
  void setShape(Tutris::PieceShape shape, Tutris::PieceColor = Tutris::PieceColor::piece_red);

private:
  std::vector<Tutris::Block> m_shape;
};

#endif