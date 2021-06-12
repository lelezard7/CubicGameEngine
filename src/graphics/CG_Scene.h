#ifndef _CG_SCENE_
#define _CG_SCENE_

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "../CG_ConsoleManager.h"
#include <QMatrix4x4>
#include <QVector>
#include "CG_Primitives.h"
#include <QOpenGLBuffer>

#include "../underConsideration/Camera.h"

class CGSky;
class CGGrid;


class CGScene : public QOpenGLFunctions
{
    const QWidget* parent_;

    CGSky* sky_;
    CGGrid* grid_;

public:
    CGScene(const QWidget* parent);
    ~CGScene();

    void setShaderProgramGrid(QOpenGLShaderProgram* shaderProgram);

    void initialize();
//    QOpenGLShaderProgram* getProgram();
    void draw();

};


class CGSky : public QOpenGLFunctions
{
    const QWidget* parent_;

public:
    CGSky(const QWidget* parent);
    ~CGSky();

    void initialize();
    void draw();

};


class CGGrid : public QOpenGLFunctions
{
    const QWidget* parent_;

    QOpenGLShaderProgram* shaderProgram_;

    QOpenGLBuffer vertexBuffer_;
    QOpenGLBuffer indexBuffer_;

    QVector<CGVertex> vertices_;
    QVector<GLuint> indices_;

public:
    CGGrid(const QWidget* parent);
    ~CGGrid();

    void initialize();
    void createGrid();

    void setShaderProgram(QOpenGLShaderProgram* shaderProgram);

//    QOpenGLShaderProgram* getProgram();
    void draw();

};

#endif
