#include "tutris/tetromino.h"
#include "tutris/tutris.h"
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
    setShape(s);
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

void Tetromino::setShape(tutris::tetromino_shape s)
{
    int p = tutris::grid_cell_type::curr_piece;
    switch (s)
    {
        case tutris::tetromino_shape::el:
        {
            int shape_el [tutris::PIECE_DIMENSION] = {
                0,0,p,0,
                0,0,p,0,
                0,0,p,0,
                0,p,p,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_el[0], &shape_el[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::line:
        {
            int shape_line [tutris::PIECE_DIMENSION] = {
                0,0,p,0,
                0,0,p,0,
                0,0,p,0,
                0,0,p,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_line[0], &shape_line[tutris::PIECE_DIMENSION]);
            break; 
        }
        case tutris::tetromino_shape::square:
        {
            int shape_square [tutris::PIECE_DIMENSION] = {
                0,p,p,0,
                0,p,p,0,
                0,0,0,0,
                0,0,0,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_square[0], &shape_square[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::tee:
        {
            int shape_tee [tutris::PIECE_DIMENSION] = {
                0,0,p,0,
                0,p,p,0,
                0,0,p,0,
                0,0,0,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_tee[0], &shape_tee[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::random:
        {
            //int randShape[8] = tutris::getRandomShape()
            //std::copy(std::begin(randShape), std::end(randShape), std::begin(shape));
            break;
        }
        case tutris::tetromino_shape::test:
        {
            int shape_test [tutris::PIECE_DIMENSION] = {
                0,0,p,0,
                0,p,p,0,
                0,p,0,0,
                0,0,0,0
            };
            shape.clear();
            shape.insert(shape.end(), &shape_test[0], &shape_test[tutris::PIECE_DIMENSION]);
            break;
        }
    }
}