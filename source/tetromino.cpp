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
            
        }
        

    }
}

std::vector<tutris::block> Tetromino::getPiece()
{
    return m_shape;
}

void Tetromino::setShape(tutris::tetromino_shape s)
{
    tutris::block p;
    p.block_type = tutris::grid_cell_type::curr_piece;

    tutris::block o;
    o.block_type = tutris::grid_cell_type::empty;
    switch (s)
    {
        case tutris::tetromino_shape::el:
        {
            tutris::block shape_el [tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,p,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_el[0], &shape_el[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::line:
        {
            tutris::block shape_line [tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,o,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_line[0], &shape_line[tutris::PIECE_DIMENSION]);
            break; 
        }
        case tutris::tetromino_shape::square:
        {
            tutris::block shape_square [tutris::PIECE_DIMENSION] = {
                o,o,o,o,
                o,p,p,o,
                o,p,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_square[0], &shape_square[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::tee:
        {
            tutris::block shape_tee [tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,o,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_tee[0], &shape_tee[tutris::PIECE_DIMENSION]);
            break;
        }
        case tutris::tetromino_shape::random:
        {
            break;
        }
        case tutris::tetromino_shape::zee:
        {
            tutris::block shape_zee [tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,p,o,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_zee[0], &shape_zee[tutris::PIECE_DIMENSION]);
            break;
        }
    }
}