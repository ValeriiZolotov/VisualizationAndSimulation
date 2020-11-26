#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include "octahedronball.h"
#include "shader.h"
#include "mainwindow.h"
#include "grid.h"
#include "trianglesurface.h"
#include "bsplinecurve.h"
#include "objloader.h"
#include "mesh.h"
#include "player.h"
#include "npc.h"
#include "triangle.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    /*Camera settings*/
    //0.811886f, 10.2819f, 4.64979f
//0.0f, 10.2819f, 20.64979f
    mCamera = new Camera(vec3(0.0f, 15.2819f, 30.64979f),vec3(0.f,1.f,0.f),90.f,-141.f,0.f);
    mFSM = new FSM();
    light = new Light();

}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

/// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::setUpShader()
{
    mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mPMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pmatrix" );
    mVMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vmatrix" );
}
void RenderWindow::setUpPhongShader()
{
    qDebug()<<"Texture shader id "<<mPhShaderProgram->getProgram();
    mPhMatrixUniform = glGetUniformLocation( mPhShaderProgram->getProgram(), "matrix" );
    mPhPMatrixUniform = glGetUniformLocation( mPhShaderProgram->getProgram(), "vmatrix" );
    mPhVMatrixUniform = glGetUniformLocation( mPhShaderProgram->getProgram(), "pmatrix" );
    mAmbientColourUniform =glGetUniformLocation(mPhShaderProgram->getProgram(),"lightColour");
    mAmbientIntensityUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"ambientIntesity");
    mDiffuseIntensityUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"diffuseIntensity");
    mLightPositionUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"lightPosition");
    mCameraPositionUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"cameraPosition");
    mSpecularIntensityUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"specularIntensity");
    mSpecularExponentUniform = glGetUniformLocation(mPhShaderProgram->getProgram(),"specularExponent");
    mObjectColor = glGetUniformLocation(mPhShaderProgram->getProgram(),"objectColor");
}

void RenderWindow::setPointsHeight(std::vector<vec3>& points)
{
    mControllPoints.clear();
    auto terrain = static_cast<TriangleSurface*>(mObjectPool.at(2));
    for(std::size_t i{0};i<points.size();i++)
    {
        vec3 point = points.at(i);
        float h = terrain->findObject(vec2(point.x(),point.z()));
        mControllPoints.push_back(vec3(point.x(),h,point.z()));
    }
}

