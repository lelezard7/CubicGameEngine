#ifndef _CG_VIEWPORT_
#define _CG_VIEWPORT_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QKeyEvent>
#include "CG_Scene.h"
#include "CG_Primitives.h"
#include "../CG_ConsoleManager.h"
#include "CG_ViewportCamera.h"


class Viewport : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

    CGStdShaderProgram* shaderProgram_;

    CGScene* scene_;
//    CGPrimitives* primitives_;
    const CGConsoleManager* consoleManager_;

//    CGViewportCamera* viewportCamera_;
//    CGCamera* camera_;
    CGDynamicCamera* dynamicCamera_;
    CGTriangle triangle_;
    CGCube cube_;
    CGCube cube_2_;

    bool isRcmPressed_;

public:
    Viewport(QWidget* parent = nullptr);
    ~Viewport();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int newWidth, int newHeight) override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void ddd(const CGConsoleManager* consoleManager);
};

#endif
