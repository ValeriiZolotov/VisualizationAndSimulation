#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include "vertex.h"
#include <vector>
#include <cmath>
#include <qimage.h>
#include <QTextStream>
#include <QFile>

struct _Triangle
{
    _Triangle(){}
    _Triangle(Vertex a,Vertex b,Vertex c,vec3 brCoor, int id)
        :m_a(a),m_b(b),m_c(c),m_barycCoor(brCoor),m_id(id){}
    Vertex m_a;
    Vertex m_b;
    Vertex m_c;
    vec3 m_barycCoor;
    int m_id;

};

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(const QString filNavn);
    void readFile(const QString filNavn);
    void init(GLint matrixUniform) override;
    void draw() override;
private:
    std::vector<int> mNeighbours;
};

#endif // TRIANGLESURFACE_H
