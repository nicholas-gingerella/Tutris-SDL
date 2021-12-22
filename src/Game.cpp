#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Piece.h"
#include "tutris.h"


Game::Game() :
m_pos_x (0),
m_pos_y (0),
m_num_rows(0),
m_num_cols(0),
m_num_grid_cells(0),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false),
m_grid (nullptr),
m_time_remaining_ms(Tutris::MAX_TIME_MS)
{}


Game::Game(int x, int y, int cols, int rows) :
m_pos_x(x),
m_pos_y(y),
m_num_rows(rows),
m_num_cols(cols),
m_num_grid_cells(cols * rows),
m_piece_pos_x(0),
m_piece_pos_y(0),
m_piece_active(false),
m_time_remaining_ms(Tutris::MAX_TIME_MS)
{
    int num_grid_elems = m_num_rows * m_num_cols;
    m_grid = new Tutris::Block [num_grid_elems];

    Tutris::Block wall_block;
    wall_block.block_type = Tutris::GridCellType::empty;
    wall_block.color = Tutris::TUTRIS_COLOR_DARKGREY;

    // initialize to empty
    for (int i = 0; i < num_grid_elems; ++i)
    {
        Tutris::Block empty_block;
        empty_block.block_type = Tutris::GridCellType::empty;
        empty_block.color = Tutris::TUTRIS_COLOR_LIGHTGREY;
        m_grid[i] = empty_block;
    }

    // create walls around perimeter of field (left wall, start on 2nd row)
    int counter = 1;
    for (int i = 2*m_num_cols; i < num_grid_elems; ++i)
    {
        if ((counter % m_num_cols == 0))
        {
            m_grid[i] = Tutris::BLOCK_WALL;
        }
        counter++;
    }

    // create walls around perimeter of field (right wall, start on 2nd row)
    for (int i = 2*m_num_cols; i < num_grid_elems; ++i)
    {
        if ((i % m_num_cols == 0))
        {
            m_grid[i] = Tutris::BLOCK_WALL;
        }
        counter++;
    }

    // create walls around perimeter of field (bottom)
    for (int i = (num_grid_elems - m_num_cols); i < num_grid_elems; ++i)
    {
        m_grid[i] = Tutris::BLOCK_WALL;
    }
}


Game::~Game()
{
    if (m_grid != nullptr)
    {
        delete[] m_grid;
    }
}


void Game::render(SDL_Renderer *renderer)
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
            Tutris::BLOCK_SIZE_PIXEL,
            Tutris::BLOCK_SIZE_PIXEL
        };

        // Check cell type of block at this grid index
        if (m_grid[i].block_type == Tutris::GridCellType::wall ||
            m_grid[i].block_type == Tutris::GridCellType::piece ||
            m_grid[i].block_type == Tutris::GridCellType::curr_piece ||
            m_grid[i].block_type == Tutris::GridCellType::clearing)
        {
            SDL_SetRenderDrawColor( renderer, m_grid[i].color.r,m_grid[i].color.g,m_grid[i].color.b,0xFF);
            SDL_RenderFillRect(renderer, &field_square);
            SDL_SetRenderDrawColor( renderer, m_grid[i].outline_color.r, m_grid[i].outline_color.g, m_grid[i].outline_color.b, 0xFF);
            SDL_RenderDrawRect(renderer, &field_square);
        }
        else if (m_grid[i].block_type == Tutris::GridCellType::empty)
        {
            // Only start drawing empty field blocks after the first two
            // rows of the field. Makes it easier to see when a piece
            // crosses the top boundary when game over happens.
            if (i > m_num_cols*2)
            {
                SDL_SetRenderDrawColor( renderer, m_grid[i].color.r, m_grid[i].color.r, m_grid[i].color.r, 0xFF);
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
            draw_pos_y += Tutris::BLOCK_SIZE_PIXEL;
            draw_pos_x = m_pos_x;
        }
        else 
        {
            draw_pos_x += Tutris::BLOCK_SIZE_PIXEL;
        }

        counter++;
    }
}


bool Game::addPiece(Tutris::PieceShape shape)
{
    // Only one piece can be active on the field
    // at any time. Don't add a new piece if we
    // still have an active piece on the field.
    if (m_piece_active)
    {
        return false;
    }

    // Set the shape of the new piece
    Piece piece;
    if (shape == Tutris::PieceShape::random)
    {
        // Choose a random piece shape and color for the new piece
        unsigned int random_shape = rand() % int(Tutris::PieceShape::random);
        unsigned int random_piece_color = rand() % Tutris::PieceColor::piece_random;
        piece.setShape(static_cast<Tutris::PieceShape>(random_shape), static_cast<Tutris::PieceColor>(random_piece_color));
    }
    else
    {
        piece.setShape(shape);
    }
    
    // replace the currently active piece with the new one
    m_active_piece.clear();
    m_active_piece = piece.getPiece();

    // place piece at the center top of the field
    m_piece_pos_x = (m_num_cols/2) - 1;
    m_piece_pos_y = 0;

    // can we place the piece at this starting point? If not, then
    // return false (likely game over)
    if ( !canMoveCurrPieceToPosition(m_piece_pos_x, m_piece_pos_y) )
    {
        return false;
    }

    moveCurrPieceToPosition(m_piece_pos_x, m_piece_pos_y);
    m_piece_active = true;
    return true;
}


