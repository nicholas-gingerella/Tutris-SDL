#ifndef TUTRIS_H
#define TUTRIS_H

namespace tutris
{
    const int FIELD_WIDTH = 16;
    const int FIELD_HEIGHT = 32;
    const int BLOCK_SIZE_PIXEL = 15; // size of piece on screen (size x size pixels)
    const int PIECE_DIMENSION = 16; // number of indexes in piece array
    const int SCORE_INCREMENT_BASIC = FIELD_WIDTH * 10;
    const int SCORE_INCREMENT_COLLAPSE = FIELD_WIDTH * 100;

    enum tetromino_shape
    {
        tee = 0,
        line,
        square,
        el,
        zee,
        random
    };

    enum move_direction
    {
        left = 0,
        right,
        down,
    };

    enum grid_cell_type
    {
        empty = 0,
        piece,
        curr_piece,
        clearing,
        wall
    };

    enum Color
    {
        white = 0,
        red,
        blue,
        green,
        yellow
    };

    typedef struct block_color
    {
        unsigned int r;
        unsigned int g;
        unsigned int b;

        block_color():
        r(0),
        g(0),
        b(0)
        {}

        block_color(int r, int g, int b):
        r(r),
        g(g),
        b(b)
        {}

    } block_color;
    
    typedef struct block
    {
        unsigned int pos_x;
        unsigned int pos_y;
        block_color color;
        unsigned int block_type;
        bool isActive;

        block():
        pos_x(0),
        pos_y(0),
        block_type(0),
        isActive(false)
        {}

        block(block_color color, unsigned int block_type):
        color(color),
        block_type(block_type)
        {}
    } block;
}

#endif