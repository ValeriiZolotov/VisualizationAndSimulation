#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"



class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   virtual ~VisualObject();
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   virtual void move(float x, float y, float z);
   inline const std::vector<Vertex> get_mVertices () const{return mVertices;}
   inline const QMatrix4x4 get_mMatrix()const{return mMatrix;}
   inline const std::vector<GLuint> get_Indices()const{return mIndices;}

   vec3 getPosition(){return vec3(mMatrix.column(3).x(),mMatrix.column(3).y(),mMatrix.column(3).z());}
   void setPosition(float x,float y, float z){mMatrix.translate(x,y,z);}

protected:

   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices;

   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mEAB{0};
   GLint mMatrixUniform{0};
   QMatrix4x4 mMatrix;





};
#endif // VISUALOBJECT_H