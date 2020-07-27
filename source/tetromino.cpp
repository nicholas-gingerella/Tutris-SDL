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
            std::copy(std::begin(shape_el), std::end(shape_el), std::begin(shape));
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
            std::copy(std::begin(shape_line), std::end(shape_line), std::begin(shape));
            break; 
        }
        case tutris::tetromino_shape::square:
        {
            int shape_square [8] = {
                1,1,
                1,1,
                0,0,
                0,0
            };
            std::copy(std::begin(shape_square), std::end(shape_square), std::begin(shape));
            break;
        }
        case tutris::tetromino_shape::tee:
        {
            int shape_tee [8] = {
                0,1,
                1,1,
                0,1,
                0,0
            };
            std::copy(std::begin(shape_tee), std::end(shape_tee), std::begin(shape));
            break;
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