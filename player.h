#ifndef PLAYER_H
#define PLAYER_H
#include <QOpenGLFunctions_4_1_Core>
#include "mesh.h"
#include "visualobject.h"
#include "trianglesurface.h"
class Player : public VisualObject
{
public:
    Player();
    Player(Mesh* mesh, TriangleSurface* tr);
    virtual ~Player() override;
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    void moveRight();
    void moveLeft();
    void moveForward();
    void moveBackwards();
    void update(vec3& mForward, vec3& mUp, vec3& mUpDir,vec3& mLeft);
    float getAngle();
    void setHeight(float height)
    {
        mMatrix.setToIdentity();
        //rotate(180.f,vec3(0.f,1.f,0.f));
        mMatrix.translate(mPosition.x(),0.f,mPosition.z());
    }
    BoundingBox* getBox(){return mBox;}
private:
    float mMovementSpeed{0.5f};
    vec3 mPosition{0.f,0.f,0.f};
    TriangleSurface* mTerrain;
    BoundingBox* mBox;
    Mesh* mMesh;

};

#endif // PLAYER_H
