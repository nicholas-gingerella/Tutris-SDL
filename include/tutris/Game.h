#ifndef FIELD_H
#define FIELD_H
#include <algorithm>
#include <vector>
#include "tutris/tutris.h"
class Game
{
public:
    Game();
    Game(int x, int y, int cols, int rows);
    ~Game();
    void render(SDL_Renderer *renderer);
    bool isFilled();
    void printField();
    bool addPiece(ns_Tutris::tetromino_shape piece);
    void movePiece(ns_Tutris::move_direction dir);
    void rotatePiece();
    bool isPieceActive();
    unsigned int getNumGridCells();
    unsigned int getNumRows();
    unsigned int getNumCols();
    std::vector<int> scanField();
    void markClearRows(std::vector<int> clear_rows);
    void removeRows(std::vector<int> clear_rows);
    void collapseBlocks();
    void shiftBlocks(std::vector<int> rows);
    bool moveBlock(int grid_index, ns_Tutris::move_direction dir, int num_moves = 1);


private:
    int m_pos_x;
    int m_pos_y;
    int m_num_rows;
    int m_num_cols;
    int m_num_grid_cells;
    int m_piece_pos_x;
    int m_piece_pos_y;
    bool m_piece_active;
    int m_current_piece_rotation;
    std::vector<ns_Tutris::block> m_piece_shape;
    ns_Tutris::block* m_grid;
    unsigned int m_time_remaining_ms;
};
#endif