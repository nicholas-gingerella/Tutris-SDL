#include "tutris/tetromino.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <SDL2/SDL.h>

Tetromino::Tetromino() :
pos_x(0),
pos_y(0)
{

}

Tetromino::Tetromino(tutris::tetromino_shape s)
{
    switch (s)
    {
        case tutris::tetromino_shape::el:
        {
            int shape_el [8] = {
                0,1,
                0,1,
                0,1,
                1,1
            };
            shape.clear();
            shape.insert(shape.end(), &shape_el[0], &shape_el[8]);
            break;
        }
        case tutris::tetromino_shape::line:
        {
            int shape_line [8] = {
                0,1,
                0,1,
                0,1,
                0,1
            };
            shape.clear();
            shape.insert(shape.end(), &shape_line[0], &shape_line[8]);            break; 
        }
        case tutris::tetromino_shape::square:
        {
            int shape_square [8] = {
                1,1,
                1,1,
                0,0,
                0,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_square[0], &shape_square[8]);            break;
        }
        case tutris::tetromino_shape::tee:
        {
            int shape_tee [8] = {
                0,1,
                1,1,
                0,1,
                0,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_tee[0], &shape_tee[8]);            break;
        }
        case tutris::tetromino_shape::random:
        {
            //int randShape[8] = tutris::getRandomShape()
            //std::copy(std::begin(randShape), std::end(randShape), std::begin(shape));
            break;
        }
    }
}

Tetromino::~Tetromino()
{

}

void Tetromino::render(SDL_Renderer *renderer)
{

}

void Tetromino::rotate()
{

}

void Tetromino::move()
{

}

void Tetromino::printPiece()
{
    for (int i = 0; i < 8; ++i)
    {
        if ( (i > 0) && (i % 2 == 0))
        {
            std::cout << std::endl;
        }
        std::cout << shape[i];

    }
}

std::vector<int> Tetromino::getPiece()
{
    return shape;
}