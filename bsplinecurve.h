#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H
#include <vector>
#include <QVector3D>
#include "visualobject.h"

using Knot = float;
using ControlPoint = QVector3D;

class BSplineCurve:public VisualObject
{
public:
    BSplineCurve(std::vector<ControlPoint>& controlPoints):mControllPoints(controlPoints)
    {

        for(auto i:mControllPoints)
            mVertices.push_back(Vertex(i,vec3(0.f,0.f,1.f),vec2(0.f,0.f)));
    }
    BSplineCurve(std::vector<ControlPoint>& controlPoints, int degree)
        :mControllPoints(controlPoints),mDegree(degree)
    {
        defineKnots();
        evaluateBSpline();
    }

    vec3 deBoor(int my, float x); // used to evaluate a point on the curve
    vec3 deBoor(int k,int degree, int i, double x, std::vector<float>* knots, std::vector<vec3> *ctrlPoints);
    void evaluateBSpline(); // forms Vertices by evaluating the spline along with all control points
    int findKnotInterval(float x);
    void defineKnots(); // defines all knots using the algorithm inside the function
    void init(GLint matrixUniform) override;
    void draw() override;
    void deletePoint(int i){mControllPoints.erase(mControllPoints.begin()+i);}
    std::vector<ControlPoint>& getPoints(){return mControllPoints;}
    std::vector<ControlPoint> mControllPoints;

    ~BSplineCurve() override;

public slots:
    void reevaluetaBSpline(std::vector<ControlPoint>& controlPoints, int degree); // recomputes the values of the spine and reinitialize the OpenGL buffers for the curve
private:
    std::vector<Knot> mKnots;
    int mKnotsNumber{0};
    int mDegree{0};
};

#endif // BSPLINECURVE_H
