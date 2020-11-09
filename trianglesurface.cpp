#include "trianglesurface.h"

TriangleSurface::TriangleSurface()
{

}
TriangleSurface::TriangleSurface(const QString fileName)
{
    readFile(fileName);
    toTriagles();
    setNormals();
}
void TriangleSurface::readFile(const QString fileNavn)
{
    QFile file(fileNavn);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    unsigned long long vertexNumber;
    unsigned long long indexNumber;
    unsigned long long neighboursNumber;

    int lineItr{0};
    Vertex vrt;

    while (!in.atEnd()) {
        QString line = in.readLine();
        ++lineItr;

        if(line.contains("#vertex"))
        {
            QStringList strList;
            strList = line.split(" ");
            vertexNumber = strList.at(1).toULongLong(); // get the number of vertices to reserv memory in mVertices
            mVertices.reserve(vertexNumber);
            qDebug()<<vertexNumber;
        }
        if(line.contains("#indices:"))
        {
            //qDebug()<<line;
            QStringList strList;
            strList = line.split(" ");
            indexNumber = strList.at(1).toULongLong(); // get the number of vertices to reserv memory in mVertices
            mIndices.reserve(indexNumber);
            qDebug()<<indexNumber;;

        }
        if(line.contains("#neighbours:"))
        {
            //qDebug()<<line;
            QStringList strList;
            strList = line.split(" ");
            neighboursNumber = strList.at(1).toULongLong();
            qDebug()<<neighboursNumber;
        }
        else
        {
            if(line.contains("v:"))
            {
                QStringList strList;
                //qDebug()<<line;

                strList = line.split(":"); // spli the line into seperated blocks
                QStringList dataList = strList.at(1).split(",");
                vec3 xyz = vec3(dataList.at(0).toFloat(),
                                dataList.at(1).toFloat(),
                                dataList.at(2).toFloat());
                vrt.set_xyz(xyz);
                vrt.set_normal(0.f,lineItr,lineItr);
                vrt.set_uv(0.f,0.f);
                mVertices.push_back(vrt);
            }
            if(line.contains("i:"))
            {
                QStringList strList;
                strList = line.split(":"); // spli the line into seperated blocks
                QStringList dataList = strList.at(1).split(",");

                for(int i{0};i<3;i++)
                    mIndices.push_back(dataList.at(i).toFloat());
            }
            if(line.contains("n:"))
            {
                QStringList strList;
                strList = line.split(":"); // spli the line into seperated blocks
                QStringList dataList = strList.at(1).split(",");

                for(int i{0};i<3;i++)
                    mNeighbours.push_back(dataList.at(i).toFloat());
            }

        }

    }
    file.close();
    //toXZ();

}
void TriangleSurface::init(GLint matrixUniform)
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
}
void TriangleSurface::draw()
{
    glBindVertexArray( mVAO );

    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glPolygonOffset(10, 10);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glBindVertexArray(0);

    glBindVertexArray( mVAO );
    glPointSize(10.f);
    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

}

void TriangleSurface::toTriagles()
{
    ///sets the vertices into triangles using _Triangle struct
    int id{0};
    for(unsigned long long i{0};i<mIndices.size();i+=3)
    {
        mTriangles.push_back(_Triangle(mVertices.at(mIndices.at(i)),
                                       mVertices.at(mIndices.at(i+1)),
                                       mVertices.at(mIndices.at(i+2)),
                                       vec3(0.f,0.f,0.f),
                                       id++));
    }

}
void TriangleSurface::setNormals()
{
    for(unsigned long long int itr{0}; itr<mTriangles.size();++itr)
    {
        vec3 a,b,c;
        a = mTriangles.at(itr).m_a.getXYZ();
        b = mTriangles.at(itr).m_b.getXYZ();
        c = mTriangles.at(itr).m_c.getXYZ();
        //qDebug()<<"vertices for the triangle# "<<itr<<" "<<a<<b<<c;
        vec3 e1,e2;
        e1 = b-a;
        e2 = c-a;
        //qDebug()<<"edgies for the tr  iangle# "<<itr<<" "<<e1<<e2;
        vec3 normal;
        normal = normal.crossProduct(e1,e2);
        //qDebug()<<"unnormilised normal for the triangle# "<<itr<<" "<<normal;
        normal.normalize();
        //qDebug()<<"normilised normal for the triangle# "<<itr<<" "<<normal;
        mTriangles.at(itr).m_a.set_normal(normal);
        mTriangles.at(itr).m_b.set_normal(normal);
        mTriangles.at(itr).m_c.set_normal(normal);
    }

}

int TriangleSurface::findBall(vec2 playerPosition)
{
    int itr = mTriangles.at(0).m_id;
    bool found{false};
    for(auto triangle:mTriangles)
    {
        vec3 baryc;
        barycentricCoordinates(vec2(mTriangles.at(itr).m_a.getXYZ().x(),
                                    mTriangles.at(itr).m_a.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_b.getXYZ().x(),
                                    mTriangles.at(itr).m_b.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_c.getXYZ().x(),
                                    mTriangles.at(itr).m_c.getXYZ().z()),
                               playerPosition,
                               &baryc);

        mTriangles.at(itr).m_barycCoor = baryc;

        if(baryc.x()>=0 && baryc.y()>=0 && baryc.z()>=0)
        {
            found = true;
            //qDebug()<<"found on triangle# "<<itr;
            return itr;
            break;
        }
        else
        {
             ++itr;
        }
    }
    return -1;
}

void TriangleSurface::barycentricCoordinates(vec2 p1, vec2 p2, vec2 p3,vec2 playerPosition, vec3 *baryc)
{
    vec2 p12 = p2-p1;
    vec2 p13 = p3-p1;

    vec3 n = n.crossProduct(p12,p13);

    //qDebug()<<n1<<n.getZ();
    float areal_123 = n.length();

    //u
    vec2 p = p2 - playerPosition;
    vec2 q = p3 - playerPosition;
    n = n.crossProduct(p,q);
    baryc->setX(n.z() / areal_123);
    //v
    p = p3 - playerPosition;
    q = p1 - playerPosition;
    n = n.crossProduct(p,q);
    baryc->setY(n.z() / areal_123);
    //w
    p = p1 - playerPosition;
    q = p2 - playerPosition;
    n = n.crossProduct(p,q);
    baryc->setZ(n.z() / areal_123);

}