void Game::movePiece(Tutris::MoveDirection dir)
{
    if (m_piece_active == false)
    {
        // if there is no current piece to move
        // don't bother with move logic
        return;
    }

    // Attempt to move the current piece in the specified
    // direction
    switch (dir)
    {
        case Tutris::MoveDirection::left:
        {   
            int target_x_pos = m_piece_pos_x - 1;
            if ( canMoveCurrPieceToPosition(target_x_pos, m_piece_pos_y) )
            {
                moveCurrPieceToPosition(target_x_pos, m_piece_pos_y);
            }
            break;
        }
        case Tutris::MoveDirection::right:
        {
            int target_x_pos = m_piece_pos_x + 1;
            if ( canMoveCurrPieceToPosition(target_x_pos, m_piece_pos_y) )
            {
                moveCurrPieceToPosition(target_x_pos, m_piece_pos_y);
            }
            break;
        }
        case Tutris::MoveDirection::down:
        {

            int target_y_pos = m_piece_pos_y + 1;
            if ( canMoveCurrPieceToPosition(m_piece_pos_x, target_y_pos) )
            {
                moveCurrPieceToPosition(m_piece_pos_x, target_y_pos);
            }
            else
            {
                // If we can't move down anymore, then it's time to
                // lock the piece in place and prepare to add another
                // piece to the field.
                lockCurrPieceInPlace();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}


void Game::rotatePiece()
{
    if (canRotateCurrPiece())
    {
        // NOTE: block array must be deleted (dynamically allocated)
        Tutris::Block* rotated_piece = getRotatedPieceCopy();

        // Clear block positions from old piece orientation
        clearCurrPiecePosition();

        // update current piece shape to rotated version    
        for (int i = 0; i < Tutris::PIECE_DIMENSION; ++i)
        {
            m_active_piece[i] = rotated_piece[i];
        }
        
        // rotated piece copy no longer needed.
        // clean it up.
        delete rotated_piece;

        // update the current piece to the new shape by "moving" it in place
        moveCurrPieceToPosition(m_piece_pos_x, m_piece_pos_y);
    }
}


bool Game::isPieceActive()
{
    return m_piece_active;
}


bool Game::moveBlock(int grid_index, Tutris::MoveDirection dir, int num_moves)
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
        case Tutris::MoveDirection::down:
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
    if (m_grid[target_index].block_type != Tutris::GridCellType::empty)
    {
        return false;
    }

    Tutris::Block tmpBlock = m_grid[start_index];
    m_grid[start_index] = Tutris::BLOCK_EMPTY;
    m_grid[target_index] = tmpBlock;

    return true;
}


unsigned int Game::getNumGridCells()
{
    return m_num_grid_cells;
}


unsigned int Game::getNumRows()
{
    return m_num_rows;
}


unsigned int Game::getNumCols()
{
    return m_num_cols;
}


std::vector<int> Game::scanField()
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
        if (m_grid[i].block_type == Tutris::GridCellType::empty || m_grid[i].block_type == Tutris::GridCellType::clearing)
        {
            clear_row_found = false;
        }
    }

    // return list of row numbers that can be cleared
    return clear_row_nums;
}


void Game::markClearRows(std::vector<int> clear_rows)
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
            m_grid[i] = Tutris::BLOCK_CLEARING;
        }
    }
}


void Game::removeRows(std::vector<int> clear_rows)
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
            m_grid[i] = Tutris::BLOCK_EMPTY;
        }
    }
}


