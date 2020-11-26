#include "boundingbox.h"
#include "math.h"

BoundingBox::BoundingBox(VisualObject* mesh)
{

    mVertices.push_back(Vertex(-0.5, -0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(0.5, -0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, 0.5, 0.5, 0, 0, 1));

    mVertices.push_back(Vertex(-0.5, -0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(0.5, 0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, 0.5, 0, 0, 1 ));

    mVertices.push_back(Vertex(-0.5, -0.5, -0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, -0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, 0.5, 0, 0, 1));

    mVertices.push_back(Vertex(-0.5, -0.5, -0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, -0.5, 0, 0, 1));

    mVertices.push_back(Vertex(0.5, 0.5, -0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, -0.5, 0, 1, 0));
    mVertices.push_back(Vertex(-0.5, -0.5, -0.5, 0, 0, 1));

    mVertices.push_back(Vertex(0.5, 0.5, -0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, -0.5, -0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, -0.5, -0.5, 0, 0, 1 ));

    mVertices.push_back(Vertex(0.5, 0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(0.5, 0.5, -0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, -0.5, -0.5, 0, 0, 1));

    mVertices.push_back(Vertex(0.5, -0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(0.5, -0.5, -0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, 0.5, 0.5, 0, 0, 1));

    mVertices.push_back(Vertex(-0.5, -0.5, 0.5, 1, 0, 0 ));
    mVertices.push_back(Vertex(-0.5, -0.5, -0.5, 0, 1, 0 ));
    mVertices.push_back(Vertex(0.5, -0.5, -0.5, 0, 0, 1 ));

    mVertices.push_back(Vertex(0.5, -0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, -0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, -0.5, -0.5, 0, 0, 1));

    mVertices.push_back(Vertex(-0.5, 0.5, 0.5, 1, 0, 0 ));
    mVertices.push_back(Vertex(-0.5, 0.5, -0.5, 0, 1, 0 ));
    mVertices.push_back(Vertex(0.5, 0.5, -0.5, 0, 0, 1 ));

    mVertices.push_back(Vertex(0.5, 0.5, 0.5, 1, 0, 0));
    mVertices.push_back(Vertex(-0.5, 0.5, 0.5, 0, 1, 0));
    mVertices.push_back(Vertex(0.5, 0.5, -0.5, 0, 0, 1));
    setBox(mesh);


}

void BoundingBox::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    mMatrixUniform = matrixUniform;

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, GLint(mVertices.size()*sizeof( Vertex )), mVertices.data(), GL_STATIC_DRAW );

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

void BoundingBox::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_LINE_LOOP, 0, GLsizei( mVertices.size()));
}


void BoundingBox::update(VisualObject* mesh)
{

    QMatrix4x4 tempM;
    tempM.setToIdentity();
    tempM.translate(mCenter.x(),mCenter.y(),mCenter.z());
    tempM.scale(mSize.x(),mSize.y(),mSize.z());

    mMatrix = mesh->get_mMatrix()*tempM;
}

bool BoundingBox::intersect(BoundingBox *other)
{
    vec3 center1 = this->getPosition();
    vec3 center2 = other->getPosition();
    vec3 distance = center1-center2;

    float length = distance.length();
    float r = 0.9f;
    //qDebug()<<length;
    if(length<=r)
    {
        //qDebug()<<"I, "<< this->getName()<<"got collision with "<< other->getName();
        return true;

    }
    return false;

}


void BoundingBox::setBox(VisualObject* mesh)
{
    std::vector<Vertex> mTempVertices = mesh->get_mVertices();
    min_x = max_x = mTempVertices.at(0).getXYZ().x();
    min_y = max_y = mTempVertices.at(0).getXYZ().y();
    min_z = max_z =mTempVertices.at(0).getXYZ().z();

    for (unsigned int i = 0; i < mTempVertices.size(); i++) {
        if (mTempVertices.at(i).getXYZ().x() < min_x) min_x = mTempVertices.at(i).getXYZ().x();
        if (mTempVertices.at(i).getXYZ().x() > max_x) max_x = mTempVertices.at(i).getXYZ().x();
        if (mTempVertices.at(i).getXYZ().y() < min_y) min_y = mTempVertices.at(i).getXYZ().y();
        if (mTempVertices.at(i).getXYZ().y() > max_y) max_y = mTempVertices.at(i).getXYZ().y();
        if (mTempVertices.at(i).getXYZ().z() < min_z) min_z = mTempVertices.at(i).getXYZ().z();
        if (mTempVertices.at(i).getXYZ().z() > max_z) max_z = mTempVertices.at(i).getXYZ().z();
    }
    vec3 min =  vec3(min_x,min_y,min_z);
    mMin.setX(min.x()); mMin.setY(min.y());mMin.setZ(min.z());
    vec3 max =  vec3(max_x, max_y,max_z);
    mMax.setX(max.x());mMax.setY(max.y());mMax.setZ(max.z());
    vec3 size = vec3(max_x-min_x, max_y-min_y, max_z-min_z);
    mSize.setX(size.x()); mSize.setX(size.y());mSize.setX(size.z());
    vec3 center = vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
    mCenter.setX(center.x());    mCenter.setY(center.y());    mCenter.setZ(center.z());

    update(mesh);

}
