#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <vector>
#include <cmath>
#include <qimage.h>
#include <QTextStream>
#include <QFile>
#include "visualobject.h"
#include "vertex.h"
#include "delaunator.h"


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
    std::vector<int> m_neighbours;


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
    void readLFile(const QString filNavn);
    std::vector<_Triangle>& getTriangles(){return mTriangles;}
    int findBall(vec2 playerPosition);
    void barycentricCoordinates(vec2 v1, vec2 v2, vec2 v3,vec2 playerPosition,vec3 *barycCoordinates);
    void generateSurface(int sizeX,int sizeY);
    void setNeighbors();
    void delaunay(const std::vector<double>& coord);

    std::vector<double> mHeights;
    std::vector<double> mCoords;
    float findObject(vec2 position);

    void scale(double x, double maxX, double minX, double maxXS, double minXS,
                 double y, double maxY, double minY, double maxYS, double minYS,double &xNew,double &yNew);
    double scale(double x, double maxX, double minX, double maxXS, double minXS);
    void scaleTerrainData(std::vector<double>& XCoords,
                          std::vector<double>& YCoords,
                          std::vector<double>& heights);


private:
    std::vector<int> mNeighbours;
    std::vector<_Triangle> mTriangles;


};

#endif // TRIANGLESURFACE_H
