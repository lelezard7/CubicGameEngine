#include "CG_Primitives.h"


CGObject::CGObject()
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer)
{
    modelMatrix_.setToIdentity();

    position_ = QVector3D(0.0f, 0.0f, -1.0f);
    size_ = QVector3D(1.0f, 1.0f, 1.0f);
    rotation_ = QVector3D(0.0f, 0.0f, 1.0f);

    rotationAngle_ = 0.0f;
}

void CGObject::create()
{
    initializeOpenGLFunctions();

    vertices_ = createVertices();
    indices_ = createIndices();

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


}

QVector<CGVertex> CGObject::getVertices() const
{
    return vertices_;
}

QVector<GLuint> CGObject::getIndices() const
{
    return indices_;
}

int CGObject::verticesCount() const
{
    return vertices_.size();
}

int CGObject::indicesCount() const
{
    return indices_.size();
}

QMatrix4x4 CGObject::getModelMatrix() const
{
    return modelMatrix_;
}

void CGObject::setPosition(float x, float y, float z)
{
    position_.setX(x);
    position_.setY(y);
    position_.setZ(z);

    applyTransformation();
}

void CGObject::setPosition(const QVector3D& position)
{
    setPosition(position.x(), position.y(), position.z());
}

QVector3D CGObject::getPosition() const
{
    return position_;
}

void CGObject::setSize(float x, float y, float z)
{
    size_.setX(x);
    size_.setY(y);
    size_.setZ(z);

    applyTransformation();
}

void CGObject::setSize(const QVector3D& size)
{
    setSize(size.x(), size.y(), size.z());
}

QVector3D CGObject::getSize() const
{
    return size_;
}

void CGObject::rotate(float angle, float x, float y, float z)
{
    rotationAngle_ = angle;

    rotation_.setX(x);
    rotation_.setY(y);
    rotation_.setZ(z);

    applyTransformation();
}

void CGObject::rotate(float angle, const QVector3D& vector)
{
    rotate(angle, vector.x(), vector.y(), vector.z());
}

QVector3D CGObject::getRotationAxis() const
{
    return rotation_;
}

float CGObject::getRotationAngle() const
{
    return rotationAngle_;
}

void CGObject::bindVertexBuffer()
{
    vertexBuffer_.bind();
}

void CGObject::bindIndexBuffer()
{
    indexBuffer_.bind();
}

void CGObject::releaseVertexBuffer()
{
    vertexBuffer_.release();
}

void CGObject::releaseIndexBuffer()
{
    indexBuffer_.release();
}

void CGObject::applyTransformation()
{
    modelMatrix_.setToIdentity();

    modelMatrix_.translate(position_);
    modelMatrix_.rotate(rotationAngle_, rotation_);
    modelMatrix_.scale(size_);
}











CGStdObject::CGStdObject()
{
    shaderProgram_ = nullptr;
}

void CGStdObject::setShaderProgram(CGStdShaderProgram* shaderProgram)
{
    shaderProgram_ = shaderProgram;
    shaderProgram_->CGShaderProgram::initializeVariables();
}

CGStdShaderProgram* CGStdObject::getShaderProgram() const
{
    return shaderProgram_;
}

void CGStdObject::bind()
{
    shaderProgram_->bind();
    bindVertexBuffer();
    bindIndexBuffer();
}

void CGStdObject::release()
{
    shaderProgram_->release();
    releaseVertexBuffer();
    releaseIndexBuffer();
}

void CGStdObject::draw()
{
    CGCamera* _camera = CGCamera::getActiveCamera();

    QMatrix4x4 ere = _camera->getProjectionMatrix() * _camera->getViewMatrix() * getModelMatrix();

    shaderProgram_->setMatrix(CG_SHADER_U_TRANSFORMATION_MATRIX, &ere);
    shaderProgram_->CGShaderProgram::refreshVariables();

    shaderProgram_->setUniformValue("u_gridColor", QVector4D(0.0f, 1.0f, 0.0f, 1.0f));

    glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, nullptr);

}







