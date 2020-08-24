#include "tutris/tetromino.h"
#include "tutris/tutris.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <SDL2/SDL.h>

Tetromino::Tetromino()
{}

Tetromino::Tetromino(tutris::tetromino_shape s)
{
    setShape(s);
}

Tetromino::~Tetromino()
{}

std::vector<tutris::block> Tetromino::getPiece()
{
    return m_shape;
}

void Tetromino::setShape(tutris::tetromino_shape s, tutris::Color color)
{
    tutris::block p;
    p.block_type = tutris::grid_cell_type::curr_piece;
    p.color = getColorStruct(color);


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

tutris::block_color Tetromino::getColorStruct(tutris::Color color)
{
    tutris::block_color c;

    unsigned int r, g, b;
    switch (color)
    {
        case tutris::Color::red:
        {
            r = 0xFF;
            g = 0x00;
            b = 0x00;
            break;
        }
        case tutris::Color::green:
        {
            r = 0x00;
            g = 0xFF;
            b = 0x00;
            break;
        }
        case tutris::Color::blue:
        {
            r = 0x00;
            g = 0x00;
            b = 0xFF;
            break;
        }
        case tutris::Color::yellow:
        {
            r = 0xE4;
            g = 0xE8;
            b = 0x00;
            break;
        }
        case tutris::Color::white:
        {
            r = 0x00;
            g = 0x00;
            b = 0x00;
            break;
        }
        default:
        {
            break;
        }
    }

    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}
