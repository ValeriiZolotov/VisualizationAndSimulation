#include "mesh.h"

Mesh::Mesh(OBJLoader *loader, TriangleSurface* tr)
{
    mVertices = loader->getPair().first;
    mIndices = loader->getPair().second;
    mBBox = new BoundingBox(this);
    mTerrain = tr;
}

Mesh::Mesh(QString filename)
{
    OBJLoader loader(filename);
    mVertices = loader.getPair().first;
    mIndices = loader.getPair().second;
    mBBox = new BoundingBox(this);
}

Mesh::~Mesh()
{
    delete mBBox;
}

void Mesh::init(GLint matrixUniform)
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
    mBBox->init(matrixUniform);
}

void Mesh::draw()
{
    mBBox->draw();
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    mBBox->update(this);
    //mPosition = vec3(mMatrix.column(3));
}
float Mesh::setHeight()
{
    return mTerrain->findObject(vec2(mPosition.x(),mPosition.z()));

}
