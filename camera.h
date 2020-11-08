#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
#include "gltypes.h"

class Camera
{
private:
    QMatrix4x4 m_Projection;
    QMatrix4x4 m_View;
    vec3 m_PlayerPosition{0.f,0.f,0.f};
    vec3 m_position; //eye
    vec3 m_front;
    vec3 m_up;
    vec3 m_worldUp;
    vec3 m_right{1.0f,0.f,0.f};
    GLfloat m_yaw{0.f};
    GLfloat m_pitch{0.f};
    GLfloat m_roll{0.f};
    GLfloat m_fovy{60.f};
    GLfloat m_movementSpeed{0.05f};
    GLfloat m_turnSpeed{0.05f};
    GLfloat m_far{100.f};
    GLfloat m_near{0.1f};
    QQuaternion mRotation;


    void update();


public:
    Camera();
    Camera(vec3 starting_position,vec3 up,GLfloat st_yaw,GLfloat st_pitch, GLfloat st_roll)
        :m_position(starting_position),m_worldUp(up),m_yaw(st_yaw),m_pitch(st_pitch),m_roll(st_roll),
          m_front(vec3(0.f,0.f,-1.f)),m_movementSpeed(0.05f),m_turnSpeed(0.05f),m_fovy(60.f),m_right(vec3(1.0f,0.f,0.f)){update();}
    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;
    void lookAt();
    void perspective(float aspectRatio);
    void ortho();
    void frustum(float left, float right, float bottom, float top, float Cnear, float Cfar);
    void handelCamera();
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void mouseControl(GLfloat dx,GLfloat dy);
    void mouseControl(const QQuaternion &rotation);
    void wheelControl(GLfloat dx, GLfloat dy);
    vec3 getCameraPosition()const{return m_position;}
    void setCameraPosition(const vec3 &position){m_position = position;}
    void setCameraYaw(const GLfloat &yaw){m_yaw = yaw;}
    void setCameraPitch(const GLfloat &pitch){m_pitch = pitch;}
    void setCameraRoll(const GLfloat &roll){m_roll = roll;}
    void setCameraFovy(const GLfloat fovy){m_fovy = fovy;}
    void setCameraFar(const GLfloat cfar){m_far = cfar;}
    void setCameraNear(const GLfloat cnear){m_near = cnear;}
    GLfloat getCameraYaw()const{return m_yaw;}
    GLfloat getCameraPitch()const{return m_pitch;}
    GLfloat getCameraZoom()const{return m_fovy;}
    //QMatrix4x4 getpMatrix(){return pMatrix;}
    vec3 getCameraRight(){return m_right;}
    vec3 getCameraUP(){return m_up;}

    bool frustumCheck(vec3 center, float radiuse);

    float getData(QMatrix4x4* m, int row, int col)
    {
        return m->column(col)[row];
    }
    void setFrustumGeometry();
};

#endif // CAMERA_H
