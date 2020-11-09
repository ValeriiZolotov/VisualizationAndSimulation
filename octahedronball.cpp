#include <iostream>
#include <sstream>
#include <math.h>
#include "octahedronball.h"

//! \param n - the recursion level (default is 0 which makes the original Octahedron)
//!
//! The number of (duplicated) vertices are calculated based on the parameter n - the recursion level.
//! Initially we have 8 triangles, each with 3 vertices.
//! A subdivision consists of
//! - compute the midpoint of each edge in a triangel
//! - add these midpoint as vertices
//! - make three new edges by connecting new vertices
//! - preserve orientation
//! - move the three new points on the unit ball's surface by normalizing. Remember the vertex vector
//! is also a normal to the ball's surface.
//! The formula for the number of vertices is
//! m_nVertices = 3 * 8 * 4^n
//! where
//! - 3 comes from the number of vertices in a triangle,
//! - 8 comes from the original number of triangles in a regular Octahedron
//! - n is the recursion level (number of repeated subdivisions)
//!

OctahedronBall::OctahedronBall(int n,TriangleSurface* surface) : m_rekursjoner(n), m_indeks(0),m_tr(surface)
{
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
    mMatrix.setToIdentity();
    //mMatrix.rotate(-90,vec3(1.0f,0.f,0.f));
    //mRotation.setToIdentity();
    //mStart_speed = 0.1f;
    //mitr=0;
}

//!//! \brief OctahedronBall::~OctahedronBall() virtual destructor
//!
OctahedronBall::~OctahedronBall()
{

}

// Parametre inn: xyz koordinatene til et triangle v1, v2, v3 ccw
// Bevarer orienteringen av hjørnene
//!
//! \brief OctahedronBall::lagTriangel()
//! \param v1 - position on the unit ball for vertex 1
//! \param v2 - position on the unit ball for vertex 2
//! \param v3 - position on the unit ball for vertex 3
//!
//! lagTriangel() creates vertex data for a triangle's 3 vertices. This is done in the
//! final step of recursion.
//!
void OctahedronBall::lagTriangel(const vec3& v1, const vec3& v2, const vec3& v3)
{
    Vertex v{};

    v.set_xyz(v1.x(), v1.y(), v1.z());		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.set_normal(v1.x(), v1.y(), v1.z());	// rgb
    v.set_st(0.0f, 0.0f);			// kan utelates
    mVertices.push_back(v);
    v.set_xyz(v2.x(), v2.y(), v2.z());
    v.set_normal(v2.x(), v2.y(), v2.z());
    v.set_st(1.0f, 0.0f);
    mVertices.push_back(v);
    v.set_xyz(v3.x(), v3.y(), v3.z());
    v.set_normal(v3.x(), v3.y(), v3.z());
    v.set_st(0.5f, 1.0f);
    mVertices.push_back(v);
}

// Rekursiv subdivisjon av triangel
//!
//! \brief OctahedronBall::subDivide() recursive subdivision of a triangel
//! \param a coordinates for vertex a
//! \param b coordinates for vertex b
//! \param c coordinates for vertex c
//! \param n level of recursion
//!
//! The function tests
//! (if n>0)
//! - three new edge midpoints are computed and normalized,
//! - four new calls to subDivide() is done - one for each new triangle, preserving orientation
//! and with decreased parameter n
//! else
//! - call lagTriangel(a, b, c)
//!
void OctahedronBall::subDivide(const vec3& a, const vec3& b, const vec3& c, int n)
{
    if (n>0) {
        vec3 v1 = a+b; v1.normalize();
        vec3 v2 = a+c; v2.normalize();
        vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        lagTriangel(a, b, c);
    }
}

