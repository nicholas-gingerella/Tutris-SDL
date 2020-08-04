#ifndef TUTRIS_H
#define TUTRIS_H

namespace tutris
{
    const int FIELD_WIDTH = 16;
    const int FIELD_HEIGHT = 32;
    const int BLOCK_SIZE_PIXEL = 10;

    enum tetromino_shape
    {
        tee = 0,
        line,
        square,
        el,
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
        wall
    };
}

#endif