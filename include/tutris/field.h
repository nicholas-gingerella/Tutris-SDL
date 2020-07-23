#ifndef FIELD_H
#define FIELD_H
class Field
{
public:
    Field();
    Field(int x, int y, int rows, int cols);
    ~Field();
    void render();
    bool isFilled();
    void printField();

private:
    int pos_x, pos_y;
    int num_rows, num_cols;
    int *grid;

};
#endif