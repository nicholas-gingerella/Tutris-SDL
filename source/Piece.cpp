#include "tutris/Piece.h"
#include "tutris/tutris.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <SDL2/SDL.h>

Piece::Piece()
{}

Piece::Piece(ns_Tutris::tetromino_shape s){ setShape(s); }

Piece::~Piece(){}

std::vector<ns_Tutris::block> Piece::getPiece(){ return m_shape; }

void Piece::setShape(ns_Tutris::tetromino_shape s, ns_Tutris::piece_color color)
{
    ns_Tutris::block p;
    p.block_type = ns_Tutris::grid_cell_type::curr_piece;
    int color_r, color_g, color_b;
    int outline_color_r, outline_color_g, outline_color_b;
    switch (color)
    {
        case ns_Tutris::piece_color::piece_red:
        {
            color_r = ns_Tutris::COLOR_RED.r;
            color_g = ns_Tutris::COLOR_RED.g;
            color_b = ns_Tutris::COLOR_RED.b;
            outline_color_r = ns_Tutris::COLOR_DARKRED.r;
            outline_color_g = ns_Tutris::COLOR_DARKRED.g;
            outline_color_b = ns_Tutris::COLOR_DARKRED.b;
            break;
        }
        case ns_Tutris::piece_color::piece_blue:
        {
            color_r = ns_Tutris::COLOR_BLUE.r;
            color_g = ns_Tutris::COLOR_BLUE.g;
            color_b = ns_Tutris::COLOR_BLUE.b;
            outline_color_r = ns_Tutris::COLOR_DARKBLUE.r;
            outline_color_g = ns_Tutris::COLOR_DARKBLUE.g;
            outline_color_b = ns_Tutris::COLOR_DARKBLUE.b;
            break;
        }
        case ns_Tutris::piece_color::piece_green:
        {
            color_r = ns_Tutris::COLOR_GREEN.r;
            color_g = ns_Tutris::COLOR_GREEN.g;
            color_b = ns_Tutris::COLOR_GREEN.b;
            outline_color_r = ns_Tutris::COLOR_DARKGREEN.r;
            outline_color_g = ns_Tutris::COLOR_DARKGREEN.g;
            outline_color_b = ns_Tutris::COLOR_DARKGREEN.b;
            break;
        }
        case ns_Tutris::piece_color::piece_yellow:
        {
            color_r = ns_Tutris::COLOR_YELLOW.r;
            color_g = ns_Tutris::COLOR_YELLOW.g;
            color_b = ns_Tutris::COLOR_YELLOW.b;
            outline_color_r = ns_Tutris::COLOR_DARKYELLOW.r;
            outline_color_g = ns_Tutris::COLOR_DARKYELLOW.g;
            outline_color_b = ns_Tutris::COLOR_DARKYELLOW.b;
            break;
        }
        case ns_Tutris::piece_color::piece_random:
            // intentional fall-through
            // should never get this option
        default:
        {
            // Uh....we shouldn't get here...hopefully
            color_r = ns_Tutris::COLOR_WHITE.r;
            color_g = ns_Tutris::COLOR_WHITE.g;
            color_b = ns_Tutris::COLOR_WHITE.b;
            outline_color_r = ns_Tutris::COLOR_BLACK.r;
            outline_color_g = ns_Tutris::COLOR_BLACK.g;
            outline_color_b = ns_Tutris::COLOR_BLACK.b;
            break;
        }
    }

    // Set the color of the player block
    p.color = {color_r, color_g, color_b};
    p.outline_color = {outline_color_r, outline_color_g, outline_color_b};

    // o is just an empty block used for padding the piece shape
    ns_Tutris::block o;
    o.block_type = ns_Tutris::grid_cell_type::empty;

    switch (s)
    {
        case ns_Tutris::tetromino_shape::el:
        {
            ns_Tutris::block shape_el [ns_Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,p,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_el[0], &shape_el[ns_Tutris::PIECE_DIMENSION]);
            break;
        }
        case ns_Tutris::tetromino_shape::line:
        {
            ns_Tutris::block shape_line [ns_Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,o,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_line[0], &shape_line[ns_Tutris::PIECE_DIMENSION]);
            break; 
        }
        case ns_Tutris::tetromino_shape::square:
        {
            ns_Tutris::block shape_square [ns_Tutris::PIECE_DIMENSION] = {
                o,o,o,o,
                o,p,p,o,
                o,p,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_square[0], &shape_square[ns_Tutris::PIECE_DIMENSION]);
            break;
        }
        case ns_Tutris::tetromino_shape::tee:
        {
            ns_Tutris::block shape_tee [ns_Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,o,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_tee[0], &shape_tee[ns_Tutris::PIECE_DIMENSION]);
            break;
        }
        case ns_Tutris::tetromino_shape::random:
        {
            break;
        }
        case ns_Tutris::tetromino_shape::zee:
        {
            ns_Tutris::block shape_zee [ns_Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,p,o,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_zee[0], &shape_zee[ns_Tutris::PIECE_DIMENSION]);
            break;
        }
    }
}