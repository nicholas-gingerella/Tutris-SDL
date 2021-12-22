#ifndef FIELD_H
#define FIELD_H
#include <algorithm>
#include <vector>
#include "tutris.h"
class Game
{
public:
    Game();
    Game(int x, int y, int cols, int rows);
    ~Game();
    void render(SDL_Renderer *renderer);
    void printField();
    bool addPiece(Tutris::PieceShape piece);
    void movePiece(Tutris::MoveDirection dir);
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
    bool moveBlock(int grid_index, Tutris::MoveDirection dir, int num_moves = 1);


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
    std::vector<Tutris::Block> m_active_piece;
    Tutris::Block* m_grid;
    unsigned int m_time_remaining_ms;
    int pieceIndexToFieldIndex(int x_pos, int y_pos, int piece_index);
    void clearCurrPiecePosition();
    void moveCurrPieceToPosition(int x_pos, int y_pos);
    bool canMoveCurrPieceToPosition(int new_x_pos, int new_y_pos);
    void lockCurrPieceInPlace();
    bool canRotateCurrPiece();
    Tutris::Block* getRotatedPieceCopy();


};
#endif