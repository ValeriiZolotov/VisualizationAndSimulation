#include "visualobject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
}
VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

void VisualObject::move(float x, float y, float z)
{
    mMatrix.translate(x,y,z);
}
