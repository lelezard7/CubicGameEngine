#include "CG_Scene.h"


// CGScene

CGScene::CGScene(const QWidget* parent)
{
    parent_ = parent;

    sky_ = new CGSky(parent);
    grid_ = new CGGrid(parent);
}

CGScene::~CGScene()
{
    delete sky_;
    delete grid_;
}

void CGScene::setShaderProgramGrid(QOpenGLShaderProgram* shaderProgram)
{
    grid_->setShaderProgram(shaderProgram);
}

void CGScene::initialize()
{
    sky_->initialize();
    grid_->initialize();
}

//QOpenGLShaderProgram* CGScene::getProgram()
//{
//    return grid_->getProgram();
//}

void CGScene::draw()
{
    sky_->draw();
    grid_->draw();
}


// CGSky

CGSky::CGSky(const QWidget* parent)
{
    parent_ = parent;
}

CGSky::~CGSky()
{
}

void CGSky::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0.39f, 0.41f, 1.0f, 1.0f);
}

void CGSky::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


// CGGrid

CGGrid::CGGrid(const QWidget* parent)
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer)
{
    for (float i = -0.9f; i <= 1.0f; i += 0.0001f) {
        vertices_.push_back(CGVertex(QVector3D(i, 0.0f,  1.0f)));
        vertices_.push_back(CGVertex(QVector3D(i, 0.0f, -1.0f)));

        vertices_.push_back(CGVertex(QVector3D(-1.0f, 0.0f, i)));
        vertices_.push_back(CGVertex(QVector3D( 1.0f, 0.0f, i)));
    }

    for (int i = 0; i < vertices_.size(); ++i) {
        indices_.push_back(i);
    }

    parent_ = parent;
}

CGGrid::~CGGrid()
{
}

void CGGrid::initialize()
{
    initializeOpenGLFunctions();

    vertexBuffer_.create();
    vertexBuffer_.bind();
    vertexBuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer_.allocate(vertices_.constData(), vertices_.size() * sizeof(CGVertex));
    vertexBuffer_.release();

    indexBuffer_.create();
    indexBuffer_.bind();
    indexBuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBuffer_.allocate(indices_.constData(), indices_.size() * sizeof(GLuint));
    indexBuffer_.release();



//    QMatrix4x4 _model;
//    _model.setToIdentity();
//    _model.translate(0.0f, -1.0f, -5.0f);
//    _model.rotate(45.0f, 1.0f, 0.0f, 0.0f);

//    QMatrix4x4 _matrix;
//    _matrix.setToIdentity();
//    _matrix.perspective(45.0f, parent_->width()/parent_->height(), 1.0f, 100.0f);


//    shaderProgram_.setUniformValue("u_modelViewProjectionMatrix", _matrix * _model);
}

void CGGrid::createGrid()
{
}

void CGGrid::setShaderProgram(QOpenGLShaderProgram* shaderProgram)
{
    shaderProgram_ = shaderProgram;
}

//QOpenGLShaderProgram* CGGrid::getProgram()
//{
//    return &shaderProgram_;
//}

void CGGrid::draw()
{
    shaderProgram_->bind();
    vertexBuffer_.bind();
    indexBuffer_.bind();

    QMatrix4x4 _model;
    _model.setToIdentity();
    _model.translate(0.0f, 0.0f, -15.0f);
    _model.scale(20000.0f, 20000.0f, 20000.0f);

    CGCamera* _camera = CGCamera::getActiveCamera();
    shaderProgram_->setUniformValue("u_modelViewProjectionMatrix",
                                    _camera->getProjectionMatrix() * _camera->getViewMatrix() * _model);

    shaderProgram_->setUniformValue("u_gridColor", QVector4D(0.0f, 1.0f, 0.0f, 1.0f));

    int _location = shaderProgram_->attributeLocation("in_gridPosition");
    shaderProgram_->enableAttributeArray(_location);
    shaderProgram_->setAttributeArray(_location, GL_FLOAT, 0, 3, sizeof(CGVertex));

    glDrawElements(GL_LINES, indices_.size(), GL_UNSIGNED_INT, nullptr);

//    vertexBuffer_.release();
//    indexBuffer_.release();
//    shaderProgram_->release();
}