//    if (modelMatrixName_ == viewMatrixName_ == projectionMatrixName_) {
//        shaderProgram_->setUniformValue(modelMatrixName_.toStdString().c_str(),
//            _camera->getProjectionMatrix() * _camera->getViewMatrix() * modelMatrix_);
//    }
//    else if (modelMatrixName_ == viewMatrixName_) {
//        shaderProgram_->setUniformValue(modelMatrixName_.toStdString().c_str(),
//                                        _camera->getViewMatrix() * modelMatrix_);

//        shaderProgram_->setUniformValue(projectionMatrixName_.toStdString().c_str(),
//                                        _camera->getProjectionMatrix());
//    }
//    else if (viewMatrixName_ == projectionMatrixName_) {
//        shaderProgram_->setUniformValue(viewMatrixName_.toStdString().c_str(),
//                                        _camera->getViewMatrix() * _camera->getProjectionMatrix());

//        shaderProgram_->setUniformValue(modelMatrixName_.toStdString().c_str(), modelMatrix_);
//    }
//    else {
//        shaderProgram_->setUniformValue(modelMatrixName_.toStdString().c_str(), modelMatrix_);
//        shaderProgram_->setUniformValue(viewMatrixName_.toStdString().c_str(), _camera->getViewMatrix());
//        shaderProgram_->setUniformValue(projectionMatrixName_.toStdString().c_str(), _camera->getProjectionMatrix());
//    }

//    int _location = shaderProgram_->attributeLocation("in_gridPosition");
//    shaderProgram_->enableAttributeArray(_location);
//    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 0, 3, sizeof(CGVertex));

//    _location = shaderProgram_->attributeLocation("in_gridColor");
//    shaderProgram_->enableAttributeArray(_location);
//    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 3, 4, sizeof(CGVertex));

//    _location = shaderProgram_->attributeLocation("in_gridNormal");
//    shaderProgram_->enableAttributeArray(_location);
//    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 7, 3, sizeof(CGVertex));





QVector<CGVertex> CGCube::createVertices()
{
    QVector<CGVertex> _vertices {
        CGVertex(QVector3D(-1.0f, -1.0f,  1.0f)),
        CGVertex(QVector3D(-1.0f,  1.0f,  1.0f)),
        CGVertex(QVector3D( 1.0f,  1.0f,  1.0f)),
        CGVertex(QVector3D( 1.0f, -1.0f,  1.0f)),

        CGVertex(QVector3D(-1.0f, -1.0f, -1.0f)),
        CGVertex(QVector3D(-1.0f,  1.0f, -1.0f)),
        CGVertex(QVector3D( 1.0f,  1.0f, -1.0f)),
        CGVertex(QVector3D( 1.0f, -1.0f, -1.0f)),
    };

    return _vertices;
}

QVector<GLuint> CGCube::createIndices()
{
    QVector<GLuint> _indices {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 5, 7,

        1, 2, 5,
        5, 6, 2,

        0, 3, 4,
        4, 7, 3,

        0, 1, 5,
        4, 5, 0,

        2, 3, 7,
        6, 7, 2
    };

    return _indices;
}












//CGPrimitives::CGPrimitives(const QWidget* parent)
//{
//    parent_ = parent;
//}

//void CGPrimitives::initialize()
//{
//    initializeOpenGLFunctions();

//    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/sky_def_vert");
//    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/sky_def_frag");
//    shaderProgram_.link();



//    glGenBuffers(1, &VBO_);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

////    glGenV(1, &VBO_);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
//}

//const QOpenGLShaderProgram* CGPrimitives::getProgram()
//{
//    return &shaderProgram_;
//}

//int CGPrimitives::draw()
//{
//    return parent_->width();
//}
















CGTriangle::CGTriangle()
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer)
{
    vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, 0.0f)));
    vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, 0.0f)));
    vertices_.push_back(CGVertex(QVector3D( 0.0f,  1.0f, 0.0f)));

    indices_.push_back(0);
    indices_.push_back(1);
    indices_.push_back(2);
}

CGTriangle::CGTriangle(const CGVertex* vertices, size_t n)
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer)
{
    for (size_t i = 0; i < 3 && i < n; ++i) {
        vertices_.push_back(*vertices);
    }

    indices_.push_back(0);
    indices_.push_back(1);
    indices_.push_back(2);
}

