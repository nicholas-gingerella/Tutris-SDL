#ifndef FIELD_H
#define FIELD_H
class Field
{
public:
    Field();
    ~Field();
    void render();

private:
    int pos_x, pos_y;
    int num_rows, num_cols;
    int grid;
};
#endif