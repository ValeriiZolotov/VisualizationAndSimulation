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
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBindVertexArray( mVAO );

    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray( mVAO );
    glPointSize(10.f);
    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

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
//ddffsfs
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
        normal = normal.normalized();
        //qDebug()<<"normilised normal for the triangle# "<<itr<<" "<<normal;
        mTriangles.at(itr).m_a.set_normal(normal);
        mTriangles.at(itr).m_b.set_normal(normal);
        mTriangles.at(itr).m_c.set_normal(normal);
    }

}

void TriangleSurface::scaleTerrainData(std::vector<double>& XCoords,
                                       std::vector<double>& YCoords,
                                       std::vector<double>& heights)
{
        auto [minX,maxX] = std::minmax_element(XCoords.begin(),XCoords.end());
        auto [minY,maxY] = std::minmax_element(YCoords.begin(),YCoords.end());
        auto [minZ,maxZ] = std::minmax_element(heights.begin(),heights.end());

                //qDebug()<<"Min X"<<*minX<<" Max X"<<*maxX;
                //qDebug()<<"Min Y"<<*minY<<" Max Y"<<*maxY;
                //qDebug()<<"Min H"<<*minZ<<" Max H"<<*maxZ;



                for(std::size_t i = 0;i<XCoords.size();i++)
        {
            double x,y;
            scale(XCoords.at(i),*maxX,*minX,50,-50,
                  YCoords.at(i),*maxY,*minY,50,-50
                  ,x,y);
            mCoords.push_back(x);
            mCoords.push_back(y);
        }

        for(std::size_t i = 0;i<heights.size();i++)
        {
            mHeights.push_back(scale(heights.at(i),*maxZ,*minZ,10,-10));
        }
}

void TriangleSurface::readLFile(const QString filNavn)
{
    QFile file(filNavn);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);

    int lineItr{0};
    Vertex vrt;

    std::vector<double> XCoords;// for Delaunay algorithm
    std::vector<double> YCoords;// for Delaunay algorithm
    std::vector<double> heights;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(lineItr!=0)
        {
            if(lineItr%500 == 0)
            {
                QStringList strList;
                strList = line.split("\t");
                XCoords.push_back(strList.at(0).toDouble()); // x
                YCoords.push_back(strList.at(1).toDouble()); // y
                heights.push_back(strList.at(2).toDouble());
            }
        }
        ++lineItr;
    }
    file.close();
    scaleTerrainData(XCoords,YCoords,heights);

}

