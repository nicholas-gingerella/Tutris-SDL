#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include "tutris/tutris.h"
class Field
{
public:
    Field();
    Field(int x, int y, int cols, int rows);
    ~Field();
    void render(SDL_Renderer *renderer);
    bool isFilled();
    void printField();
    bool addPiece(tutris::tetromino_shape piece);
    void movePiece(tutris::move_direction dir);
    void rotatePiece();
    bool isPieceActive();
    bool moveBlock(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y);
    unsigned int getNumGridCells();
    unsigned int getNumRows();
    unsigned int getNumCols();
    std::vector<int> scanField();
    void markClearRows(std::vector<int> clear_rows);
    void removeRows(std::vector<int> clear_rows);
    void shiftFallingBlocks();
    bool moveBlock(int grid_index, tutris::move_direction dir, int num_moves = 1);


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
    std::vector<int> m_piece_shape;
    int *m_grid;
};
#endif