#include "light.h"


void Light::init(GLint i)
{
    mMatrixUniform = i;
    mMatrix.scale(0.1f,0.1f,0.1f);
    mMatrix.translate(0.f,-35.f,0.f);

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
}

void Light::draw()
{
    mMatrix.translate(cosf(m_rotate)/2.f,sinf(m_rotate)/2.f,0);     //just to move the light each frame
    m_rotate += 0.01f;
    m_LightPosition = getPosition();

    glBindVertexArray( mVAO );

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

Light::~Light()
{

}

void Light::useLight(GLint ambientColourUniform, GLint ambientIntensityUnifor, GLint diffuseIntensityUniform, GLint lightPositionUniform)
{
    glUniform3f(ambientColourUniform,m_LightColour.x(),m_LightColour.y(),m_LightColour.z());
    glUniform1f(ambientIntensityUnifor, m_AmbientIntensity);
    glUniform3f(lightPositionUniform,m_LightPosition.x(),m_LightPosition.y(),m_LightPosition.z());
    glUniform1f(diffuseIntensityUniform,m_DiffuseIntesity);
}
