#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>
#include <vector>
#include "visualobject.h"
#include "camera.h"
#include "boundingbox.h"
#include "fsm.h"
#include "light.h"


class QOpenGLContext;
class Shader;
class MainWindow;
class BoundingBox;
///This inherits from QWindow so we can put it inside a QWidget
/// It has to inherit from QWindow, not a QWidget because we want to have
/// access to the full OpenGL, and then we have to set the format for this window.
/// Not possible for a Widget.
///
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL stuff
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;

    void error(const QString &msg);

    void setUpShader();
    void setUpPhongShader();
    std::vector<VisualObject*>&getObjectPool(){return mObjectPool;}

    void playerControl(QKeyEvent* event);
    void setPointsHeight(std::vector<vec3>& points);

public slots:
    void render();
    int checkCollision();
    void changeSpline();

private:

    void init();
    void startOpenGLDebugger();
    void setBoxVector();

    std::vector<vec3> mControllPoints;
    QOpenGLContext *mContext;
    bool mInitialized;

    Shader *mShaderProgram;
    Shader *mPhShaderProgram;
    GLint  mMatrixUniform;
    GLint  mPMatrixUniform;
    GLint  mVMatrixUniform;
    GLint  mPhMatrixUniform;
    GLint  mPhPMatrixUniform;
    GLint  mPhVMatrixUniform;

    GLint mAmbientIntensityUniform{-1};
    GLint mAmbientColourUniform{-1};
    GLint mDiffuseIntensityUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularIntensityUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mObjectColor{-1};
    Light* light;

    std::vector<BoundingBox*> mBBoxes;
    GLuint mVAO;
    GLuint mVBO;

    QMatrix4x4 *mMVPmatrix; //The matrix with the transform for the object we draw

    QBasicTimer mTimer;     //timer that drives the gameloop
    QTime mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    std::vector<VisualObject*> mObjectPool;
    float mAspectRation{0.f};
    Camera* mCamera;
    bool sendM{false};
    QString message{"Custom message"};
    FSM* mFSM;


protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}

    void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
