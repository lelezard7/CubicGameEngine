#include "CG_Primitives.h"


CGVertex::CGVertex() {}

CGVertex::CGVertex(const CGVertex& other)
    : position(other.position),
      color(other.color),
      normal(other.normal) {}

CGVertex::CGVertex(const QVector3D& position)
    : position(position),
      color(0.0f, 0.0f, 0.0f, 1.0f),
      normal(0.0f, 0.0f, 0.0f) {}

CGVertex::CGVertex(const QVector3D& position, const QVector4D& color)
    : position(position),
      color(color),
      normal(0.0f, 0.0f, 0.0f) {}

CGVertex::CGVertex(const QVector3D& position, const QVector4D& color, const QVector3D& normal)
    : position(position),
      color(color),
      normal(normal) {}

void CGVertex::operator=(const CGVertex& other)
{
    position = other.position;
    color = other.color;
    normal = other.normal;
}






//CGShaderProgram::CGShaderProgram()
//{
//}

//void CGShaderProgram::setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName)
//{
//    if (type == QOpenGLShader::Vertex) {

//        if (vertexShaderFileName_ != "") {
//            vertexShaderFileName_ = "";
//        }

//        if (vertexShaderSourceCode_ != "") {
//            vertexShaderSourceCode_ = "";
//        }


//        vertexShaderFileName_ = fileName;
//        shaderProgram_.addShaderFromSourceFile(type, fileName);

//        if (fragmentShaderFileName_ != "" || fragmentShaderSourceCode_ != "") {
//            shaderProgram_.link();
//        }
//    }

//    if (type == QOpenGLShader::Fragment) {

//        if (fragmentShaderFileName_ != "") {
//            fragmentShaderFileName_ = "";
//        }

//        if (fragmentShaderSourceCode_ != "") {
//            fragmentShaderSourceCode_ = "";
//        }

//        fragmentShaderFileName_ = fileName;
//        shaderProgram_.addShaderFromSourceFile(type, fileName);

//        if (vertexShaderFileName_ != "" || vertexShaderSourceCode_ != "") {
//            shaderProgram_.link();
//        }
//    }
//}

//void CGShaderProgram::setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString source)
//{
//    if (type == QOpenGLShader::Vertex) {

//        if (vertexShaderFileName_ != "") {
//            vertexShaderFileName_ = "";
//        }

//        if (vertexShaderSourceCode_ != "") {
//            vertexShaderSourceCode_ = "";
//        }

//        vertexShaderSourceCode_ = source;
//        shaderProgram_.addShaderFromSourceCode(type, vertexShaderSourceCode_);

//        if (fragmentShaderFileName_ != "" || fragmentShaderSourceCode_ != "") {
//            shaderProgram_.link();
//        }
//    }

//    if (type == QOpenGLShader::Fragment) {

//        if (fragmentShaderFileName_ != "") {
//            fragmentShaderFileName_ = "";
//        }

//        if (fragmentShaderSourceCode_ != "") {
//            fragmentShaderSourceCode_ = "";
//        }

//        fragmentShaderSourceCode_ = source;
//        shaderProgram_.addShaderFromSourceCode(type, fragmentShaderSourceCode_);

//        if (vertexShaderFileName_ != "" || vertexShaderSourceCode_ != "") {
//            shaderProgram_.link();
//        }
//    }
//}

void CGShaderProgram::addType(unsigned int type, QString name, bool isUniform)
{
    for (int i = 0; i < types_.size(); ++i) {
        if (types_[i].dataType == type || types_[i].name == name)
            return;
    }

    int _location;

    if (isUniform) {
        _location = uniformLocation(name);
        enableAttributeArray(_location);
    }

    types_.push_back(CGShaderProgramData(type, name, _location));
}

void CGShaderProgram::removeType(unsigned int type)
{
    for (int i = 0; i < types_.size(); ++i) {
        if (types_[i].dataType == type) {
            types_.remove(i);
            break;
        }
    }
}

int CGShaderProgram::initializeVariables()
{
    return initializeVariables(types_);
}

int CGShaderProgram::refreshVariables()
{
    return refreshVariables(types_);
}



void CGStdShaderProgram::setTransformationMatrix(const QMatrix4x4* transformationMatrix)
{
    transformationMatrix_ = transformationMatrix;

    CGShaderProgram::refreshVariables();

    transformationMatrix_ = nullptr;
}

int CGStdShaderProgram::initializeVariables(const QVector<CGShaderProgramData>& types)
{
    for (int i = 0; i < types.size(); ++i) {
        if (types[i].dataType == CG_SHADER_PROGRAM_VERTICES) {
            setAttributeBuffer(types[i].location, GL_FLOAT, 0, 3, sizeof(CGVertex));
            continue;
        }

        if (types[i].dataType == CG_SHADER_PROGRAM_COLORS) {
            setAttributeBuffer(types[i].location, GL_FLOAT, 3, 4, sizeof(CGVertex));
            continue;
        }

        if (types[i].dataType == CG_SHADER_PROGRAM_NORMALS) {
            setAttributeBuffer(types[i].location, GL_FLOAT, 7, 3, sizeof(CGVertex));
            continue;
        }
    }

    return 0;
}

