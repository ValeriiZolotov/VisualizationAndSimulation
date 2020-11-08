#include "trianglesurface.h"

TriangleSurface::TriangleSurface()
{

}
TriangleSurface::TriangleSurface(const QString fileName)
{
    readFile(fileName);
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
                vrt.set_normal(1.f,0.f,0.f);
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
    glPointSize(10.f);
    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindVertexArray( mVAO );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
