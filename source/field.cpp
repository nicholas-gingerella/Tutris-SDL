#include <iostream>
#include <SDL2/SDL.h>
#include "tutris/field.h"
#include "tutris/tetromino.h"
#include "tutris/tutris.h"

Field::Field() :
pos_x (0),
pos_y (0),
num_rows(0),
num_cols(0),
grid (nullptr)
{

}

Field::Field(int x, int y, int cols, int rows) :
pos_x(x),
pos_y(y),
num_rows(rows),
num_cols(cols)
{
    int num_grid_elems = num_rows * num_cols;
    grid = new int [num_grid_elems];

    for (int i = 0; i < num_grid_elems; ++i)
    {
        grid[i] = 0;
    }

}

Field::~Field()
{
    if (grid != nullptr)
    {
        delete[] grid;
    }
}

void Field::render(SDL_Renderer *renderer)
{
    int draw_pos_x = pos_x;
    int draw_pos_y = pos_y;
    int counter = 1;
    int num_grid_elems = num_rows * num_cols;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        SDL_Rect field_square = {
            draw_pos_x,
            draw_pos_y,
            tutris::BLOCK_SIZE_PIXEL,
            tutris::BLOCK_SIZE_PIXEL
        };

        if (grid[i] != 0)
        {
            //draw square
            SDL_SetRenderDrawColor( renderer, 0xFF,0x00,0x00,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, 0x00,0x00,0x00,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);

        }
        else
        {
            SDL_SetRenderDrawColor( renderer, 0xCC,0xCC,0xCC,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        
        if ( counter % num_cols == 0 )
        {
            // move to next row
            draw_pos_y += tutris::BLOCK_SIZE_PIXEL;
            draw_pos_x = pos_x;
        }
        else 
        {
            draw_pos_x += tutris::BLOCK_SIZE_PIXEL;
        }

        counter++;
    }
}

bool Field::isFilled()
{
    return false;
}

void Field::printField()
{
    int num_grid_elems = num_rows * num_cols;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        if ( (i > 0) && (i%num_cols == 0) )
        {
            std::cout << std::endl;
        }
        std::cout << grid[i] << " ";
    }

    std::cout << std::endl;
}

void Field::addPiece(tutris::tetromino_shape shape)
{
    // choose a random piece
    Tetromino piece(shape);

    // place piece at top of grid
    std::vector<int> newPiece = piece.getPiece();

    // decide grid position (index) to start drawing piece
    // from top left to bottom right.
    int x_pos = 3; // top of screen just off the corner
    int y_pos = 0;
    grid[x_pos + (y_pos * num_cols)] = newPiece[0];
    grid[(x_pos+1) + (y_pos * num_cols)] = newPiece[1];
    grid[(x_pos) + ((y_pos+1) * num_cols)] = newPiece[2];
    grid[(x_pos+1) + ((y_pos+1) * num_cols)] = newPiece[3];
    grid[(x_pos) + ((y_pos+2) * num_cols)] = newPiece[4];
    grid[(x_pos+1) + ((y_pos+2) * num_cols)] = newPiece[5];
    grid[(x_pos) + ((y_pos+3) * num_cols)] = newPiece[6];
    grid[(x_pos+1) + ((y_pos+3) * num_cols)] = newPiece[7];

    // change grid entry to '1' for solid blocks

}

void Field::movePiece()
{

}
