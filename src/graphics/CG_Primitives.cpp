#include "CG_Primitives.h"


CGObject::CGObject()
{
    vbo_usagePattern_ = QOpenGLBuffer::StaticDraw;
    ebo_usagePattern_ = QOpenGLBuffer::StaticDraw;

    vertexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    indexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    modelMatrix_.setToIdentity();

    position_ = QVector3D(0.0f, 0.0f, -1.0f);
    size_ = QVector3D(1.0f, 1.0f, 1.0f);

    rotation_.first = 0.0f;
    rotation_.second = QVector3D(0.0f, 0.0f, 1.0f);
}

CGObject::CGObject(const CGObject& other)
{
    vbo_usagePattern_ = other.vbo_usagePattern_;
    ebo_usagePattern_ = other.ebo_usagePattern_;

    vertexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    indexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    vertices_ = other.vertices_;
    indices_ = other.indices_;

    modelMatrix_ = other.modelMatrix_;

    position_ = other.position_;
    size_ = other.size_;
    rotation_ = other.rotation_;
}

CGObject::~CGObject()
{
    if (vertexBuffer_.isCreated()) {
        vertexBuffer_.release();
        vertexBuffer_.destroy();
    }

    if (indexBuffer_.isCreated()) {
        indexBuffer_.release();
        indexBuffer_.destroy();
    }
}

int CGObject::initialize()
{
    initializeOpenGLFunctions();

    if (vertexBuffer_.isCreated() || indexBuffer_.isCreated())
        return -1;

    if (!vertexBuffer_.create())
        return -1;

    if (!indexBuffer_.create()) {
        vertexBuffer_.destroy();
        return -1;
    }

    if (!setVertices(vertices_) || !setIndices(indices_))
        return 0;

    return 1;
}

bool CGObject::setVertices(QVector<CGVertex> vertices)
{
    if (vertices.size() == 0)
        return false;

    if (!vertexBuffer_.isCreated()) {
        vertices_ = vertices;
        return true;
    }

    if (!vertexBuffer_.bind())
        return false;

    vertexBuffer_.setUsagePattern(vbo_usagePattern_);
    vertexBuffer_.allocate(vertices.constData(), vertices.size() * sizeof(CGVertex));
    vertexBuffer_.release();

    vertices_ = vertices;
    return true;
}

bool CGObject::setIndices(QVector<GLuint> indices)
{
    if (indices.size() == 0)
        return false;

    if (!indexBuffer_.isCreated()) {
        indices_ = indices;
        return true;
    }

    if (!indexBuffer_.bind())
        return false;

    indexBuffer_.setUsagePattern(ebo_usagePattern_);
    indexBuffer_.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    indexBuffer_.release();

    indices_ = indices;
    return true;
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

void CGObject::setUsagePattern(QOpenGLBuffer::Type buffer, QOpenGLBuffer::UsagePattern usagePattern)
{
    if (buffer == QOpenGLBuffer::VertexBuffer) {
        vbo_usagePattern_ = usagePattern;
        setVertices(vertices_);
        return;
    }

    if (buffer == QOpenGLBuffer::IndexBuffer) {
        ebo_usagePattern_ = usagePattern;
        setIndices(indices_);
        return;
    }
}

QOpenGLBuffer::UsagePattern CGObject::getVboUsagePattern() const
{
    return vbo_usagePattern_;
}

QOpenGLBuffer::UsagePattern CGObject::getEboUsagePattern() const
{
    return ebo_usagePattern_;
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

void CGObject::setRotation(float angle, float x, float y, float z)
{
    rotation_.first = angle;

    rotation_.second.setX(x);
    rotation_.second.setY(y);
    rotation_.second.setZ(z);

    applyTransformation();
}

void CGObject::setRotation(float angle, const QVector3D& vector)
{
    setRotation(angle, vector.x(), vector.y(), vector.z());
}

QVector3D CGObject::getRotationAxis() const
{
    return rotation_.second;
}

float CGObject::getRotationAngle() const
{
    return rotation_.first;
}

bool CGObject::bind()
{
    if (!vertexBuffer_.bind())
        return false;

    if (!indexBuffer_.bind()) {
        vertexBuffer_.release();
        return false;
    }

    return true;
}

void CGObject::release()
{
    vertexBuffer_.release();
    indexBuffer_.release();
}

const CGObject& CGObject::operator=(const CGObject& other)
{
    vbo_usagePattern_ = other.vbo_usagePattern_;
    ebo_usagePattern_ = other.ebo_usagePattern_;

    vertexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    indexBuffer_ = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    vertices_ = other.vertices_;
    indices_ = other.indices_;

    modelMatrix_ = other.modelMatrix_;

    position_ = other.position_;
    size_ = other.size_;
    rotation_ = other.rotation_;

    return *this;
}

void CGObject::applyTransformation()
{
    modelMatrix_.setToIdentity();

    modelMatrix_.translate(position_);
    modelMatrix_.rotate(rotation_.first, rotation_.second);
    modelMatrix_.scale(size_);
}



CGStdObject::CGStdObject() : CGObject()
{
    shaderProgram_ = nullptr;
}

CGStdObject::CGStdObject(const CGStdObject& other) : CGObject(other)
{
    shaderProgram_ = other.shaderProgram_;
}

CGStdObject::~CGStdObject()
{
    shaderProgram_ = nullptr;
}

void CGStdObject::setShaderProgram(CGStdShaderProgram* shaderProgram)
{
    shaderProgram_ = shaderProgram;
}

CGStdShaderProgram* CGStdObject::getShaderProgram() const
{
    return shaderProgram_;
}

bool CGStdObject::bind()
{
    if (!shaderProgram_->bind())
        return false;

    return CGObject::bind();
}

void CGStdObject::release()
{
    shaderProgram_->release();
    CGObject::release();
}

int CGStdObject::draw()
{
    CGCamera* _camera = CGCamera::getActiveCamera();

    if (!_camera)
        return -1;

    QMatrix4x4 _transformationMatrix = _camera->getProjectionMatrix() * _camera->getViewMatrix() * getModelMatrix();
    shaderProgram_->setMatrix(CG_SHADER_U_TRANSFORMATION_MATRIX, &_transformationMatrix,
                              CGStdShaderProgram::SHORT_TERM_STORAGE);

    glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, nullptr);
    return 0;
}








CGCube::CGCube() : CGStdObject()
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

    setVertices(_vertices);
    setIndices(_indices);
}








//vertices_.push_back(CGVertex(QVector3D(-1.0f, -1.0f, 0.0f)));
//vertices_.push_back(CGVertex(QVector3D( 1.0f, -1.0f, 0.0f)));
//vertices_.push_back(CGVertex(QVector3D( 0.0f,  1.0f, 0.0f)));

//indices_.push_back(0);
//indices_.push_back(1);
//indices_.push_back(2);


