#include <iostream>
#include <SDL2/SDL.h>
#include "tutris/field.h"
#include "tutris/tetromino.h"
#include "tutris/tutris.h"

Field::Field() :
m_pos_x (0),
m_pos_y (0),
m_num_rows(0),
m_num_cols(0),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false),
m_grid (nullptr)
{

}

Field::Field(int x, int y, int cols, int rows) :
m_pos_x(x),
m_pos_y(y),
m_num_rows(rows),
m_num_cols(cols),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false)
{
    int num_grid_elems = m_num_rows * m_num_cols;
    m_grid = new int [num_grid_elems];

    // initialize to empty
    for (int i = 0; i < num_grid_elems; ++i)
    {
        int grid_cell = tutris::grid_cell_type::empty;
        m_grid[i] = grid_cell;
    }

    // create walls around perimeter of field
    int counter = 1;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        int grid_cell = tutris::grid_cell_type::wall;

        if ((counter % m_num_cols == 0))
        {
            m_grid[i] = grid_cell;
        }

        counter++;
    }

    // create walls around perimeter of field
    for (int i = 0; i < num_grid_elems; ++i)
    {
        int grid_cell = tutris::grid_cell_type::wall;

        if ((i % m_num_cols == 0))
        {
            m_grid[i] = grid_cell;
        }

        counter++;
    }

    // create walls around perimeter of field
    for (int i = (num_grid_elems - m_num_cols); i < num_grid_elems; ++i)
    {
        int grid_cell = tutris::grid_cell_type::wall;
        m_grid[i] = grid_cell;
    }
}

Field::~Field()
{
    if (m_grid != nullptr)
    {
        delete[] m_grid;
    }
}

void Field::render(SDL_Renderer *renderer)
{
    int draw_pos_x = m_pos_x;
    int draw_pos_y = m_pos_y;
    int counter = 1;
    int num_grid_elems = m_num_rows * m_num_cols;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        SDL_Rect field_square = {
            draw_pos_x,
            draw_pos_y,
            tutris::BLOCK_SIZE_PIXEL,
            tutris::BLOCK_SIZE_PIXEL
        };

        if (m_grid[i] == tutris::grid_cell_type::wall)
        {
            //draw dark grey square
            SDL_SetRenderDrawColor( renderer, 0xCC,0xCC,0xCC,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, 0x00,0x00,0x00,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        else if (m_grid[i] == tutris::grid_cell_type::piece)
        {
            //draw square
            SDL_SetRenderDrawColor( renderer, 0xFF,0x00,0x00,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, 0x00,0x00,0x00,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        else
        {
            SDL_SetRenderDrawColor( renderer, 0xFF,0xFF,0xFF,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, 0xCC,0xCC,0xCC,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        
        if ( counter % m_num_cols == 0 )
        {
            // move to next row
            draw_pos_y += tutris::BLOCK_SIZE_PIXEL;
            draw_pos_x = m_pos_x;
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
    int num_grid_elems = m_num_rows * m_num_cols;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        if ( (i > 0) && (i%m_num_cols == 0) )
        {
            std::cout << std::endl;
        }
        std::cout << m_grid[i] << " ";
    }

    std::cout << std::endl;
}

void Field::addPiece(tutris::tetromino_shape shape)
{
    if (m_piece_active)
    {
        return;
    }

    m_piece_active = true;

    // choose a random piece
    Tetromino piece(shape);
    m_piece_shape.clear();
    m_piece_shape = piece.getPiece();

    // place piece at top of grid
    m_piece_pos_x = (m_num_cols/2) - 1;
    m_piece_pos_y = 0;
    int local_cols = 2;
    for (int i = 0; i < 8; ++i)
    {
        // Calculate grid position (index) to start drawing piece
        // from top left to bottom right.
        // change grid entry to '1' for solid blocks
        // Draw order for default piece orientation:
        //   1 2
        //   3 4
        //   5 6
        //   7 8
        // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
        m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
    }
}

void Field::movePiece(tutris::move_direction dir)
{
    switch (dir)
    {
        case tutris::move_direction::left:
        {   
            int local_cols = 2;

            // Can we move to the left?
            bool can_move = true;
            int new_x_pos = m_piece_pos_x - 1;
            for (int i = 0; i < 4; ++i)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                int grid_index = (new_x_pos) + ((m_piece_pos_y + i) * m_num_cols);
                if (m_grid[grid_index] != 0)
                {
                    can_move = false;
                    break;
                }
            }

            if (can_move)
            {
                for (int i = 0; i < 8; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    int grid_index = (m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols);
                    m_grid[grid_index] = 0;
                }

                m_piece_pos_x = new_x_pos;

                for (int i = 0; i < 8; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
                }
            }
            
            break;
        }
        case tutris::move_direction::right:
        {
            int local_cols = 2;

            // // Can we move to the right?
            // bool can_move = true;
             int new_x_pos = m_piece_pos_x + 1;
            // for (int i = 0; i < 8; ++i)
            // {
            //     // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
            //     int grid_index = (new_x_pos) + ((m_piece_pos_y + i) * m_num_cols);
            //     if (i%local_cols != 0)
            //     {
            //         grid_index = (new_x_pos) + ((m_piece_pos_y + i) * m_num_cols);
            //     }

            //     if (m_grid[grid_index] != 0)
            //     {
            //         can_move = false;
            //         break;
            //     }
            // }

            if (true)
            {
                for (int i = 0; i < 8; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = 0;
                }
                                
                m_piece_pos_x = new_x_pos;

                for (int i = 0; i < 8; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
                }
            }

           
            break;
        }
        case tutris::move_direction::down:
        {
            int local_cols = 2;
  
            for (int i = 0; i < 8; ++i)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = 0;
            }
                            
            m_piece_pos_y += 1;

            for (int i = 0; i < 8; ++i)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

bool Field::isPieceActive()
{
    return m_piece_active;
}
