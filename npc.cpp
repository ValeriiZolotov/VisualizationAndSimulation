#include "npc.h"

NPC::NPC()
{

}

NPC::NPC(Mesh *mesh, TriangleSurface *tr, BSplineCurve* curve)
{
    mVertices = mesh->getMeshData().first;
    mIndices = mesh->getMeshData().second;
    mPosition = vec3(mMatrix.column(3));
    mCurve = curve;
    mMesh = mesh;
    mTerrain = tr;
}

NPC::~NPC()
{

}

void NPC::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    mMatrixUniform = matrixUniform;
    //mMatrix.scale(2.f);
    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    mMesh->getBox()->init(matrixUniform);


}

void NPC::draw()
{
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void NPC::update(vec3 &mForward, vec3 &mUp, vec3 &mUpDir, vec3 &mLeft,vec3& target)
{
    mForward = target - mPosition;
    mForward.normalize();
    // compute the left vector
    mLeft =vec3::crossProduct(mUpDir,mForward).normalized();  // cross product
    // compute the orthonormal up vector
    mUp = vec3::crossProduct(mForward,mLeft).normalized();

    QMatrix4x4 mRotation;

    mRotation.setRow(0,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(1,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(2,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(3,vec4(0.f,0.f,0.f,1.f));
}
float NPC::patrol()
{

    x+=mStep;

    if(x>1|| x<=0)
    {
        mStep*=-1.f;
    }
    vec3 position = mCurve->deBoor(mCurve->findKnotInterval(x),x);
    //float height = mTerrain->findObject(vec2(position.x(),position.z()));

    mMatrix.setToIdentity();
    mMatrix.translate(position);
    mMatrix.scale(vec3(0.004f,0.004f,0.004f));

    return x;

}
