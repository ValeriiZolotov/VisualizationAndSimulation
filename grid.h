#ifndef GRID_H
#define GRID_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class Grid : public VisualObject
{
public:
    Grid();
    Grid(int size, int scale);
    ~Grid() override;
    void init(GLint matrixUniform) override;
    void draw() override;

private:
    int mSize{0};
    int mScale{0};
};

#endif // GRID_H
