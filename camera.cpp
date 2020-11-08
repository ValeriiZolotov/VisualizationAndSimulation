#include "camera.h"
#include <cmath>
#include <QtMath>



Camera::Camera()
{

}

QMatrix4x4 Camera::getViewMatrix()const
{
    return m_View;
}

QMatrix4x4 Camera::getProjectionMatrix()const
{

    return m_Projection;
}


void Camera::lookAt()
{

    m_position = getCameraPosition();
    m_yaw = getCameraYaw();
    m_pitch = getCameraPitch();
    update();
    m_View.setToIdentity();
    m_View.lookAt(m_position,m_position+m_front,m_up);

}

void Camera::perspective(float aspectRatio)
{
    m_Projection.setToIdentity();
    m_Projection.perspective(m_fovy, aspectRatio, m_near, m_far);
}

void Camera::ortho()
{
    m_View.setToIdentity();
    m_Projection.setToIdentity();
//    switch(ortho_type)
//    {
//    case XY:
//        m_Projection.ortho(-10.f,10.f,-10.f,10.f,10.f,10.f); //xy
//        break;
//    case XZ:
//        m_Projection.ortho(-10.f,10.f,10.f,10.f,10.f,-10.f); //xy
//        m_Projection.rotate(-90,vec3(1.f,0.f,0.f));//xz
//        break;
//    case YZ:
//        m_Projection.ortho(-10.f,10.f,-10.f,10.f,10.f,-10.f); //xy
//        m_Projection.rotate(90,vec3(0.f,1.f,0.f));//yz
//        break;

    //}
}

void Camera::frustum(float left, float right, float bottom, float top, float Cnear, float Cfar)
{
    m_Projection.setToIdentity();
    m_Projection.frustum(left,right, bottom,top,Cnear,Cfar);
}

void Camera::update()
{

    m_front.setX(cosf( qDegreesToRadians(m_yaw))*cosf( qDegreesToRadians(m_pitch)));
    m_front.setY(sinf( qDegreesToRadians(m_pitch)));
    m_front.setZ(sinf( qDegreesToRadians(m_yaw))*cosf (qDegreesToRadians(m_pitch)));

    m_front.normalize();
    m_right = m_right.crossProduct(m_front,m_worldUp);
    m_right.normalize();
    m_up = m_up.crossProduct(m_right,m_front);
    m_up.normalize();

}

void Camera::mouseControl(GLfloat dx,GLfloat dy)
{
    dx*=m_turnSpeed;
    dy*=m_turnSpeed;

    m_yaw += -dx;
    m_pitch += -dy;

    if(m_pitch >= 89.f)
        m_pitch = 89.f;
    if(m_pitch > -89.f)
        m_pitch = -89.f;

    update();
}

void Camera::mouseControl(const QQuaternion &rotation)
{
    m_View.rotate(rotation);
    update();
}

void Camera::wheelControl(GLfloat dx, GLfloat dy)
{
    dx = 0;
    if(m_fovy >= 1.0f && m_fovy <= 60.0f)
        m_fovy -= dy;
    else if(m_fovy <= 1.0f)
        m_fovy = 1.0f;
    else if(m_fovy >= 60.0f)
        m_fovy = 60.0f;


}



bool Camera::frustumCheck(vec3 center, float radiuse)
{


}



void Camera::moveForward()
{
    m_position += m_front * m_movementSpeed;
}

void Camera::moveBackward()
{
    m_position -= m_front * m_movementSpeed;
}

void Camera::moveRight()
{
    m_position += m_right * m_movementSpeed;
}

void Camera::moveLeft()
{
    m_position -= m_right * m_movementSpeed;
}

void Camera::moveUp()
{
    m_position += m_up * m_movementSpeed;
}

void Camera::moveDown()
{
    m_position -= m_up * m_movementSpeed;
}
