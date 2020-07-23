#include "tutris/field.h"
#include <iostream>

Field::Field() :
pos_x (0),
pos_y (0),
num_rows(0),
num_cols(0),
grid (nullptr)
{

}

Field::Field(int x, int y, int rows, int cols) :
pos_x(x),
pos_y(y),
num_rows(rows),
num_cols(cols)
{
    int num_grid_elems = num_rows * num_cols;
    grid = new int [num_grid_elems];

    for (int i = 0; i < num_grid_elems; ++i)
    {
        grid[i] = 0;
    }

}

Field::~Field()
{
    if (grid != nullptr)
    {
        delete[] grid;
    }
}

void Field::render()
{

}

bool Field::isFilled()
{
    return false;
}

void Field::printField()
{
    int num_grid_elems = num_rows * num_cols;
    for (int i = 0; i < num_grid_elems; ++i)
    {
        std::cout << grid[i] << " ";
    }

    std::cout << std::endl;
}
