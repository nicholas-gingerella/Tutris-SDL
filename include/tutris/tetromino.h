#ifndef TETROMINO_H
#define TETROMINO_H

class Tetromino
{
public:
  Tetromino();
  ~Tetromino();
  void move();
  void rotate();
  void render();

private:
  int pos_x, pos_y;
  int shape[8] = {0};
};

#endif