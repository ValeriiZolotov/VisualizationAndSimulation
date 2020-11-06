#ifndef VERTEX_H
#define VERTEX_H
#include <iostream>
#include <fstream>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include "gltypes.h"

typedef QVector4D vec4;
typedef QVector3D vec3;
typedef QVector2D vec2;
typedef QVector2D point2D;
typedef QVector3D point3D;

class Vertex {
public:
    Vertex();
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(float x, float y, float z, float r, float g, float b,float s, float v);
    Vertex(vec3 a, vec3 b, vec2 c);
    ~Vertex();

    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    //friend std::ostream& operator<< (std::ostream&, Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);

    void set_xyz(GLfloat *xyz);
    void set_xyz(GLfloat x, GLfloat y, GLfloat z);
    void set_xyz(vec3 xyz_in);
    void set_rgb(GLfloat *rgb);
    void set_rgb(GLfloat r, GLfloat g, GLfloat b);
    void set_normal(GLfloat *normal);
    void set_normal(GLfloat x, GLfloat y, GLfloat z);
    void set_normal(vec3 normal_in);
    void set_st(GLfloat *st);
    void set_st(GLfloat s, GLfloat t);
    void set_st(vec2 st);
    void set_uv(GLfloat u, GLfloat v);
    vec3 getXYZ(){return mXYZ;}
    vec3 getNormals(){return mNormal;}
    vec2 getST(){return mST;}

private:
    vec3 mXYZ;
    vec3 mNormal;
    vec2 mST;
};

#endif // VERTEX_H
