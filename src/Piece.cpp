#include "Piece.h"
#include "tutris.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <SDL2/SDL.h>

Piece::Piece()
{}

Piece::Piece(Tutris::PieceShape s){ setShape(s); }

Piece::~Piece(){}

std::vector<Tutris::Block> Piece::getPiece(){ return m_shape; }

void Piece::setShape(Tutris::PieceShape s, Tutris::PieceColor color)
{
    Tutris::Block p;
    p.block_type = Tutris::GridCellType::curr_piece;
    int color_r, color_g, color_b;
    int outline_color_r, outline_color_g, outline_color_b;
    switch (color)
    {
        case Tutris::PieceColor::piece_red:
        {
            color_r = Tutris::TUTRIS_COLOR_RED.r;
            color_g = Tutris::TUTRIS_COLOR_RED.g;
            color_b = Tutris::TUTRIS_COLOR_RED.b;
            outline_color_r = Tutris::TUTRIS_COLOR_DARKRED.r;
            outline_color_g = Tutris::TUTRIS_COLOR_DARKRED.g;
            outline_color_b = Tutris::TUTRIS_COLOR_DARKRED.b;
            break;
        }
        case Tutris::PieceColor::piece_blue:
        {
            color_r = Tutris::TUTRIS_COLOR_BLUE.r;
            color_g = Tutris::TUTRIS_COLOR_BLUE.g;
            color_b = Tutris::TUTRIS_COLOR_BLUE.b;
            outline_color_r = Tutris::TUTRIS_COLOR_DARKBLUE.r;
            outline_color_g = Tutris::TUTRIS_COLOR_DARKBLUE.g;
            outline_color_b = Tutris::TUTRIS_COLOR_DARKBLUE.b;
            break;
        }
        case Tutris::PieceColor::piece_green:
        {
            color_r = Tutris::TUTRIS_COLOR_GREEN.r;
            color_g = Tutris::TUTRIS_COLOR_GREEN.g;
            color_b = Tutris::TUTRIS_COLOR_GREEN.b;
            outline_color_r = Tutris::TUTRIS_COLOR_DARKGREEN.r;
            outline_color_g = Tutris::TUTRIS_COLOR_DARKGREEN.g;
            outline_color_b = Tutris::TUTRIS_COLOR_DARKGREEN.b;
            break;
        }
        case Tutris::PieceColor::piece_yellow:
        {
            color_r = Tutris::TUTRIS_COLOR_YELLOW.r;
            color_g = Tutris::TUTRIS_COLOR_YELLOW.g;
            color_b = Tutris::TUTRIS_COLOR_YELLOW.b;
            outline_color_r = Tutris::TUTRIS_COLOR_DARKYELLOW.r;
            outline_color_g = Tutris::TUTRIS_COLOR_DARKYELLOW.g;
            outline_color_b = Tutris::TUTRIS_COLOR_DARKYELLOW.b;
            break;
        }
        case Tutris::PieceColor::piece_random:
            // intentional fall-through
            // should never get this option
        default:
        {
            // Uh....we shouldn't get here...hopefully
            color_r = Tutris::TUTRIS_COLOR_WHITE.r;
            color_g = Tutris::TUTRIS_COLOR_WHITE.g;
            color_b = Tutris::TUTRIS_COLOR_WHITE.b;
            outline_color_r = Tutris::TUTRIS_COLOR_BLACK.r;
            outline_color_g = Tutris::TUTRIS_COLOR_BLACK.g;
            outline_color_b = Tutris::TUTRIS_COLOR_BLACK.b;
            break;
        }
    }

    // Set the color of the player block
    p.color = {color_r, color_g, color_b};
    p.outline_color = {outline_color_r, outline_color_g, outline_color_b};

    // o is just an empty block used for padding the piece shape
    Tutris::Block o;
    o.block_type = Tutris::GridCellType::empty;

    switch (s)
    {
        case Tutris::PieceShape::el:
        {
            Tutris::Block shape_el [Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,p,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_el[0], &shape_el[Tutris::PIECE_DIMENSION]);
            break;
        }
        case Tutris::PieceShape::line:
        {
            Tutris::Block shape_line [Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,o,p,o,
                o,o,p,o,
                o,o,p,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_line[0], &shape_line[Tutris::PIECE_DIMENSION]);
            break; 
        }
        case Tutris::PieceShape::square:
        {
            Tutris::Block shape_square [Tutris::PIECE_DIMENSION] = {
                o,o,o,o,
                o,p,p,o,
                o,p,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_square[0], &shape_square[Tutris::PIECE_DIMENSION]);
            break;
        }
        case Tutris::PieceShape::tee:
        {
            Tutris::Block shape_tee [Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,o,p,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_tee[0], &shape_tee[Tutris::PIECE_DIMENSION]);
            break;
        }
        case Tutris::PieceShape::random:
        {
            break;
        }
        case Tutris::PieceShape::zee:
        {
            Tutris::Block shape_zee [Tutris::PIECE_DIMENSION] = {
                o,o,p,o,
                o,p,p,o,
                o,p,o,o,
                o,o,o,o
            };
            m_shape.clear();
            m_shape.insert(m_shape.end(), &shape_zee[0], &shape_zee[Tutris::PIECE_DIMENSION]);
            break;
        }
    }
}