#include "objloader.h"

OBJLoader::OBJLoader(const QString & filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


    QTextStream in(&file);
    while (!in.atEnd()) {
        mLine = in.readLine();
        processLine(mLine);
    }
}

//OBJLoader::OBJLoader(const std::string &filePath)
//{
//    QString newFilePath = QString::fromStdString(filePath);
//    QFile file(newFilePath);

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;


//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        mLine = in.readLine();
//        processLine(mLine);
//    }
//}

void OBJLoader::processLine(QString &line)
{
    if(line.contains("#"))
    {
    }
    else
    {
        QStringList strList;
        strList = line.split(" ");
        foreach(QString str,strList)
        {
            if(str == "v")
            {
                strList.removeAt(strList.indexOf("v"));
                //strList.removeAt(strList.indexOf(""));

                mVertices.push_back(vec3(strList.at(0).toFloat(),
                                             strList.at(1).toFloat(),
                                             strList.at(2).toFloat())
                                    );


            }
            if(str == "vn")
            {
                strList.removeAt(strList.indexOf("vn"));
                //strList.removeAt(strList.indexOf(""));

                mNormals.push_back(vec3(strList.at(0).toFloat(),
                                            strList.at(1).toFloat(),
                                            strList.at(2).toFloat())
                                   );

            }
            if(str == "vt")
            {
                strList.removeAt(strList.indexOf("vt"));
                //strList.removeAt(strList.indexOf(""));
                if(strList.size() == 3)
                {
                    mTextures.push_back(vec3(strList.at(0).toFloat(),
                                                 strList.at(1).toFloat(),
                                                 strList.at(2).toFloat()));
                }
                else
                {
                    mTextures.push_back(vec3(strList.at(0).toFloat(),
                                                 strList.at(1).toFloat(),
                                                 0.f));
                }


            }
            if(str == "g")
            {
                strList.removeAt(strList.indexOf("g"));
                //strList.removeAt(strList.indexOf(""));
            }
            if(str == "f")
            {
                strList.removeAt(strList.indexOf("f"));
                //strList.removeAt(strList.indexOf(""));
                /*
                qDebug()<<"Vertices size: "<<mVertices.size();
                qDebug()<<"Normals size: "<<mNormals.size();
                qDebug()<<"Textures size: "<<mTextures.size();
                */


                //qDebug()<<strList;
                for(int itr{0}; itr<strList.size();itr++)
                {
                    QStringList fList = strList.at(itr).split("/");

                    for(int fitr{0}; fitr<fList.size();fitr++)
                    {
                        //qDebug()<<fList;
                        unsigned long long v = fList.at(fitr).toULongLong()-1;

                        unsigned long long vt = fList.at(++fitr).toULongLong()-1;

                        unsigned long long vn = fList.at(++fitr).toULongLong()-1;

                        //qDebug()<<"v: "<<fList.at(fitr)<<" vn: "<<vn<<" vt: "<<fList.at(++fitr);

                        //qDebug()<<"v: "<<v<<" vn: "<<vn<<" vt: "<<vt;

                        /****v/vn/vt*****/
                        Vertex vrt;
                        vrt.set_xyz(mVertices.at(v));
                        vrt.set_normal(mNormals.at(vn));
                        vrt.set_st(vec2(mTextures.at(vt).x(),mTextures.at(vt).y()));
                        mData.push_back(vrt);
                        mIndices.push_back(tempIndex++);

                    }
                }
            }
        }
    }
}