void Game::collapseBlocks()
{
    // scan field rows from bottom to top
    std::vector<int> clear_row_nums;
    for (int i = m_num_grid_cells - 1 ; i >= 0; i--)
    {
        // If there is a space in the row, then this is not a row that
        // can be marked for clearing
        if (m_grid[i].block_type == Tutris::GridCellType::piece) //clearing check can go away after actually clearing cells to empty
        {
            // while block can move down, move block down. (Collapse logic)
            int curr_block_index = i;
            while(true)
            {
                 if (!moveBlock(curr_block_index, Tutris::MoveDirection::down))
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


void Game::shiftBlocks(std::vector<int> rows)
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
        if (m_grid[i].block_type == Tutris::GridCellType::piece)
        {
            int curr_block_index = i;
            moveBlock(curr_block_index, Tutris::MoveDirection::down, num_drops);
        }
    }    
}


int Game::pieceIndexToFieldIndex(int x_pos, int y_pos, int piece_index)
{
    // Each shape stored as a 4x4 grid: 4 rows and 4 columns.
    // Needed to determine what row/col we are looking at for
    // the piece shape.
    int num_piece_shape_cols = 4;

    // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
    int grid_piece_x = x_pos + (piece_index % num_piece_shape_cols);
    int grid_piece_y = y_pos + (piece_index/num_piece_shape_cols);
    int target_grid_index = (grid_piece_x) + (grid_piece_y * m_num_cols);

    return target_grid_index;
}


void Game::clearCurrPiecePosition()
{
    for (int i = 0; i < Tutris::PIECE_DIMENSION; ++i)
    {
        int curr_piece_index = i;
        int grid_index = pieceIndexToFieldIndex(m_piece_pos_x, m_piece_pos_y, curr_piece_index);
        if (m_active_piece[curr_piece_index].block_type == Tutris::GridCellType::curr_piece)
        {
            m_grid[grid_index] = Tutris::BLOCK_EMPTY;
        }
    }
}


void Game::moveCurrPieceToPosition(int new_x_pos, int new_y_pos)
{
    // Clears pieces from current position prior to setting
    // the new position
    clearCurrPiecePosition();

    // set piece blocks at new position
    for (int i = 0; i < Tutris::PIECE_DIMENSION; ++i)
    {
        if (m_active_piece[i].block_type == Tutris::GridCellType::curr_piece)
        {
            int curr_piece_index = i;
            int target_index = pieceIndexToFieldIndex(new_x_pos, new_y_pos, curr_piece_index);
            m_grid[target_index] = m_active_piece[curr_piece_index];
        }
    }

    // set current piece coordinates to new position
    m_piece_pos_x = new_x_pos;
    m_piece_pos_y = new_y_pos;
}


void Game::lockCurrPieceInPlace()
{
    // Locking piece means we no longer have a piece
    // active on the field
    m_piece_active = false;

    // lock and set piece in place
    for (int i = 0; i < Tutris::PIECE_DIMENSION; ++i)
    {
        // Index for x/y-coordinate in grid: x-pos + y-pos*num_grid_cols
        if (m_active_piece[i].block_type == Tutris::GridCellType::curr_piece)
        {
            // set the grid spot to a locked piece rather than an active piece
            int target_index = pieceIndexToFieldIndex(m_piece_pos_x, m_piece_pos_y, i);
            m_grid[target_index].block_type = Tutris::GridCellType::piece;
        }
    }
}


bool Game::canMoveCurrPieceToPosition(int new_x_pos, int new_y_pos)
{
    // check collision with piece shape
    for (int i = 0; i < Tutris::PIECE_DIMENSION; i++)
    {
        // only do collision checks on solid parts of the piece shape
        if (m_active_piece[i].block_type == Tutris::GridCellType::curr_piece)     
        {
            int target_grid_index = pieceIndexToFieldIndex(new_x_pos, new_y_pos, i);
            if(m_grid[target_grid_index].block_type != Tutris::GridCellType::curr_piece &&
                m_grid[target_grid_index].block_type != Tutris::GridCellType::empty)
            {
                return false;
            }
        }
    }

    return true;
}


bool Game::canRotateCurrPiece()
{
    // Create rotated copy of our piece (MUST CLEANUP POINTER)
    Tutris::Block* rotated_piece = getRotatedPieceCopy();

    // Check if the rotated version of the piece collides
    // with anything on the field
    for (int i = 0; i < Tutris::PIECE_DIMENSION; i++)
    {
        int target_grid_index = pieceIndexToFieldIndex(m_piece_pos_x, m_piece_pos_y, i);

        // only do collision checks on solid parts of the piece shape
        if (rotated_piece[i].block_type == Tutris::GridCellType::curr_piece)     
        {
            if(m_grid[target_grid_index].block_type != Tutris::GridCellType::curr_piece &&
                m_grid[target_grid_index].block_type != Tutris::GridCellType::empty)
            {
                delete rotated_piece;
                return false;
            }
        }
    }

    delete rotated_piece;
    return true;
}


Tutris::Block* Game::getRotatedPieceCopy()
{
    // Each shape stored as a 4x4 grid: 4 rows and 4 columns.
    // Needed to determine what row/col we are looking at for
    // the piece shape.
    int num_piece_shape_cols = 4;

    // Create rotated copy of our piece
    Tutris::Block* rotated_piece  = new Tutris::Block[Tutris::PIECE_DIMENSION];
    for (int i = 0; i < Tutris::PIECE_DIMENSION; i++)
    {
        int x = i % num_piece_shape_cols;
        int y = i / num_piece_shape_cols;
        int target_index = 12 - 4*x + y; // rotate indexes CCW
        rotated_piece[target_index] = m_active_piece[i];
    }

    return rotated_piece;
}



