#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <utility>
#include <QVector3D>
#include "vertex.h"


class OBJLoader
{
public:
    OBJLoader(){}
    OBJLoader(const QString & filePath);
    //OBJLoader(const std::string &filePath);
    void processLine(QString &line);
    std::vector<Vertex>& getData(){return mData;}
    std::pair<std::vector<Vertex>,std::vector<GLuint>>& getPair()
    {
        mVertexPair = std::make_pair(mData,mIndices);
        return mVertexPair;
    }
private:
    QString mLine;
    QVector3D mVertice;

    /*Temp values*/
    std::vector<vec3> mVertices;
    std::vector<vec3> mNormals;
    std::vector<vec3> mTextures;
    int tempIndex{0};

    std::vector<Vertex> mData;
    std::vector<GLuint> mIndices;
    std::pair<std::vector<Vertex>,std::vector<GLuint>> mVertexPair;
};

#endif // OBJLOADER_H
