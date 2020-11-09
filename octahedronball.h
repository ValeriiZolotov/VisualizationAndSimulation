#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H
#include "visualobject.h"
#include "trianglesurface.h"


class OctahedronBall : public VisualObject
{
private:

    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    QMatrix4x4 mRotation;
    vec3 mVelocity;
    QMatrix4x4 mPosition;
    float mStart_speed;

    void lagTriangel(const vec3& v1, const vec3& v2, const vec3& v3);
    void subDivide(const vec3& a, const vec3& b, const vec3& c, int n);
    void oktaederUnitBall();
    int mitr;
    int i{1};
    TriangleSurface* m_tr;
    vec3 mOdlNormal{0.f,1.f,0.f};
    int mOldIndex{0};

public:
    OctahedronBall(int n = 0,TriangleSurface* surface = nullptr);
    ~OctahedronBall() override;
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    void move(float x, float y, float z) override;
    void move(float dt);

};

#endif // OCTAHEDRONBALL_H
