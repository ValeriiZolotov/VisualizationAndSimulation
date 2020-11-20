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
    BSplineCurve();
    BSplineCurve(std::vector<ControlPoint>& controlPoints, int degree)
        :mControllPoints(controlPoints),mDegree(degree)
    {
        defineKnots();
        evaluateBSpline();
    }

    vec3 deBoor(int my, float x);
    vec3 deBoor(int k,int degree, int i, double x, std::vector<float>* knots, std::vector<vec3> *ctrlPoints);
    void evaluateBSpline();
    int findKnotInterval(float x);
    void defineKnots();
    void init(GLint matrixUniform) override;
    void draw() override;
    ~BSplineCurve() override;

private:
    std::vector<Knot> mKnots;
    std::vector<ControlPoint> mControllPoints;
    int mKnotsNumber{0};
    int mDegree{0};
};

#endif // BSPLINECURVE_H
