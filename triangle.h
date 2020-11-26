#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "visualobject.h"

class Triangle: public VisualObject
{
public:
    Triangle();
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    virtual ~Triangle() override;
    void defineTriangle(std::vector<Vertex>& v);
};

#endif // TRIANGLE_H
