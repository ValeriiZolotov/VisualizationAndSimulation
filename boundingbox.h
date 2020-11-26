#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "visualobject.h"
#include "mesh.h"

class Mesh;
class BoundingBox : public VisualObject
{
public:
    BoundingBox(VisualObject* mesh);
    void init(GLint matrixUniform) override;
    void draw() override;
    void setBox(VisualObject* mesh);
    void update(VisualObject* mesh);
    bool intersect(BoundingBox* other);
    vec3& getCenter(){return mCenter;}
    vec3& getMax(){return mMax;}
    vec3& getMin(){return mMin;}

private:
    GLfloat min_x, max_x,
            min_y, max_y,
            min_z, max_z;
    vec3 mSize;
    vec3 mCenter;
    vec3 mMin;
    vec3 mMax;


};


#endif // BOUNDINGBOX_H