void RenderWindow::init()
{
    // connect sigmals andl slots for FSM
    connect(mFSM, SIGNAL(sendGState(game_state)),mFSM,SLOT(setGState(game_state)));
    connect(mFSM, SIGNAL(sendNPCState(npc_state)),mFSM,SLOT(setNPCState(npc_state)));
    connect(mFSM, SIGNAL(sendFSMMEssage(QString)),mFSM,SLOT(printFSMMessage(QString)));
    connect(mFSM, SIGNAL(itemTaken(int)),mFSM,SLOT(updateControllPoints(int)));


    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram = new Shader("../VisualizationAndSimulation/plainvertex.vert", "../VisualizationAndSimulation/plainfragment.frag");
    mPhShaderProgram = new Shader("../VisualizationAndSimulation/phongcolorvertex.vert", "../VisualizationAndSimulation/phnogcolorfragment.frag");

    setUpShader();
    setUpPhongShader();

    //********************** Making the object to be drawn **********************
    //0
    VisualObject* temp = new Grid(20,1);
    temp->init(mMatrixUniform);
    mObjectPool.emplace_back(temp);
    //1
    temp = new TriangleSurface("../VisualizationAndSimulation/Assets/datasett/two_triangles.txt");
    temp->init(mMatrixUniform);
    temp->setName("two_triangles");
    mObjectPool.push_back(temp);

    //2    /*terrain delaunay*/
    temp = new TriangleSurface();
    static_cast<TriangleSurface*>(temp)->readLFile("../VisualizationAndSimulation/Assets/datasett/test_las.txt");
    static_cast<TriangleSurface*>(temp)->delaunay(static_cast<TriangleSurface*>(temp)->mCoords);
    static_cast<TriangleSurface*>(temp)->toTriagles();
    static_cast<TriangleSurface*>(temp)->setNormals();
    temp->setName("terrain");
    temp->init(mPhMatrixUniform);
    mObjectPool.push_back(temp);

    //3
    temp = new OctahedronBall(4,static_cast<TriangleSurface*>(mObjectPool.at(2)));
    temp->init(mMatrixUniform);
    temp->setName("Ball");
    mObjectPool.push_back(temp);

    //control points for quadratic BSpline
    std::vector<vec3> tempPoints{vec3(-9.6f,0,-2.f), vec3(-4.f,0.f,4.f), vec3(2.27f,0.f,-5.f),
                vec3(8.f,0.f,4.f), vec3(12.5f,0.f,-2.f), vec3(-2.9f,0.f,-6.5f), vec3(-6.8f,0.f,-3.66f)};

    setPointsHeight(tempPoints);

    //4
    temp = new BSplineCurve(mControllPoints,2);
    temp->init(mMatrixUniform);
    mObjectPool.push_back(temp);

    //5    //to draw t control polygon with control points
    temp = new BSplineCurve(mControllPoints);
    temp->init(mMatrixUniform);
    mObjectPool.push_back(temp);

    OBJLoader* itemLoader = new OBJLoader("../VisualizationAndSimulation/Assets/Mesh/cameraMesh.obj");
    // initializing items mesh;
    for(std::size_t i=1; i<mControllPoints.size()-1;i++)
    {
        temp = new Mesh(itemLoader,static_cast<TriangleSurface*>(mObjectPool.at(2)));
        temp->init(mMatrixUniform);
        temp->setPosition(mControllPoints.at(i).x(),mControllPoints.at(i).y(),mControllPoints.at(i).z()); // move to the position of the item #1
        mObjectPool.push_back(temp);
    }
    //11
    OBJLoader* NPCLoader = new OBJLoader("../VisualizationAndSimulation/Assets/Mesh/skeleton.obj");
    Mesh* NPCMesh = new Mesh(NPCLoader,static_cast<TriangleSurface*>(mObjectPool.at(2)));
    temp = new NPC(NPCMesh,static_cast<TriangleSurface*>(mObjectPool.at(2)),static_cast<BSplineCurve*>(mObjectPool.at(4)));
    temp->init(mMatrixUniform);
    temp->scale(vec3(0.004f,0.004f,0.004f));
    mObjectPool.push_back(temp);


    mFSM->assignNPC(static_cast<NPC*>(mObjectPool.at(11)));
    mFSM->assignCurve(static_cast<BSplineCurve*>(mObjectPool.at(4)));
    emit mFSM->sendGState(GAME_ACTIVE);
    emit mFSM->sendNPCState(PATROL);

    //12
    OBJLoader* playerLoader = new OBJLoader("../VisualizationAndSimulation/Assets/Mesh/player.obj");
    Mesh* playerMesh = new Mesh(playerLoader,static_cast<TriangleSurface*>(mObjectPool.at(2)));
    temp = new Player(playerMesh,static_cast<TriangleSurface*>(mObjectPool.at(2)));
    temp->init(mMatrixUniform);
    temp->setPosition(0.f,0.f,15.f); // move to the starting point of the path
    mObjectPool.push_back(temp);
    //13
    temp = new Triangle();
    temp->init(mMatrixUniform);
    temp->setName("triangel");
    mObjectPool.push_back(temp);

    //sets triangle for the ball simulations
    static_cast<OctahedronBall*>(mObjectPool.at(3))->setTriangle(static_cast<Triangle*>(mObjectPool.at(13)));

    //enable the matrixUniform
    // NB: enable in shader and in render() function also to use matrix


    glBindVertexArray( 0 );
    setBoxVector();

}

///Called each frame - doing the rendering
void RenderWindow::changeSpline()
{
    std::vector<vec3> tempPoints{vec3(-9.6f,0,-2.f), vec3(-4.f,0.f,4.f), vec3(2.27f,0.f,-5.f),
                vec3(8.f,0.f,4.f)};
    setPointsHeight(tempPoints);
    static_cast<BSplineCurve*>(mObjectPool.at(4))->reevaluetaBSpline(mControllPoints,2);
}

