#include "bsplinecurve.h"



vec3 BSplineCurve:: deBoor(int my, float x)
{
    // Parametre inn:
    // x - en parameterverdi på skjøtvektor
    // my - et tall slik at t[my] <= x < t[my+1]
    // Returverdi: et punkt på splinekurven
    // b,n,d,t er objektvariable i klassen BSplineCurve
    std::vector<vec3> a;
    a.reserve(mDegree+1);

    for (int j=0; j<=mDegree; j++)
        a[mDegree-j] = mControllPoints[my-j];
    for (int k=mDegree; k>0; k--)
    {
        int j = my-k;
        for (int i=0; i<k; i++)
        {
            j++;
            float w = (x-mKnots[j])/(mKnots[j+k]-mKnots[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a[0];
}

void BSplineCurve::defineKnots()
{
    //size of the knot vector
    mKnotsNumber = mControllPoints.size()+mDegree+1;
    qDebug()<<"Knots number# "<<mKnotsNumber;
    mKnots.reserve(mKnotsNumber);

    float offset = 1.f/((mKnotsNumber-((mDegree+1.f)*2.f))+1.f);
    float v{0.f};

    for(auto i{0};i<mDegree+1;i++)
        mKnots.insert(mKnots.begin()+i,0.f);

    for(auto j{mDegree+1};j<mKnotsNumber-(mDegree+1);j++)
    {
        mKnots.insert(mKnots.begin()+j,v+=offset);
    }

    for(int k{mKnotsNumber-(mDegree+1)};k<mKnotsNumber;k++)
    {
        mKnots.insert(mKnots.begin()+k,1.f);
    }
}
int BSplineCurve::findKnotInterval(float x)
{
    int my = mControllPoints.size()-1;
    while(x<mKnots.at(my)&&my>mDegree)
    {
        my--;
    }
    return my;
}
void BSplineCurve::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
void BSplineCurve::evaluateBSpline()
{
    //grad: 4;
    //skjøter: { 0, 0, 0, 0,0, 0.2, 0.4,0.6, 0.8,1,1,1,1,1};
    float x{0};
    float step{0.05f};
    vec3 vertex{};

    for(int i{0};i<(1/step);i++)
    {
        x += 0.05f;
        vertex = deBoor(findKnotInterval(x),x);
        //qDebug()<<vertex;
        mVertices.push_back(Vertex(vertex,vec3(1.f,0.f,0.f),vec2(0.f,0.f)));

    }
}

vec3 BSplineCurve::deBoor(int k,int degree, int i, double x, std::vector<float>* knots, std::vector<vec3> *ctrlPoints)
{
    if( k == 0)
        return ctrlPoints->at(i);
    else
    {
        double alpha = (x-knots->at(i))/(knots->at(i+degree+1-k)-knots->at(i));
        return (deBoor(k-1,degree, i-1, x, knots, ctrlPoints)*(1-alpha )+deBoor(k-1,degree, i, x, knots, ctrlPoints)*alpha );
    }
}

void BSplineCurve::draw()
{   
    glBindVertexArray( mVAO );
    glPointSize(10.f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}

BSplineCurve::~BSplineCurve()
{

}

void BSplineCurve::reevaluetaBSpline(std::vector<ControlPoint>& controlPoints, int degree)
{
    mVertices.clear();
    mKnots.clear();
    mKnotsNumber = 0;
    mControllPoints.clear();
    mControllPoints= controlPoints;
    defineKnots();
    evaluateBSpline();

    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
    init(mMatrixUniform);
}
