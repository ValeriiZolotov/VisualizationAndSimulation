#include "vertex.h"

#include <iostream>

Vertex::Vertex() { }

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    mXYZ.setX(x);
    mXYZ.setY(y);
    mXYZ.setZ(z);

    mNormal.setX(r);
    mNormal.setY(g);
    mNormal.setZ(b);
}
Vertex::Vertex(float x, float y, float z, float r, float g, float b,float s, float v)
{
    mXYZ.setX(x);
    mXYZ.setY(y);
    mXYZ.setZ(z);

    mNormal.setX(r);
    mNormal.setY(g);
    mNormal.setZ(b);

    mST.setX(s);
    mST.setY(v);
}

Vertex::Vertex(vec3 a, vec3 b , vec2 c)
{
    mXYZ = a;
    mNormal = b;
    mST = c;
}

Vertex::~Vertex() { }

void Vertex::set_xyz(GLfloat *xyz)
{
    mXYZ.setX(xyz[0]);
    mXYZ.setY(xyz[1]);
    mXYZ.setZ(xyz[2]);
}

void Vertex::set_xyz(GLfloat x, GLfloat y, GLfloat z)
{
    mXYZ.setX(x);
    mXYZ.setY(y);
    mXYZ.setZ(z);
}

void Vertex::set_xyz(vec3 xyz_in)
{
    mXYZ = xyz_in;
}

void Vertex::set_rgb(GLfloat *rgb)
{
    mNormal.setX(rgb[0]);
    mNormal.setY(rgb[1]);
    mNormal.setZ(rgb[2]);
}

void Vertex::set_rgb(GLfloat r, GLfloat g, GLfloat b)
{
    mNormal.setX(r);
    mNormal.setY(g);
    mNormal.setZ(b);
}

void Vertex::set_normal(GLfloat *normal)
{
    mNormal.setX(normal[0]);
    mNormal.setY(normal[1]);
    mNormal.setZ(normal[2]);
}

void Vertex::set_normal(GLfloat x, GLfloat y, GLfloat z)
{
    mNormal.setX(x); mNormal.setY(y); mNormal.setZ(z);
}

void Vertex::set_normal(vec3 normal_in)
{
    mNormal = normal_in;
}

void Vertex::set_st(GLfloat *st)
{
    mST.setX(st[0]);
    mST.setY(st[1]);
}

void Vertex::set_st(GLfloat s, GLfloat t)
{
    mST.setX(s); mST.setY(t);
}

void Vertex::set_st(vec2 st)
{
    mST.setX(st.x()); mST.setY(st.y());
}

void Vertex::set_uv(GLfloat u, GLfloat v)
{
    mST.setX(u);
    mST.setY(v);
}

std::ostream& operator<< (std::ostream& os, Vertex& v) {
    os << std::fixed;
    os << "(" << v.getXYZ().x() << ", " << v.getXYZ().y() << ", " << v.getXYZ().z() << ") ";
    os << "(" << v.getNormals().x() << ", " << v.getNormals().y() << ", " << v.getNormals().z() << ") ";
    os << "(" << v.getST().x() << ", " << v.getST().y() << ") ";
    return os;
}


