#include "CG_Viewport.h"
//#include "
#include <QDebug>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);
    setUpdatesEnabled(true);

    isRcmPressed_ = false;

//    primitives_ = new CGPrimitives(parent);
//    viewportCamera_= new CGViewportCamera(parent);
//    camera_ = new CGCamera();
    dynamicCamera_ = new CGDynamicCamera();
    scene_ = new CGScene(parent);
}

Viewport::~Viewport()
{
    delete scene_;
}

void Viewport::initializeGL()
{
    initializeOpenGLFunctions();

    shaderProgram_ = new CGStdShaderProgram;

    shaderProgram_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/grid_def_vert");
    shaderProgram_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/grid_def_frag");
    shaderProgram_->link();
    shaderProgram_->bind();

    shaderProgram_->addVariableData(CG_SHADER_A_VERTICES, "in_gridPosition", false, false);
    shaderProgram_->addVariableData(CG_SHADER_U_TRANSFORMATION_MATRIX, "u_transformationMatrix", true, false);


//    shaderProgram_->addType(CG_SHADER_PROGRAM_COLORS, "u_gridColor", true);


    scene_->initialize();
    triangle_.initialize();
    cube_.create();
    cube_2_.create();

    cube_.setPosition(0.0f, 0.0f, -15.0f);
    cube_.setSize(5.0f, 2.0f, 1.0f);
    cube_.rotate(30.0f, 0.0f, 0.0f, 1.0f);

    cube_2_.setPosition(0.0f, 0.0f, 15.0f);
    cube_2_.setSize(1.0f, 1.0f, 1.0f);
    cube_2_.rotate(0.0f, 0.0f, 0.0f, 0.0f);



    scene_->setShaderProgramGrid(shaderProgram_);
    triangle_.setShaderProgram(shaderProgram_);
    cube_.setShaderProgram(shaderProgram_);
    cube_2_.setShaderProgram(shaderProgram_);

//    viewportCamera_->setMovementSpeed(1.0f);
//    viewportCamera_->setRotationalSpeed(0.0005f);

//    camera_->setPosition(0.0f, 0.0f, 0.0f);
//    camera_->setDrawDistance(0.001f, 1000.0f);
//    camera_->setYaw(-90.0f);

//    camera_->refresh();
//    camera_->use();

    dynamicCamera_->setPosition(0.0f, 0.0f, 0.0f);
    dynamicCamera_->setDrawDistance(0.001f, 1000.0f);
    dynamicCamera_->setYaw(-90.0f);
    dynamicCamera_->setMaxPitch(89);
    dynamicCamera_->setRotationalSpeed(1.0f);

    dynamicCamera_->refresh();
    dynamicCamera_->use();


    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);



//    QOpenGLShaderProgram* f = new CGStdShaderProgram;
//    f->
//    CGStdObject g;
//    d.setShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/grid_def_vert");
//    d.setShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/grid_def_frag");

}

void Viewport::paintGL()
{
    dynamicCamera_->refresh();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene_->draw();

    cube_.bind();
    cube_.draw();

    cube_2_.bind();
    cube_2_.draw();
}

void Viewport::resizeGL(int newWidth, int newHeight)
{
    qDebug() << "window width: " << newWidth << " height: " << newHeight;
//    viewportCamera_->setScreenSize(newWidth, newHeight);
    dynamicCamera_->turnOnPerspective(45.0f, newWidth/newHeight);
    dynamicCamera_->refresh();
    glViewport(0, 0, newWidth, newHeight);
}

void Viewport::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W) {
        dynamicCamera_->move(CGE::FORWARD);
//        viewportCamera_->move(CGE::FORWARD);
    }

    if (event->key() == Qt::Key_S) {
        dynamicCamera_->move(CGE::BACKWARD);
//        viewportCamera_->move(CGE::BACKWARD);
    }

    if (event->key() == Qt::Key_D) {
        dynamicCamera_->move(CGE::RIGHT);
//        camera_->setRoll(30.0f);
//        camera_->refresh();
//        viewportCamera_->move(CGE::RIGHT);
    }

    if (event->key() == Qt::Key_A) {
        dynamicCamera_->move(CGE::LEFT);
//        viewportCamera_->move(CGE::LEFT);
    }

//static int f = 0;
    if (event->key() == Qt::Key_Space) {
        dynamicCamera_->move(CGE::UP);
//        viewportCamera_->move(CGE::UP);
//        camera_->setPosition(0,f,0);
//        f++;
//        camera_->refresh();
    }

    if (event->key() == Qt::Key_Z) {
        dynamicCamera_->move(CGE::DOWN);
//        viewportCamera_->move(CGE::DOWN);
    }

    if (event->key() == Qt::Key_Q) {
        dynamicCamera_->rotate(90);
//        viewportCamera_->move(CGE::DOWN);
    }

    if (event->key() == Qt::Key_E) {
        dynamicCamera_->rotate(-90);
//        viewportCamera_->move(CGE::DOWN);
    }

    dynamicCamera_->refresh();
    update();
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
//    if (event->button() == Qt::RightButton) {
//        if (isRcmPressed_) {
////            setMouseTracking(false);
//            isRcmPressed_ = false;
//            qDebug() << "isRcmPressed: false";
//        }
//        else {
////            setMouseTracking(true);
//            isRcmPressed_ = true;
//            qDebug() << "isRcmPressed: true";
//        }
//    }

//    viewportCamera_->centerOn(true);
//    viewportCamera_->setCenter(event->x(), event->y());
    dynamicCamera_->setFocus();
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
//    viewportCamera_->centerOn(false);
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
//    if (isRcmPressed_) {
//        viewportCamera_->lookAt(event->x(), event->y());
//    camera_->setYaw(event->x());
//    camera_->setPitch(event->y());
//    camera_->refresh();
//        qDebug() << "x: " << event->x() << "y: " << event->y();

    dynamicCamera_->lookAt(event->x(), event->y());
    dynamicCamera_->refresh();
        update();
//    }
}

void Viewport::ddd(const CGConsoleManager* consoleManager)
{
    consoleManager_ = consoleManager;
    consoleManager_->sendWarningMessage("", "ssssssss", "sdf", "dsfs", "1");
}
