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
    void addPiece(tutris::tetromino_shape piece);
    void movePiece(tutris::move_direction dir);
    bool isPieceActive();
    bool moveBlock(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y);


private:
    int m_pos_x;
    int m_pos_y;
    int m_num_rows;
    int m_num_cols;
    int m_piece_pos_x;
    int m_piece_pos_y;
    bool m_piece_active;
    std::vector<int> m_piece_shape;
    int *m_grid;
};
#endif