int CGStdShaderProgram::refreshVariables(const QVector<CGShaderProgramData>& types)
{
    for (int i = 0; i < types.size(); ++i) {
        if (types[i].dataType == CG_SHADER_PROGRAM_TRANSFORMATION_MATRIX) {
            setUniformValue(types[i].location, *transformationMatrix_);
            break;
        }
    }

    return 0;
}

const QMatrix4x4* CGStdShaderProgram::getTransformationMatrix()
{
    return transformationMatrix_;
}
















CGObject::CGObject()
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer) {}

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

void CGObject::bind()
{
    shaderProgram_->bind();
    vertexBuffer_.bind();
    indexBuffer_.bind();
}

void CGObject::release()
{
    shaderProgram_->release();
    vertexBuffer_.release();
    indexBuffer_.release();
}

void CGObject::draw()
{
    modelMatrix_.setToIdentity();

    modelMatrix_.translate(position_);
    modelMatrix_.scale(size_);
    modelMatrix_.rotate(rotationAngle_, rotation_);

    CGCamera* _camera = CGCamera::getActiveCamera();















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

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
}

//void CGObject::setShaderProgram(QOpenGLShaderProgram* shaderProgram)
//{
//    shaderProgram_ = shaderProgram;
//}

QOpenGLShaderProgram* CGObject::getShaderProgram() const
{
    return shaderProgram_;
}

//QMatrix4x4 CGObject::getModelMatrix() const
//{

//}

void CGObject::setPosition(float x, float y, float z)
{

}

void CGObject::setPosition(const QVector3D& position)
{

}

//QVector3D CGObject::getPosition() const
//{

//}

void CGObject::setSize(float x, float y, float z)
{

}

void CGObject::setSize(const QVector3D& size)
{

}

//QVector3D CGObject::getSize() const
//{

//}

//void CGObject::rotate(float angle, float x, float y, float z)
//{

//}

//void CGObject::rotate(float angle, const QVector3D& vector)
//{

//}

//QVector3D CGObject::getRotation() const
//{

//}












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















CGCube::CGCube()
    : vertexBuffer_(QOpenGLBuffer::VertexBuffer),
      indexBuffer_(QOpenGLBuffer::IndexBuffer)
{
    vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, 1.0f)));
    vertices_.push_back(CGVertex(QVector3D(-1.0f,  1.0f, 1.0f)));
    vertices_.push_back(CGVertex(QVector3D( 1.0f,  1.0f, 1.0f)));
    vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, 1.0f)));

    vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, -1.0f)));
    vertices_.push_back(CGVertex(QVector3D(-1.0f,  1.0f, -1.0f)));
    vertices_.push_back(CGVertex(QVector3D( 1.0f,  1.0f, -1.0f)));
    vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, -1.0f)));

    indices_.push_back(0); indices_.push_back(1); indices_.push_back(2);
    indices_.push_back(0); indices_.push_back(2); indices_.push_back(3);

    indices_.push_back(4); indices_.push_back(5); indices_.push_back(6);
    indices_.push_back(4); indices_.push_back(5); indices_.push_back(7);

    indices_.push_back(1); indices_.push_back(2); indices_.push_back(5);
    indices_.push_back(5); indices_.push_back(6); indices_.push_back(2);

    indices_.push_back(0); indices_.push_back(3); indices_.push_back(4);
    indices_.push_back(4); indices_.push_back(7); indices_.push_back(3);

    indices_.push_back(0); indices_.push_back(1); indices_.push_back(5);
    indices_.push_back(4); indices_.push_back(5); indices_.push_back(0);

    indices_.push_back(2); indices_.push_back(3); indices_.push_back(7);
    indices_.push_back(6); indices_.push_back(7); indices_.push_back(2);
}

CGCube::~CGCube()
{

}

void CGCube::initialize()
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

void CGCube::setShaderProgram(QOpenGLShaderProgram* shaderProgram)
{
    shaderProgram_ = shaderProgram;
    _location = shaderProgram_->attributeLocation("in_gridPosition");
    shaderProgram_->enableAttributeArray(_location);
}

void CGCube::bind()
{

}

void CGCube::draw()
{
    shaderProgram_->bind();
    indexBuffer_.bind();
    vertexBuffer_.bind(); //??

    QMatrix4x4 _model;
    _model.setToIdentity();
    _model.translate(0.0f, 0.0f, -15.0f);
    _model.scale(3.0f, 3.0f, 3.0f);
    _model.rotate(30.0f, QVector3D(3.0f, 3.0f, 3.0f));

    CGCamera* _camera = CGCamera::getActiveCamera();
    QString dd = "u_modelViewProjectionMatrix";
    shaderProgram_->setUniformValue(dd.toStdString().c_str(),
                                    _camera->getProjectionMatrix() * _camera->getViewMatrix() * _model);


    shaderProgram_->setUniformValue("u_gridColor", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));


    shaderProgram_->setAttributeBuffer(_location, GL_FLOAT, 0, 3, sizeof(CGVertex));

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);

    vertexBuffer_.release();
    indexBuffer_.release();
    shaderProgram_->release();
}
































