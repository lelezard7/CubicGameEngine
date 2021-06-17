#ifndef _CG_PRIMITIVES_
#define _CG_PRIMITIVES_

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QWidget>
#include <QOpenGLBuffer>
#include <QVector>
#include "CG_ViewportCamera.h"
#include "../CG_Pair.h"
#include "CG_ShaderProgram.h"
#include "CG_Vertex.h"


class CGObject : protected QOpenGLFunctions
{
    QOpenGLBuffer::UsagePattern vbo_usagePattern_;
    QOpenGLBuffer::UsagePattern ebo_usagePattern_;

    QOpenGLBuffer vertexBuffer_;
    QOpenGLBuffer indexBuffer_;

    QVector<CGVertex> vertices_;
    QVector<GLuint> indices_;

    QMatrix4x4 modelMatrix_;

    QVector3D position_;
    QVector3D size_;
    CGPair<float, QVector3D> rotation_;

public:
    CGObject();
    CGObject(const CGObject& other);
    virtual ~CGObject();

    int initialize();

    bool setVertices(QVector<CGVertex> vertices);
    bool setIndices(QVector<GLuint> indices);

    QVector<CGVertex> getVertices() const;
    QVector<GLuint> getIndices() const;

    int verticesCount() const;
    int indicesCount() const;

    void setUsagePattern(QOpenGLBuffer::Type buffer, QOpenGLBuffer::UsagePattern usagePattern);
    QOpenGLBuffer::UsagePattern getVboUsagePattern() const;
    QOpenGLBuffer::UsagePattern getEboUsagePattern() const;

    QMatrix4x4 getModelMatrix() const;

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);
    QVector3D getPosition() const;

    void setSize(float x, float y, float z);
    void setSize(const QVector3D& size);
    QVector3D getSize() const;

    void setRotation(float angle, float x, float y, float z);
    void setRotation(float angle, const QVector3D& vector);
    QVector3D getRotationAxis() const;
    float getRotationAngle() const;

    bool bind();
    void release();

    const CGObject& operator=(const CGObject& other);

private:
    void applyTransformation();

};


class CGStdObject : public CGObject
{
    CGStdShaderProgram* shaderProgram_;

public:
    CGStdObject();
    CGStdObject(const CGStdObject& other);
    virtual ~CGStdObject();

    void setShaderProgram(CGStdShaderProgram* shaderProgram);
    CGStdShaderProgram* getShaderProgram() const;

    bool bind();
    void release();





    virtual int draw();

};






class CGCube : public CGStdObject
{
public:
    CGCube();
};


#endif