int TriangleSurface::findBall(vec2 playerPosition)
{
    int itr = 0;//mTriangles.at(0).m_id;
    bool found{false};

    //vec3 baryc;

    //    barycentricCoordinates(QVector2D(0,-4), QVector2D(4,-4),QVector2D(4,-1),
    //                           QVector2D(3.21,-3.082),
    //                           &baryc);

    //    qDebug()<<baryc;

    for(auto triangle:mTriangles)
    {
        vec3 baryc;
        //qDebug()<<playerPosition;
        //        qDebug()<<vec2(mTriangles.at(itr).m_a.getXYZ().x(),
        //                       mTriangles.at(itr).m_a.getXYZ().z())<<
        //                  vec2(mTriangles.at(itr).m_b.getXYZ().x(),
        //                       mTriangles.at(itr).m_b.getXYZ().z())<<
        //                  vec2(mTriangles.at(itr).m_c.getXYZ().x(),
        //                       mTriangles.at(itr).m_c.getXYZ().z());

        barycentricCoordinates(vec2(mTriangles.at(itr).m_a.getXYZ().x(),
                                    mTriangles.at(itr).m_a.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_c.getXYZ().x(),
                                    mTriangles.at(itr).m_c.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_b.getXYZ().x(),
                                    mTriangles.at(itr).m_b.getXYZ().z()),
                               playerPosition,
                               &baryc);
        //qDebug()<<baryc;
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
float TriangleSurface::findObject(vec2 position)
{
    int itr = 0;//mTriangles.at(0).m_id;
    bool found{false};
    float height{0.f};

    for(auto triangle:mTriangles)
    {
        vec3 baryc;

        barycentricCoordinates(vec2(mTriangles.at(itr).m_a.getXYZ().x(),
                                    mTriangles.at(itr).m_a.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_c.getXYZ().x(),
                                    mTriangles.at(itr).m_c.getXYZ().z()),
                               vec2(mTriangles.at(itr).m_b.getXYZ().x(),
                                    mTriangles.at(itr).m_b.getXYZ().z()),
                               position,
                               &baryc);
        mTriangles.at(itr).m_barycCoor = baryc;

        if(baryc.x()>=0 && baryc.y()>=0 && baryc.z()>=0)
        {
            found = true;
            //qDebug()<<"found on triangle# "<<itr;
            height = mTriangles.at(itr).m_a.getXYZ().y()*baryc.x()
                    +mTriangles.at(itr).m_c.getXYZ().y()*baryc.y()
                    +mTriangles.at(itr).m_b.getXYZ().y()*baryc.z();

            return height;
            break;
        }
        else
        {
            ++itr;
        }
    }
    return 0.f;
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

void TriangleSurface::generateSurface(int sizeX,int sizeY)
{
    mVertices.reserve((sizeX+1)*(sizeY+1));
    for (int y = -sizeY/2; y <= sizeY/2; y++) {
        for (int x = -sizeX/2; x <= sizeX/2; x++) {
            mVertices.push_back(Vertex(vec3(x,0.f,y),vec3(0.f,1.f,0.f),vec2(0.f,0.f)));
        }
    }

    int indices[sizeX * sizeY * 6];
    for (int ti = 0, vi = 0, y = 0; y < sizeY; y++, vi++) {
        for (int x = 0; x < sizeX; x++, ti += 6, vi++) {
            indices[ti] = vi;
            indices[ti + 3] =  indices[ti + 2] = vi + 1;
            indices[ti + 4] =  indices[ti + 1] = vi + sizeX + 1;
            indices[ti + 5] = vi + sizeX + 2;
        }
    }
    for(auto i:indices)
        mIndices.push_back(i);
}

void TriangleSurface::setNeighbors()
{
    for(unsigned long long i{0};i<mTriangles.size();i++)
    {
        vec3 v0{mTriangles.at(i).m_a.getXYZ()};
        vec3 v1{mTriangles.at(i).m_b.getXYZ()};
        vec3 v2{mTriangles.at(i).m_c.getXYZ()};

        for(unsigned long long k{0}; k<mTriangles.size();k++)
        {
            vec3 v{mTriangles.at(k).m_a.getXYZ()};
            if(k!=i)
            {
                if (v == v0 || v == v1 || v == v2)
                {
                    mTriangles.at(i).m_neighbours.push_back(k);
                }
            }
        }
    }


}

void TriangleSurface::delaunay(const std::vector<double>& coord)
{

    //triangulation happens here
    delaunator::Delaunator d(coord);

    for(auto i:d.triangles)
    {
        mIndices.push_back(i);
    }
    for(std::size_t i = 0, h{0}; i < d.coords.size(); i+=2 , h++)
    {
        mVertices.push_back(Vertex(vec3(d.coords[i],mHeights.at(h),d.coords[i+1]),vec3(0.f,1.f,0.f),vec2(0.f,0.f)));
    }

    //    for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
    //        printf( "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
    //                d.coords[2 * d.triangles[i]],        //tx0
    //                d.coords[2 * d.triangles[i] + 1],    //ty0
    //                d.coords[2 * d.triangles[i + 1]],    //tx1
    //                d.coords[2 * d.triangles[i + 1] + 1],//ty1
    //                d.coords[2 * d.triangles[i + 2]],    //tx2
    //                d.coords[2 * d.triangles[i + 2] + 1] //ty2
    //                );
    //    }

}

//Scales a float to an interval
void TriangleSurface::scale(double x, double maxX, double minX, double maxXS, double minXS,
                            double y, double maxY, double minY, double maxYS, double minYS,double &xNew,double &yNew)
{
    xNew = ((x-minX)/(maxX-minX))*(maxXS-minXS)+minXS;
    yNew = ((y-minY)/(maxY-minY))*(maxYS-minYS)+minYS;
}
double TriangleSurface::scale(double x, double maxX, double minX, double maxXS, double minXS)
{
    return ((x-minX)/(maxX-minX))*(maxXS-minXS)+minXS;
}
