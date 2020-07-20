#ifndef FIELD_H
#define FIELD_H
class Field
{
    public:
    void render();

    private:
    int x, y;
    int w, h;
    // grid 16 x 32
    int grid[16 * 32] = {0};
    // bg color
};
#endif