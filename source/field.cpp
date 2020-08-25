#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "tutris/field.h"
#include "tutris/tetromino.h"
#include "tutris/tutris.h"

Field::Field() :
m_pos_x (0),
m_pos_y (0),
m_num_rows(0),
m_num_cols(0),
m_num_grid_cells(0),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false),
m_grid (nullptr)
{}


Field::Field(int x, int y, int cols, int rows) :
m_pos_x(x),
m_pos_y(y),
m_num_rows(rows),
m_num_cols(cols),
m_num_grid_cells(cols * rows),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false)
{
    int num_grid_elems = m_num_rows * m_num_cols;
    m_grid = new tutris::block [num_grid_elems];

    tutris::block wall_block;
    wall_block.block_type = tutris::grid_cell_type::empty;
    wall_block.color = tutris::COLOR_DARKGREY;

    // initialize to empty
    for (int i = 0; i < num_grid_elems; ++i)
    {
        tutris::block empty_block;
        empty_block.block_type = tutris::grid_cell_type::empty;
        empty_block.color = tutris::COLOR_LIGHTGREY;
        m_grid[i] = empty_block;
    }

    // create walls around perimeter of field (left wall, start on 2nd row)
    int counter = 1;
    for (int i = 2*m_num_cols; i < num_grid_elems; ++i)
    {
        if ((counter % m_num_cols == 0))
        {
            m_grid[i] = tutris::BLOCK_WALL;
        }
        counter++;
    }

    // create walls around perimeter of field (right wall, start on 2nd row)
    for (int i = 2*m_num_cols; i < num_grid_elems; ++i)
    {
        if ((i % m_num_cols == 0))
        {
            m_grid[i] = tutris::BLOCK_WALL;
        }
        counter++;
    }

    // create walls around perimeter of field (bottom)
    for (int i = (num_grid_elems - m_num_cols); i < num_grid_elems; ++i)
    {
        m_grid[i] = tutris::BLOCK_WALL;
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

        // // Check cell type of block at this grid index



        if (m_grid[i].block_type == tutris::grid_cell_type::wall ||
            m_grid[i].block_type == tutris::grid_cell_type::piece ||
            m_grid[i].block_type == tutris::grid_cell_type::curr_piece ||
            m_grid[i].block_type == tutris::grid_cell_type::clearing)
        {
            SDL_SetRenderDrawColor( renderer, m_grid[i].color.r,m_grid[i].color.g,m_grid[i].color.b,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, 0x00,0x00,0x00,0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        else if (m_grid[i].block_type == tutris::grid_cell_type::empty)
        {
            if (i > m_num_cols*2)
            {
                SDL_SetRenderDrawColor( renderer, 0x7D,0x7D,0x7D,0xFF);
                SDL_RenderFillRect(renderer, &field_square);
            }
        }
        else
        {
            // Unknown cell type found
        }
        
        
        //  Current index is the start of the next row
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
            
        }
    } 
}


bool Field::addPiece(tutris::tetromino_shape shape)
{
    if (m_piece_active)
    {
        return false;
    }

    Tetromino piece;

    // choose a random piece
    if (shape == tutris::tetromino_shape::random)
    {
        // get pseudo random number from 0 to num pieces
        // random is the last enum entry, so it will be the
        // upper bound
        unsigned int random_shape = rand() % tutris::tetromino_shape::random;
        piece.setShape(static_cast<tutris::tetromino_shape>(random_shape), tutris::COLOR_LIGHTBLUE);
    }
    else
    {
        // set to passed in shape
        piece.setShape(shape);
    }
    
    m_piece_shape.clear();
    m_piece_shape = piece.getPiece();

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
        // +-------------------+
        // |  0 |  1 |  2 |  3 |
        // +-------------------+
        // |  4 |  5 |  6 |  7 |
        // +-------------------+
        // |  8 |  9 | 10 | 11 |
        // +-------------------+
        // | 12 | 13 | 14 | 15 |
        // +-------------------+

        // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols

        // Does adding the piece here cause a collision?
        // If so, game over
        // // Can we move to the right?
        bool can_move = true;

        // check collision with piece shape
        for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
        {
            // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
            // only do collision checks on solid parts of the piece shape
            int curr_piece_index = i;
            int grid_piece_x = m_piece_pos_x + (curr_piece_index % local_cols);
            int grid_piece_y = m_piece_pos_y + (curr_piece_index/local_cols);
            int target_grid_index = (grid_piece_x) + (grid_piece_y * m_num_cols);

            if (m_piece_shape[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
            {
                if( (m_grid[target_grid_index].block_type != tutris::grid_cell_type::curr_piece) &&
                    (m_grid[target_grid_index].block_type != tutris::grid_cell_type::empty))
                {
                    can_move = false;
                    break;
                }
            }
        }

        if (!can_move)
        {
            return false;
        }
        else
        {
            //place piece on grid
            for (int i = 0; i < tutris::PIECE_DIMENSION; i++)
            {
                // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                // only do collision checks on solid parts of the piece shape
                int curr_piece_index = i;
                int grid_piece_x = m_piece_pos_x + (curr_piece_index % local_cols);
                int grid_piece_y = m_piece_pos_y + (curr_piece_index/local_cols);
                int target_grid_index = grid_piece_x + (grid_piece_y * m_num_cols);

                if (m_piece_shape[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
                {
                    m_grid[target_grid_index] = m_piece_shape[i];
                }
            }

            m_piece_active = true;
        }
    }

    return true;
}


void Field::movePiece(tutris::move_direction dir)
{
    if (m_piece_active == false)
    {
        return;
    }

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
                int target_grid_index = (grid_piece_x - 1) + (grid_piece_y * m_num_cols);

                if (m_piece_shape[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
                {
                    if( (m_grid[target_grid_index].block_type != tutris::grid_cell_type::curr_piece) && 
                        (m_grid[target_grid_index].block_type != tutris::grid_cell_type::empty) )
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
                    int grid_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[grid_index] = tutris::BLOCK_EMPTY;
                    }
                }

                m_piece_pos_x = new_x_pos;

                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                        int target_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
                        m_grid[target_index] = m_piece_shape[i];
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
                int target_grid_index = (grid_piece_x + 1) + (grid_piece_y * m_num_cols);

                if (m_piece_shape[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
                {
                    if(m_grid[target_grid_index].block_type != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index].block_type != tutris::grid_cell_type::empty)
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
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                      m_grid[grid_index] = tutris::BLOCK_EMPTY;
                    }
                }
                                
                m_piece_pos_x = new_x_pos;
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    int grid_index = (m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols);
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
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

                if (m_piece_shape[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
                {
                    if(m_grid[target_grid_index].block_type != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index].block_type != tutris::grid_cell_type::empty)
                    {
                        can_move = false;
                        m_piece_active = false; //if we can't move down, then time to lock the piece
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
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[grid_index] = tutris::BLOCK_EMPTY;
                    } 
                }

                m_piece_pos_y = new_y_pos;
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                        m_grid[(m_piece_pos_x + (i%local_cols)) + ((new_y_pos + (i/local_cols)) * m_num_cols)] = m_piece_shape[i];
                    }  
                }
            }
            else
            {
                //if we can't move down, then time to lock the piece
                m_piece_active = false;

                // lock and set piece in place
                for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
                {
                    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
                    if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
                    {
                        // set the grid spot to a locked piece rather than an active piece
                        m_grid[(m_piece_pos_x + (i%local_cols)) + ((m_piece_pos_y + (i/local_cols)) * m_num_cols)].block_type = tutris::grid_cell_type::piece;
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

    // Create rotated copy of our piece
    tutris::block rotated_piece [tutris::PIECE_DIMENSION];
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
        if (rotated_piece[curr_piece_index].block_type == tutris::grid_cell_type::curr_piece)     
        {
            if(m_grid[target_grid_index].block_type != tutris::grid_cell_type::curr_piece && m_grid[target_grid_index].block_type != tutris::grid_cell_type::empty)
            {
                can_rotate = false;
                break;
            }
        }
    }

    if (can_rotate)
    {
        // clear current grid indexes
        for (int i = 0; i < tutris::PIECE_DIMENSION; ++i)
        {
            // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
            // only clear grid index if a solid part of the piece is currently occupying it
            int grid_index = (m_piece_pos_x + (i%piece_cols)) + ((m_piece_pos_y + (i/piece_cols)) * m_num_cols);
            if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
            {
                m_grid[grid_index] = tutris::BLOCK_EMPTY;
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
            if (m_piece_shape[i].block_type == tutris::grid_cell_type::curr_piece)
            {
                m_grid[grid_index] = m_piece_shape[i];
            }
        }
    }
}


bool Field::isPieceActive()
{
    return m_piece_active;
}


// bool Field::moveBlock(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y)
// {
//     int start_index = start_pos_x + (start_pos_y * m_num_cols);
//     int end_index = end_pos_x + (end_pos_y * m_num_cols);
//     if (m_grid[end_index].block_type != tutris::grid_cell_type::empty)
//     {
//         return false;
//     }

//     m_grid[start_index] = tutris::BLOCK_EMPTY;
//     m_grid[end_index].block_type = tutris::grid_cell_type::piece;

//     return true;
// }


bool Field::moveBlock(int grid_index, tutris::move_direction dir, int num_moves)
{
    // Current block location info
    int start_index = grid_index;
    int curr_x_pos = grid_index%m_num_cols;
    int curr_y_pos = grid_index/m_num_cols;

    // Target location default values
    int target_index = start_index;
    int target_y_pos = curr_y_pos;
    int target_x_pos = curr_x_pos;

    switch(dir)
    {
        case tutris::move_direction::down:
        {

            target_y_pos = curr_y_pos + num_moves; // +1 is down 1 on the board (top left is 0,0)
            break;
        }
        default:
        {
            return false;
            break;
        }
    }

    target_index = target_x_pos + (target_y_pos * m_num_cols);
    if (m_grid[target_index].block_type != tutris::grid_cell_type::empty)
    {
        return false;
    }

    tutris::block tmpBlock = m_grid[start_index];
    m_grid[start_index] = tutris::BLOCK_EMPTY;
    m_grid[target_index] = tmpBlock;

    return true;
}


unsigned int Field::getNumGridCells()
{
    return m_num_grid_cells;
}


unsigned int Field::getNumRows()
{
    return m_num_rows;
}


unsigned int Field::getNumCols()
{
    return m_num_cols;
}


std::vector<int> Field::scanField()
{
    // Scan field for filled in rows
    bool clear_row_found = true;
    std::vector<int> clear_row_nums;
    for (int i = 0; i < m_num_grid_cells; i++)
    {
        // beginning of rows: 0, field width, 2 * field width, 3 * field width, etc.
        if (i%m_num_cols == 0)
        {
            // start of new row
            // Only check for cleared row if we have already passed the first row
            // index (0 = top left cell of grid)
            if ((i >= m_num_cols) && clear_row_found)
            {
                // Add the row number to a list of rows to be cleared.
                // row 0 (top) - row num_rows-1 (bottom)
                // using the row number rather than index makes consecutive
                // clear row calculation easier later on
                int current_row_number = i/m_num_cols;
                clear_row_nums.push_back(current_row_number - 1); // row ABOVE current row needs to be cleared.
            }
            else
            {
                // reset to true to prepare for next row scan
                clear_row_found = true;
            }
        }

        // If there is a space in the row, then this is not a row that
        // can be marked for clearing
        if (m_grid[i].block_type == tutris::grid_cell_type::empty || m_grid[i].block_type == tutris::grid_cell_type::clearing)
        {
            clear_row_found = false;
        }
    }

    // return list of row numbers that can be cleared
    return clear_row_nums;
}


void Field::markClearRows(std::vector<int> clear_rows)
{
    // Mark cells in rows that need to be cleared
    std::vector<int>::iterator it;
    for (it = clear_rows.begin(); it != clear_rows.end(); it++)
    {
        int row_number = *it;
        int row_start_index = (row_number * m_num_cols);
        int row_end_index = row_start_index + (m_num_cols - 1);
        row_start_index++; // increase by 1, first col is a wall
        for (int i = row_start_index; i < row_end_index; i++)
        {
            m_grid[i] = tutris::BLOCK_CLEARING;
        }
    }
}


void Field::removeRows(std::vector<int> clear_rows)
{
    std::vector<int>::iterator it;
    for (it = clear_rows.begin(); it != clear_rows.end(); it++)
    {
        int row_number = *it;
        int row_start_index = (row_number * m_num_cols);
        int row_end_index = row_start_index + (m_num_cols - 1);
        row_start_index++; // increase by 1, first col is a wall
        for (int i = row_start_index; i < row_end_index; i++)
        {
            m_grid[i] = tutris::BLOCK_EMPTY;
        }
    }
}


void Field::collapseBlocks()
{
    // Scan field for filled in rows

    // scan field rows from bottom to top
    std::vector<int> clear_row_nums;
    for (int i = m_num_grid_cells - 1 ; i >= 0; i--)
    {
        // If there is a space in the row, then this is not a row that
        // can be marked for clearing
        if (m_grid[i].block_type == tutris::grid_cell_type::piece) //clearing check can go away after actually clearing cells to empty
        {
            // while block can move down, move block down. (Collapse logic)
            int curr_block_index = i;
            while(true)
            {
                 
                 if (!moveBlock(curr_block_index, tutris::move_direction::down))
                 {
                     break;
                 }

                // moved down successfully, calculate new index for the next move down
                int target_y = (curr_block_index/m_num_cols) + 1;
                int current_x =  curr_block_index%m_num_cols;
                curr_block_index = current_x + (target_y * m_num_cols);
            }
        }
    }
}


void Field::shiftBlocks(std::vector<int> rows)
{
    // Scan field for filled in rows
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    int num_drops = 0;

    // scan field rows from bottom to top
    std::vector<int> clear_row_nums;
    for (int i = m_num_grid_cells - 1 ; i >= 0; i--)
    {
        int curr_row = i/m_num_cols;
        bool is_first_of_row = i%m_num_cols == 0 ? true : false; // only want the check below to run once per row

        // start of a clear row, increment number of drops for the next blocks we see on the way up the grid
        if ( is_first_of_row && std::find(rows.begin(), rows.end(),curr_row) != rows.end())
        {
            num_drops++;
        }

        // If there is a space in the row, then this is not a row that
        // can be marked for clearing
        if (m_grid[i].block_type == tutris::grid_cell_type::piece)
        {
            int curr_block_index = i;
            moveBlock(curr_block_index, tutris::move_direction::down, num_drops);
        }
    }    
}


