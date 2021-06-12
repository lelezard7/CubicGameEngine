#ifndef _CG_PRIMITIVES_
#define _CG_PRIMITIVES_

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector>
#include "CG_ViewportCamera.h"
#include "../CG_Pair.h"
#include <vector>



struct CGVertex
{
    CGVertex();
    CGVertex(const CGVertex& other);

    CGVertex(const QVector3D& position);
    CGVertex(const QVector3D& position, const QVector4D& color);
    CGVertex(const QVector3D& position, const QVector4D& color, const QVector3D& normal);

    QVector3D position;
    QVector4D color;
    QVector3D normal;

    void operator=(const CGVertex& other);
};

#define CG_SHADER_PROGRAM_VERTICES   1
#define CG_SHADER_PROGRAM_COLORS     2
#define CG_SHADER_PROGRAM_NORMALS    3

#define CG_SHADER_PROGRAM_VIEW_MATRIX             4
#define CG_SHADER_PROGRAM_MODEL_MATRIX            5
#define CG_SHADER_PROGRAM_PROJECTION_MATRIX       6
#define CG_SHADER_PROGRAM_TRANSFORMATION_MATRIX   7


struct CGShaderProgramData
{
    CGShaderProgramData() : dataType(-1), name(""), location(-1) {};
    CGShaderProgramData(int dataType, QString name, int location)
        : dataType(dataType), name(name), location(location) {};

    unsigned int dataType;
    QString name;
    int location;
};



class CGShaderProgram : public QOpenGLShaderProgram
{
    QVector<CGShaderProgramData> types_;

public:
    void addType(unsigned int type, QString name, bool isUniform);
    void removeType(unsigned int type);

    int initializeVariables();
    int refreshVariables();

protected:
    virtual int initializeVariables(const QVector<CGShaderProgramData>& types) = 0;
    virtual int refreshVariables(const QVector<CGShaderProgramData>& types) = 0;

};



class CGStdShaderProgram : public CGShaderProgram
{
    const QMatrix4x4* transformationMatrix_;

public:
    void setTransformationMatrix(const QMatrix4x4* transformationMatrix);

protected:
    int initializeVariables(const QVector<CGShaderProgramData>& types) override;
    int refreshVariables(const QVector<CGShaderProgramData>& types) override;

    const QMatrix4x4* getTransformationMatrix();

};






//    QOpenGLShaderProgram shaderProgram_;

//    QString vertexShaderFileName_;
//    QString fragmentShaderFileName_;

//    QString vertexShaderSourceCode_;
//    QString fragmentShaderSourceCode_;

//    QVector<CGShaderProgramData> types_;

//public:
//    CGShaderProgram();

//    void setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName);
//    void setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString source);

//    void addType(int type, QString name, bool isUniform);
//    void removeType(int type);

//    void initialize();
//    void refresh();

//protected:
//    virtual bool initialize(const CGShaderProgramData& shaderProgramData) = 0;
//    virtual bool refresh(const CGShaderProgramData& shaderProgramData) = 0;

////    void setTransformationMatrix(QMatrix4x4 modelMatrix_, QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix);






class CGObject : public QOpenGLFunctions
{
    QOpenGLShaderProgram* shaderProgram_;

    QString projectionMatrixName_;
    QString viewMatrixName_;
    QString modelMatrixName_;

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

    void setShaderProgram(); // ??

    QOpenGLShaderProgram* getShaderProgram() const;

    QMatrix4x4 getModelMatrix() const;

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);
    QVector3D getPosition() const;

    void setSize(float x, float y, float z);
    void setSize(const QVector3D& size);
    QVector3D getSize() const;

//    void rotate(float angle, float x, float y, float z);
//    void rotate(float angle, const QVector3D& vector);
//    QVector3D getRotation() const;



    void bind();
    void release();
    void draw();

protected:
    virtual QVector<CGVertex> createVertices() = 0;
    virtual QVector<GLuint> createIndices() = 0;

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

class CGCube : public QOpenGLFunctions
{
    QOpenGLShaderProgram* shaderProgram_;

    QVector<CGVertex> vertices_;
    QVector<GLuint> indices_;

    QOpenGLBuffer vertexBuffer_;
    QOpenGLBuffer indexBuffer_;

    int _location;


public:
    CGCube();
    ~CGCube();

    void initialize();
    void setShaderProgram(QOpenGLShaderProgram* shaderProgram);
    void bind();
    void draw();

};


















#endif
