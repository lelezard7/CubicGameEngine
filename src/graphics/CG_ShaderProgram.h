#ifndef _CG_SHADER_PROGRAM_
#define _CG_SHADER_PROGRAM_

#include "../CG_Pair.h"

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <QVector>
#include <QString>

// От 0x0001 до 0x8000
#define CG_SHADER_A_VERTICES    0x00000001
#define CG_SHADER_A_COLORS      0x00000002
#define CG_SHADER_A_NORMALS     0x00000004

// От 0x00010000 до 0x80000000
#define CG_SHADER_U_MODEL_MATRIX            0x00010000
#define CG_SHADER_U_VIEW_MATRIX             0x00020000
#define CG_SHADER_U_PROJECTION_MATRIX       0x00040000
#define CG_SHADER_U_TRANSFORMATION_MATRIX   0x00080000
#define CG_SHADER_U_COLOR                   0x00100000


//TODO: Тут все плохо. Переделать все -_- !
//Класс CGShaderProgram не удобен в использовании в целом,
//а класс CGStdShaderProgram и подавно один большой костыль на теле общества.
//Делал их как временную затычку на время изучения OpenGL.


class CGShaderProgram : public QOpenGLShaderProgram
{
protected:
    struct CGVariableData
    {
        CGVariableData();
        CGVariableData(unsigned int mask, QString name, int location, bool isUniform, bool isInit);

        unsigned int mask;
        QString name;
        int location;
        bool isUniform;
        bool isInit;
    };

private:
    QVector<CGVariableData> variablesData_;

public:
    CGShaderProgram();
    CGShaderProgram(const CGShaderProgram& other);
    virtual ~CGShaderProgram();

    bool addVariableData(unsigned int mask, QString name, bool isUniform = false, bool isInit = false);
    void removeVariableData(unsigned int mask);

    bool findVariableData(unsigned int mask) const;
    int findVariableData(QString name) const;

    int variablesDataSize() const;

    int initializeVariables();
    int refreshVariables();

    const CGShaderProgram& operator=(const CGShaderProgram& other);

protected:
    const CGVariableData* getVariableData(unsigned int mask) const;

    virtual int initializeVariables(const QVector<CGVariableData>& variablesData) = 0;
    virtual int refreshVariables(const QVector<CGVariableData>& variablesData) = 0;

};


class CGStdShaderProgram : public CGShaderProgram
{
public:
    enum StorageType
    {
        SHORT_TERM_STORAGE,
        PERMANENT_STORAGE
    };

protected:
    typedef CGPair<unsigned int, const QMatrix4x4*> AnonMatrix;
    typedef CGPair<unsigned int, const QVector3D*> AnonVector3D;
    typedef CGPair<unsigned int, const QVector4D*> AnonVector4D;

    struct Storage
    {
        Storage();
        Storage(const Storage& other);

        AnonMatrix matrix_;
        AnonVector3D vector3D_;
        AnonVector4D vector4D_;
    };

private:
    Storage shortTermStorage_;
    Storage permanentStorage_;

public:
    CGStdShaderProgram();
    CGStdShaderProgram(const CGStdShaderProgram& other);
    virtual ~CGStdShaderProgram();

    int setMatrix(unsigned int mask, const QMatrix4x4* matrix, StorageType storageType);
    int setVector(unsigned int mask, const QVector3D* vector, StorageType storageType);
    int setVector(unsigned int mask, const QVector4D* vector, StorageType storageType);

protected:
    AnonMatrix getMatrix(StorageType storageType) const;
    AnonVector3D getVector3D(StorageType storageType) const;
    AnonVector4D getVector4D(StorageType storageType) const;

    void removePermanentMatrix();
    void removePermanentVector3D();
    void removePermanentVector4D();

    int initializeVariables(const QVector<CGVariableData>& variablesData) override;
    int refreshVariables(const QVector<CGVariableData>& variablesData) override;

};

#endif
