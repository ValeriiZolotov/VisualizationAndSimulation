#include "player.h"

Player::Player()
{

}

Player::Player(Mesh* mesh, TriangleSurface* tr)
{
    mMesh = mesh;
    mVertices = mMesh->getMeshData().first;
    mIndices = mMesh->getMeshData().second;
    mPosition = vec3(mMatrix.column(3));
    mTerrain = tr;
    mBox= new BoundingBox(this);
}
Player::~Player()
{
    delete mBox;
}

void Player::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    mMatrixUniform = matrixUniform;
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
    mBox->init(mMatrixUniform);
}
void Player::draw()
{
    mPosition = vec3(mMatrix.column(3));
    mBox->update(this);

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    mBox->draw();
}

void Player::moveRight()
{
    move(-mMovementSpeed,0,0.f);
}

void Player::moveLeft()
{
    move(mMovementSpeed,0.f,0.f);
}

void Player::moveForward()
{
    move(0.0f,0.f,mMovementSpeed);
}

void Player::moveBackwards()
{
    move(0.0f,0.f,-mMovementSpeed);
}
void Player::update(vec3& mForward, vec3& mUp, vec3& mUpDir,vec3& mLeft)
{
    vec3 target = mPosition*-10.f;

    mForward = target - mPosition;
    mForward.normalize();
    // compute the left vector
    mLeft =vec3::crossProduct( mUpDir,mForward).normalized();  // cross product
    // compute the orthonormal up vector
    mUp = vec3::crossProduct(mForward,mLeft).normalized();

    QMatrix4x4 mRotation;

    mRotation.setRow(0,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(1,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(2,vec4(mLeft.x(),mUp.y(),mForward.z(),0));
    mRotation.setRow(3,vec4(0.f,0.f,0.f,1.f));

}
float Player::getAngle()
{

    //    vec3 target = mPosition-vec3(2.27f,0.f,-5.f);
    //    return acosf(vec3::dotProduct(mPosition,target)/mForward.length()*target.length());
}
