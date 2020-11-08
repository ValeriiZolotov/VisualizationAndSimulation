#ifndef GLTYPES_H
#define GLTYPES_H
// We put this test inside this file, instead of having it in each file that needs gl.h
// OpenGL types and declarations on different architectures

//Visual Studio 2017 - for some reason it does not like to include gl/GL.h
#ifdef _WIN64

#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

using GLfloat = float;
using GLdouble = double;
using GLint = int;
using GLuint = unsigned int;
using GLenum = unsigned int;


typedef QVector4D vec4;
typedef QVector3D vec3;
typedef QVector2D vec2;
typedef QVector2D point2D;
typedef QVector3D point3D;
#else

//minGW on windows
#ifdef _WIN32   //_WIN32 is defined if _WIN64 is defined, so must test _WIN64 first
#include <GL/gl.h>
#endif

#endif //#ifdef _WIN64

#ifdef linux
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <opengl/gl.h>
#endif
#endif // GLTYPES_H
