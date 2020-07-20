#ifndef TETROMINO_H
#define TETROMINO_H

class Tetromino
{
public:
  void move();
  void rotate();
  void render();

private:
  int x, y;
  // shape
  // color

};

#endif