void RenderWindow::render()
{


    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));



    /*Camera stuffs*/

    mCamera->perspective(width()/height());
    mCamera->lookAt(static_cast<Player*>(mObjectPool.at(10))->getPosition());

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    /*Render loop*/
    for(auto object: mObjectPool)
    {
        if(object->visiable)
        {
            //if(object->getName()=="terrain" )
            {
                glUseProgram(mShaderProgram->getProgram());
                //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
                //must be here to update each frame - if static object, it could be set only once
                glUniformMatrix4fv( mPMatrixUniform, 1, GL_FALSE, mCamera->getProjectionMatrix().constData());
                glUniformMatrix4fv( mVMatrixUniform, 1, GL_FALSE, mCamera->getViewMatrix().constData());
                glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, object->get_mMatrix().constData());
                object->draw();
            }
        }

        /*use color shader*/
        //what shader to use

        //        if(object->visiable&&object->getName()=="terrain")
        //        {

        //            /*Use phong shader*/
        //            glUseProgram(mPhShaderProgram->getProgram());
        //            glUniform4f(mObjectColor,0.f,0.f,1.f,0.f);
        //            glUniform3f(mAmbientColourUniform,1.f,1.f,1.f);
        //            glUniform1f(mAmbientIntensityUniform, 1.f);
        //            glUniform3f(mLightPositionUniform,0.f,15.f,0.f);
        //            glUniform1f(mDiffuseIntensityUniform,1.f);
        //            glUniform3f(mCameraPositionUniform, mCamera->getCameraPosition().x(),mCamera->getCameraPosition().y(),mCamera->getCameraPosition().z());
        //            glUniform1f(mSpecularIntensityUniform,0.2f);
        //            glUniform1f(mSpecularExponentUniform,32.f);

        //            glUniformMatrix4fv( mPhPMatrixUniform, 1, GL_FALSE, mCamera->getProjectionMatrix().constData());
        //            glUniformMatrix4fv( mPhVMatrixUniform, 1, GL_FALSE, mCamera->getViewMatrix().constData());
        //            glUniformMatrix4fv( mPhMatrixUniform, 1, GL_FALSE, object->get_mMatrix().constData());
        //            object->draw();
        //        }

    }

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);


}
void RenderWindow::setBoxVector()
{
    // puting object for collision check
    BoundingBox* b1 = static_cast<Player*>(mObjectPool.at(12))->getBox();
    BoundingBox* b2 = static_cast<Mesh*>(mObjectPool.at(6))->getBox();
    BoundingBox* b3 = static_cast<Mesh*>(mObjectPool.at(7))->getBox();
    BoundingBox* b4 = static_cast<Mesh*>(mObjectPool.at(8))->getBox();
    BoundingBox* b5 = static_cast<Mesh*>(mObjectPool.at(9))->getBox();
    BoundingBox* b6 = static_cast<Mesh*>(mObjectPool.at(10))->getBox();

    mBBoxes.push_back(b1);
    mBBoxes.push_back(b2);
    mBBoxes.push_back(b3);
    mBBoxes.push_back(b4);
    mBBoxes.push_back(b5);
    mBBoxes.push_back(b6);
}
int RenderWindow::checkCollision()
{
    int index{-1};
    for(std::size_t i{1}; i<mBBoxes.size();i++)
    {
        if(mBBoxes.at(0)->intersect(mBBoxes.at(i))&& mObjectPool.at(i+5)->visiable)
        {
            mObjectPool.at(i+5)->visiable = false;
            index = i;
            break;
        }
    }
    return index;
}


//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();



    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mTimer.start(16, this); //this
        mTimeStart.start();
    }
}

//This function is called everytime the timer "ticks"
//the name timerEvent() is built into Qt - what the QTimer looks for
void RenderWindow::timerEvent(QTimerEvent *)
{
    // rub the FSM
    mFSM->run();
    // simple collision check
    int index = checkCollision();
    if(index!=-1)
        emit mFSM->itemTaken(index+1);
    //calling the
    render();

    //The rest here is just to show the frame rate:
    int msSinceLastFrame = mTimeStart.restart();    //restart() returns ms since last restart.
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 60) //once pr 60 frames =  update the message once pr second
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time Pr FrameDraw: " + QString::number(msSinceLastFrame) + " ms");
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

/// Uses QOpenGLDebugLogger if present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}



///Just a silly test to show that keyboard input works
/// Updates the statusbar in the program
void RenderWindow::playerControl(QKeyEvent* event)
{
    auto player = static_cast<Player*>(mObjectPool.at(12));
    auto terrain = static_cast<TriangleSurface*>(mObjectPool.at(2));
    vec3 position = player->getPosition();
    float height = terrain->findObject(vec2(position.x(),position.z()));
    player->setHeight(height);

    if(event->modifiers() & Qt::ShiftModifier)
    {
        if (event->key() == Qt::Key_D)
        {
            player->move(0.15f,height,0.f);
        }
        if (event->key() == Qt::Key_A)
        {
            player->move(-0.15f,height,-0.f);

        }
        if (event->key() == Qt::Key_W)
        {
            player->move(0.f,height,-0.15f);

        }
        if (event->key() == Qt::Key_S)
        {
            player->move(0.f,height,0.15f);

        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    //if(event->modifiers() & Qt::ShiftModifier)
    {

        if (event->key() == Qt::Key_D)
        {
            mCamera->moveRight();
        }
        if (event->key() == Qt::Key_A)
        {
            mCamera->moveLeft();
        }
        if (event->key() == Qt::Key_W)
        {
            mCamera->moveForward();
        }
        if (event->key() == Qt::Key_S)
        {
            mCamera->moveBackward();
        }
        if (event->key() == Qt::Key_E)
        {
            mCamera->moveUp();
        }
        if (event->key() == Qt::Key_Q)
        {
            mCamera->moveDown();
        }
    }
    playerControl(event);

    event->accept();
}


