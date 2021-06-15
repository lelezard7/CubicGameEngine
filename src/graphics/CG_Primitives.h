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
    QOpenGLBuffer vertexBuffer_;
    QOpenGLBuffer indexBuffer_;

    QVector<CGVertex> vertices_;
    QVector<GLuint> indices_;

    QMatrix4x4 modelMatrix_;

    QVector3D position_;
    QVector3D size_;
    QVector3D rotation_;
    float rotationAngle_;

public:
    CGObject();

    void create();

    QVector<CGVertex> getVertices() const;
    QVector<GLuint> getIndices() const;

    int verticesCount() const;
    int indicesCount() const;

    QMatrix4x4 getModelMatrix() const;

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);
    QVector3D getPosition() const;

    void setSize(float x, float y, float z);
    void setSize(const QVector3D& size);
    QVector3D getSize() const;

    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, const QVector3D& vector);
    QVector3D getRotationAxis() const;
    float getRotationAngle() const;

    void bindVertexBuffer();
    void bindIndexBuffer();

    void releaseVertexBuffer();
    void releaseIndexBuffer();

private:
    void applyTransformation();

protected:
    virtual QVector<CGVertex> createVertices() = 0;
    virtual QVector<GLuint> createIndices() = 0;

};

class CGStdObject : public CGObject
{
    CGStdShaderProgram* shaderProgram_;

public:


    CGStdObject();

    void setShaderProgram(CGStdShaderProgram* shaderProgram);
    CGStdShaderProgram* getShaderProgram() const;

    void bind();
    void release();
    virtual void draw();

};


class CGCube : public CGStdObject
{
    QVector<CGVertex> createVertices() override;
    QVector<GLuint> createIndices() override;
};













//class CGPrimitives : public QOpenGLFunctions
//{
//    const QWidget* parent_;

//    QOpenGLShaderProgram shaderProgram_;
//    GLuint VBO_;
//    GLuint VAO_;
//    GLuint EBO_;

//public:
//    CGPrimitives(const QWidget* parent);

//    void initialize();
//    const QOpenGLShaderProgram* getProgram();
//    int draw();

//};














class CGTriangle : public QOpenGLFunctions
{
    QOpenGLShaderProgram* shaderProgram_;

    QVector<CGVertex> vertices_;
    QVector<GLuint> indices_;

    QOpenGLBuffer vertexBuffer_;
    QOpenGLBuffer indexBuffer_;

public:
    CGTriangle();
    CGTriangle(const CGVertex* vertices, size_t n);
    ~CGTriangle();

    void initialize();
    void setShaderProgram(QOpenGLShaderProgram* shaderProgram);
    void bind();
    void draw();

};

//class CGCube : public QOpenGLFunctions
//{
//    QOpenGLShaderProgram* shaderProgram_;

//    QVector<CGVertex> vertices_;
//    QVector<GLuint> indices_;

//    QOpenGLBuffer vertexBuffer_;
//    QOpenGLBuffer indexBuffer_;

//    int _location;


//public:
//    CGCube();
//    ~CGCube();

//    void initialize();
//    void setShaderProgram(QOpenGLShaderProgram* shaderProgram);
//    void bind();
//    void draw();

//};


















#endif
