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
    void toTriagles();
    void setNormals();
    std::vector<_Triangle>& getTriangles(){return mTriangles;}
    int findBall(vec2 playerPosition);
    void barycentricCoordinates(vec2 v1, vec2 v2, vec2 v3,vec2 playerPosition,vec3 *barycCoordinates);
private:
    std::vector<int> mNeighbours;
    std::vector<_Triangle> mTriangles;

};

#endif // TRIANGLESURFACE_H
