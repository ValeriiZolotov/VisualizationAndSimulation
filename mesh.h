#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"
#include "objloader.h"
#include "boundingbox.h"
#include "trianglesurface.h"

class BoundingBox;


class Mesh : public VisualObject
{
public:
    Mesh(){}
    Mesh(QString filename);
    Mesh(OBJLoader* loader, TriangleSurface* tr);
    virtual ~Mesh() override;
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    BoundingBox* getBox(){return mBBox;}
    float setHeight();

protected:
    BoundingBox* mBBox;
    TriangleSurface* mTerrain;
    vec3 mPosition;

};
#endif // MESH_H