//!
//! \brief OctahedronBall::oktaederUnitBall() creates 8 unit ball vertices and call subDivide()
//!
//! If the parameter n of the constructor OctahedronBall() is zero, the result will be the
//! original octahedron consisting of 8 triangles with duplicated vertices.
//!
void OctahedronBall::oktaederUnitBall()
{
    vec3 v0{0, 0, 1};
    vec3 v1{1, 0, 0};
    vec3 v2{0, 1, 0};
    vec3 v3{-1, 0, 0};
    vec3 v4{0, -1, 0};
    vec3 v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

//!
//! \brief OctahedronBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//! for using later use of glDrawArrays()
//!
void OctahedronBall::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();
    //mMatrix.scale(0.5f,0.5f,0.5f);
    mMatrix.translate(1.5f,4.f,-1.5f);
    //mPosition.translate(-2.5f,2.5f,0.5f);
    //mPosition.translate(mPath.at(mitr).getXYZ().x(),mPath.at(mitr).getXYZ().y(),mPath.at(mitr).getXYZ().z());
    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_TRUE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

//!
//! \brief OctahedronBall::draw() draws a ball using glDrawArrays()
//! \param positionAttribute    vertex shader variable for position
//! \param normalAttribute      vertex shader variable for normal or color
//! \param textureAttribute     vertex shader variable for texture coordinates (optional)
//!
//! draw()
//! - glBindBuffer()
//! - glVertexAttribPointer()
//! - glBindTexture()
//! - glVertexAttribPointer()
//! - glDrawArrays() with GL_TRIANGLES
//!
void OctahedronBall::draw()
{
    //    mStart_speed-=0.001f;
    //    if(mStart_speed < 0 ){mStart_speed =0;}
    //    //move(mStart_speed);
    //    mitr=mitr+i;
    //    if(mitr>90 || mitr<=0)
    //        i=i*-1;
    //    move(0.000001f,0.f,0.0);
    move(.2f);
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}

void OctahedronBall::move(float x, float y, float z)
{

    /*
    mVelocity = Q vec3 (1.0,0.0,0.0);

    Q vec3 ds;
    ds+= mVelocity * dt;
*/
    //mPosition.translate(ds.x(),ds.y(),ds.z());
    mMatrix.setToIdentity();
    //mMatrix.translate(mPath.at(mitr).getXYZ().x(),mPath.at(mitr).getXYZ().y(),mPath.at(mitr).getXYZ().z());
    //qDebug()<<mPath.at(mitr).getXYZ().getX()<<mPath.at(mitr).getXYZ().getY()<<mPath.at(mitr).getXYZ().getZ();

    /*
    Q vec3 normal = Q vec3(0.f,0.0f,1.0f);

    Q vec3 rotation;
    rotation = rotation.crossProduct(normal,mVelocity);
    rotation.normalize();

    dt = ds.x()/2.f;
    //qDebug()<<ds.x();
    float deg = 180 * (dt/M_PI);

    mRotation.rotate(deg,rotation);

    mMatrix = mPosition*mRotation;
    */


}

void OctahedronBall::move(float dt)
{
    //get the position of the ball
    vec2 ballPosition = vec2(mMatrix.column(3).x(),mMatrix.column(3).z());
    //find triangel where the ball is placed
    int index = m_tr->findBall(ballPosition);
    /*velocity vector*/
    vec3 dv = vec3(0.f,0.f,0.f);
    vec3 v0 = vec3(0.f,0.f,0.f);
    /*position vector*/
    vec3 ds = vec3(0.f,0.f,0.f);

    if(index==-1)
    {
        qDebug()<<"could not find ball"<<index;
    }
    else{
        qDebug()<<"found on triangle "<<index;
        vec3 normal = m_tr->getTriangles().at(index).m_a.getNormals();
        /*acceleration vector*/
        vec3 a = 9.8f*vec3(normal.x()*normal.y(),normal.y()*normal.y()-1,normal.z()*normal.y());
        a = a.normalized();
        ds += dv*dt+(a*dt*dt)/2.f;
        dv += v0+a*dt; //velocity is a change of position
        mMatrix.translate(ds);
    }


}
