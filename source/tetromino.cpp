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

void Tetromino::setShape(tutris::tetromino_shape s, tutris::piece_color color)
{
    tutris::block p;
    p.block_type = tutris::grid_cell_type::curr_piece;
    int color_r, color_g, color_b;
    switch (color)
    {
        case tutris::piece_color::piece_red:
        {
            color_r = tutris::COLOR_RED.r;
            color_g = tutris::COLOR_RED.g;
            color_b = tutris::COLOR_RED.b;
            break;
        }
        case tutris::piece_color::piece_blue:
        {
            color_r = tutris::COLOR_LIGHTBLUE.r;
            color_g = tutris::COLOR_LIGHTBLUE.g;
            color_b = tutris::COLOR_LIGHTBLUE.b;
            break;
        }
        case tutris::piece_color::piece_green:
        {
            color_r = tutris::COLOR_GREEN.r;
            color_g = tutris::COLOR_GREEN.g;
            color_b = tutris::COLOR_GREEN.b;
            break;
        }
        case tutris::piece_color::piece_yellow:
        {
            color_r = tutris::COLOR_YELLOW.r;
            color_g = tutris::COLOR_YELLOW.g;
            color_b = tutris::COLOR_YELLOW.b;
            break;
        }
        case tutris::piece_color::piece_random:
            // intentional fall-through
        default:
        {
            // Uh....we shouldn't get here...hopefully
            color_r = tutris::COLOR_WHITE.r;
            color_g = tutris::COLOR_WHITE.g;
            color_b = tutris::COLOR_WHITE.b;
            break;
        }
    }

    p.color = {color_r, color_g, color_b};

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