CGTriangle::~CGTriangle()
{

}

void CGTriangle::initialize()
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
}

void CGTriangle::setShaderProgram(QOpenGLShaderProgram* shaderProgram)
{
    shaderProgram_ = shaderProgram;
}

void CGTriangle::bind()
{

}

void CGTriangle::draw()
{
    shaderProgram_->bind();
    vertexBuffer_.bind(); //??
    indexBuffer_.bind();

    int _location = shaderProgram_->attributeLocation("in_gridPosition");
    shaderProgram_->enableAttributeArray(_location);
    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 0, 3, sizeof(CGVertex));


    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}















//CGCube::CGCube()
//    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
//      indexBuffer_(QOpenGLBuffer::IndexBuffer)
//{
//    vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, 1.0f)));
//    vertices_.push_back(CGVertex(QVector3D(-1.0f,  1.0f, 1.0f)));
//    vertices_.push_back(CGVertex(QVector3D( 1.0f,  1.0f, 1.0f)));
//    vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, 1.0f)));

//    vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, -1.0f)));
//    vertices_.push_back(CGVertex(QVector3D(-1.0f,  1.0f, -1.0f)));
//    vertices_.push_back(CGVertex(QVector3D( 1.0f,  1.0f, -1.0f)));
//    vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, -1.0f)));

//    indices_.push_back(0); indices_.push_back(1); indices_.push_back(2);
//    indices_.push_back(0); indices_.push_back(2); indices_.push_back(3);

//    indices_.push_back(4); indices_.push_back(5); indices_.push_back(6);
//    indices_.push_back(4); indices_.push_back(5); indices_.push_back(7);

//    indices_.push_back(1); indices_.push_back(2); indices_.push_back(5);
//    indices_.push_back(5); indices_.push_back(6); indices_.push_back(2);

//    indices_.push_back(0); indices_.push_back(3); indices_.push_back(4);
//    indices_.push_back(4); indices_.push_back(7); indices_.push_back(3);

//    indices_.push_back(0); indices_.push_back(1); indices_.push_back(5);
//    indices_.push_back(4); indices_.push_back(5); indices_.push_back(0);

//    indices_.push_back(2); indices_.push_back(3); indices_.push_back(7);
//    indices_.push_back(6); indices_.push_back(7); indices_.push_back(2);
//}

//CGCube::~CGCube()
//{

//}

//void CGCube::initialize()
//{
//    initializeOpenGLFunctions();

//    vertexBuffer_.create();
//    vertexBuffer_.bind();
//    vertexBuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vertexBuffer_.allocate(vertices_.constData(), vertices_.size() * sizeof(CGVertex));
//    vertexBuffer_.release();

//    indexBuffer_.create();
//    indexBuffer_.bind();
//    indexBuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    indexBuffer_.allocate(indices_.constData(), indices_.size() * sizeof(GLuint));
//    indexBuffer_.release();
//}

//void CGCube::setShaderProgram(QOpenGLShaderProgram* shaderProgram)
//{
//    shaderProgram_ = shaderProgram;
//    _location = shaderProgram_->attributeLocation("in_gridPosition");
//    shaderProgram_->enableAttributeArray(_location);
//}

//void CGCube::bind()
//{

//}

//void CGCube::draw()
//{
//    shaderProgram_->bind();
//    indexBuffer_.bind();
//    vertexBuffer_.bind(); //??

//    QMatrix4x4 _model;
//    _model.setToIdentity();

//    _model.translate(0.0f, 0.0f, -15.0f);
//    _model.rotate(30.0f, QVector3D(3.0f, 3.0f, 3.0f));
//    _model.scale(3.0f, 3.0f, 3.0f);



//    CGCamera* _camera = CGCamera::getActiveCamera();
//    QString dd = "u_modelViewProjectionMatrix";
//    shaderProgram_->setUniformValue(dd.toStdString().c_str(),
//                                    _camera->getProjectionMatrix() * _camera->getViewMatrix() * _model);


//    shaderProgram_->setUniformValue("u_gridColor", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));


//    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 0, 3, sizeof(CGVertex));

//    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);

//    vertexBuffer_.release();
//    indexBuffer_.release();
//    shaderProgram_->release();
//}


































