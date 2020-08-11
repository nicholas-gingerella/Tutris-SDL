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

    //TEST
    m_grid[85] = tutris::grid_cell_type::wall;
    m_grid[103] =tutris::grid_cell_type::wall;
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
        else if (m_grid[i] == tutris::grid_cell_type::piece || m_grid[i] == tutris::grid_cell_type::curr_piece)
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
    m_current_piece_rotation = 0; // default to upright orientation

    // place piece at top of grid
    m_piece_pos_x = (m_num_cols/2) - 1;
    m_piece_pos_y = 0;
    int local_cols = 4;
    for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
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
            int piece_cols = 4;

            // Can we move to the left?
            bool can_move = true;
            int new_x_pos = m_piece_pos_x - 1;
            for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                // only do collision checks on solid parts of the piece shape
                int curr_piece_index = i;
                int grid_piece_x = m_piece_pos_x + (curr_piece_index % piece_cols);
                int grid_piece_y = m_piece_pos_y + (curr_piece_index/piece_cols);
                //int current_grid_index = grid_piece_x + (grid_piece_y * m_num_cols);
                int target_grid_index = (grid_piece_x - 1) + (grid_piece_y * m_num_cols);

                if (m_piece_shape[curr_piece_index] == tutris::grid_cell_type::curr_piece)     
                {
                    if(m_grid[target_grid_index] != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index] != tutris::grid_cell_type::empty)
                    {
                        can_move = false;
                        break;
                    }
                }

                // int grid_index = (new_x_pos + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
                // if ((m_piece_shape[curr_piece_index] == 1) && (i%piece_cols == 0) && m_grid[grid_index] != 0)
                // {
                //     can_move = false;
                //     break;
                // }
            }

            if (can_move)
            {
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    int grid_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[grid_index] = 0;
                    }
                }

                m_piece_pos_x = new_x_pos;

                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[(m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols)] = m_piece_shape[i];
                    }
                }
            }
            
            break;
        }
        case tutris::move_direction::right:
        {
            int local_cols = 4;

            // // Can we move to the right?
             bool can_move = true;
             int new_x_pos = m_piece_pos_x + 1;

            // check collision with piece shape
            for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                // only do collision checks on solid parts of the piece shape
                int curr_piece_index = i;
                int grid_piece_x = m_piece_pos_x + (curr_piece_index % local_cols);
                int grid_piece_y = m_piece_pos_y + (curr_piece_index/local_cols);
                //int current_grid_index = grid_piece_x + (grid_piece_y * m_num_cols);
                int target_grid_index = (grid_piece_x + 1) + (grid_piece_y * m_num_cols);

                if (m_piece_shape[curr_piece_index] == tutris::grid_cell_type::curr_piece)     
                {
                    if(m_grid[target_grid_index] != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index] != tutris::grid_cell_type::empty)
                    {
                        can_move = false;
                        break;
                    }
                }
            }
        
            if (can_move)
            {
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    // only clear grid index if a solid part of the piece is currently occupying it
                    int grid_index = (m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols);
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                      m_grid[grid_index] = 0;
                    }
                }
                                
                m_piece_pos_x = new_x_pos;
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    int grid_index = (m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols);
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[grid_index] = m_piece_shape[i];
                    }
                }
            }

           
            break;
        }
        case tutris::move_direction::down:
        {
            int local_cols = 4;

             // // Can we move down?
             bool can_move = true;
             int new_y_pos = m_piece_pos_y + 1;

            // check collision with piece shape
            for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                // only do collision checks on solid parts of the piece shape
                int curr_piece_index = i;
                int grid_piece_x = m_piece_pos_x + (curr_piece_index % local_cols);
                int grid_piece_y = m_piece_pos_y + (curr_piece_index/local_cols);
                int target_grid_index = (grid_piece_x) + ((grid_piece_y + 1) * m_num_cols);

                if (m_piece_shape[curr_piece_index] == tutris::grid_cell_type::curr_piece)     
                {
                    if(m_grid[target_grid_index] != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index] != tutris::grid_cell_type::empty)
                    {
                        can_move = false;
                        break;
                    }
                }
            }
  
            if (can_move)
            {
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // clean up current position on grid
                    int grid_index = (m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols);
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[grid_index] = tutris::grid_cell_type::empty;
                    } 
                }

                m_piece_pos_y = new_y_pos;
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[(m_piece_pos_x + (i%local_cols)) + ((new_y_pos + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
                    }  
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

void Field::rotatePiece()
{
    int piece_cols = 4;
    // rotation 0 = upright
    // rotation 1 = left 90
    // rotation 2 = 180
    // rotation 3 = left 270 (or right -90)

    // Create rotated copy of our piece
    int rotated_piece [tutris::PIECE_DIMENSION];
    for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
    {
        int x = i%piece_cols;
        int y = i/piece_cols;
        int target_index = 12-4*x+y; // rotate indexes CCW
        rotated_piece[target_index] = m_piece_shape[i];
    }

    // Check if the rotated version of the piece collides
    // with anything on the field
    bool can_rotate = true;
    for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
    {
        // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
        int curr_piece_index = i;
        int grid_piece_x = m_piece_pos_x + (curr_piece_index % piece_cols);
        int grid_piece_y = m_piece_pos_y + (curr_piece_index/piece_cols);
        int target_grid_index = (grid_piece_x) + (grid_piece_y * m_num_cols);

        // only do collision checks on solid parts of the piece shape
        if (rotated_piece[curr_piece_index] == tutris::grid_cell_type::curr_piece)     
        {
            if(m_grid[target_grid_index] != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index] != tutris::grid_cell_type::empty)
            {
                can_rotate = false;
                std::cout << "Cannot Rotate" << std::endl;
                break;
            }
        }
    }

    //DEBUG: Print out rotated piece
    for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
    {
        if ( i > 0 && i%piece_cols == 0)
        {
            std::cout << std::endl;
        }
        std::cout << rotated_piece[i] << " ";
    }
    std::cout << std::endl;

    if (can_rotate)
    {
        // clear current grid indexes
        for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
        {
            // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
            // only clear grid index if a solid part of the piece is currently occupying it
            int grid_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
            if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
            {
                m_grid[grid_index] = 0;
            }
        }

        // update current piece shape to rotated version    
        for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
        {
            m_piece_shape[i] = rotated_piece[i];
        }

        // populate grid positions
        for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
        {
            // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
            int grid_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
            if (m_piece_shape[i] == tutris::grid_cell_type::curr_piece)
            {
                m_grid[grid_index] = m_piece_shape[i];
            }
        }
        
        std::cout << "Rotating piece from " << m_current_piece_rotation << std::endl;
        m_current_piece_rotation++;
        m_current_piece_rotation = m_current_piece_rotation % 4; //0,1,2,3,0,1,2,3,...
        std::cout << "new piece rotation is " << m_current_piece_rotation << std::endl;
    }
}


bool Field::isPieceActive()
{
    return m_piece_active;
}

bool Field::moveBlock(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y)
{
    int start_index = start_pos_x + (start_pos_y * m_num_cols);
    int end_index = end_pos_x + (end_pos_y * m_num_cols);
    if (m_grid[end_index] != 0)
    {
        return false;
    }

    m_grid[start_index] = tutris::grid_cell_type::empty;
    m_grid[end_index] = tutris::grid_cell_type::piece;

    return true;
}
