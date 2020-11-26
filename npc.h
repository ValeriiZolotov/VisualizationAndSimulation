#ifndef NPC_H
#define NPC_H
#include <QOpenGLFunctions_4_1_Core>
#include "mesh.h"
#include "visualobject.h"
#include "trianglesurface.h"
#include "bsplinecurve.h"

class NPC: public VisualObject
{
public:
    NPC();
    NPC(Mesh* mesh, TriangleSurface* tr, BSplineCurve* curve);
    virtual ~NPC()override;
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    void update(vec3& mForward, vec3& mUp, vec3& mUpDir,vec3& mLeft,vec3& target);
    float patrol();
private:
    float mMovementSpeed{15.f};
    vec3 mPosition{0.f,0.f,0.f};
    vec3 mPreviousPosition{0.f,0.f,0.f};
    BSplineCurve* mCurve;
    float x{0.f};
    float mStep{0.005f};
    Mesh* mMesh;
    TriangleSurface* mTerrain;

};

#endif // NPC_H
