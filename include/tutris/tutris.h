#ifndef TUTRIS_H
#define TUTRIS_H

namespace ns_Tutris
{
    const int FIELD_WIDTH = 16;
    const int FIELD_HEIGHT = 32;
    const int BLOCK_SIZE_PIXEL = 15; // size of piece on screen (size x size pixels)
    const int PIECE_DIMENSION = 16; // number of indexes in piece array
    const int SCORE_INCREMENT_BASIC = FIELD_WIDTH * 10;
    const int SCORE_INCREMENT_COLLAPSE = FIELD_WIDTH * 100;
    const unsigned int MAX_TIME_MS = (10 * 60 * 1000) + 1000; // 10 minutes (+1 sec so timer starts display on 10:00)

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

    enum piece_color
    {
        piece_red = 0,
        piece_blue,
        piece_green,
        piece_yellow,
        piece_random
    };

    typedef struct tutris_color
    {
        unsigned int r;
        unsigned int g;
        unsigned int b;

        tutris_color():
        r(0),
        g(0),
        b(0)
        {}

        tutris_color(int r, int g, int b):
        r(r),
        g(g),
        b(b)
        {}

    } tutris_color;
    
    typedef struct block
    {
        unsigned int pos_x;
        unsigned int pos_y;
        tutris_color color;
        tutris_color outline_color;
        unsigned int block_type;
        bool isActive;

        block():
        pos_x(0),
        pos_y(0),
        block_type(0),
        isActive(false)
        {}

        block(tutris_color color, unsigned int block_type):
        color(color),
        block_type(block_type)
        {}
    } block;

    const tutris_color TUTRIS_COLOR_RED (0xFF, 0x31, 0x31);
    const tutris_color TUTRIS_COLOR_DARKRED (0x71, 0x15, 0x15);
    const tutris_color TUTRIS_COLOR_GREEN(0x1C, 0xE0, 0x33);
    const tutris_color TUTRIS_COLOR_DARKGREEN(0x12, 0x61, 0x13);
    const tutris_color TUTRIS_COLOR_BLUE(0x3F, 0x39, 0xFF);
    const tutris_color TUTRIS_COLOR_DARKBLUE(0x15, 0x19, 0x71);
    const tutris_color TUTRIS_COLOR_BLUEGREY(0xE3, 0xE6, 0xFF);
    const tutris_color TUTRIS_COLOR_YELLOW(0xFB, 0xFF, 0x37);
    const tutris_color TUTRIS_COLOR_DARKYELLOW(0x5D, 0x60, 0x0E);
    const tutris_color TUTRIS_COLOR_WHITE(0xFF, 0xFF, 0xFF);
    const tutris_color TUTRIS_COLOR_BLACK(0x00, 0x00, 0x00);
    const tutris_color TUTRIS_COLOR_DARKGREY(0x5E, 0x5E, 0x5E);
    const tutris_color TUTRIS_COLOR_LIGHTGREY(0x7D, 0x7D, 0x7D);
    const tutris_color COLOR_BACKGROUND(0x21, 0x21, 0x21);

    const block BLOCK_WALL(TUTRIS_COLOR_DARKGREY, grid_cell_type::wall);
    const block BLOCK_EMPTY(TUTRIS_COLOR_LIGHTGREY, grid_cell_type::empty);
    const block BLOCK_CLEARING(TUTRIS_COLOR_BLUEGREY, grid_cell_type::clearing);
}

#endif