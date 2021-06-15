#include "CG_ShaderProgram.h"
#include "CG_Vertex.h"


CGShaderProgram::CGVariableData::
CGVariableData()
{
    mask = 0;
    name = "";
    location = -1;
    isUniform = false;
    isInit = false;
};

CGShaderProgram::CGVariableData::
CGVariableData(unsigned int mask, QString name, int location, bool isUniform, bool isInit)
{
    this->mask = mask;
    this->name = name;
    this->location = location;
    this->isUniform = isUniform;
    this->isInit = isInit;
}


CGShaderProgram::CGShaderProgram() {}

CGShaderProgram::CGShaderProgram(const CGShaderProgram& other)
{
    variablesData_ = other.variablesData_;
}

CGShaderProgram::~CGShaderProgram()
{
    variablesData_.clear();
}

bool CGShaderProgram::addVariableData(unsigned int mask, QString name, bool isUniform, bool isInit)
{
    if (mask <= 0)
        return false;

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].mask == mask)
            return false;
    }

    int _location;

    if (isUniform) {
        _location = uniformLocation(name);
        if (_location == -1)
            return false;
    }
    else {
        _location = attributeLocation(name);
        if (_location == -1)
            return false;

        enableAttributeArray(_location);
    }

    variablesData_.push_back(CGVariableData(mask, name, _location, isUniform, isInit));
    return true;
}

void CGShaderProgram::removeVariableData(unsigned int mask)
{
    if (mask <= 0)
        return;

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].mask == mask) {
            if (!variablesData_[i].isUniform) {
                disableAttributeArray(variablesData_[i].location);
            }

            variablesData_.remove(i);
            break;
        }
    }
}

bool CGShaderProgram::findVariableData(unsigned int mask) const
{
    if (mask <= 0)
        return false;

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].mask == mask) {
            return true;
        }
    }

    return false;
}

int CGShaderProgram::findVariableData(QString name) const
{
    int _namesCount = 0;

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].name == name) {
            _namesCount++;
        }
    }

    return _namesCount;
}

int CGShaderProgram::variablesDataSize() const
{
    return variablesData_.size();
}

int CGShaderProgram::initializeVariables()
{
    int _errCode = initializeVariables(variablesData_);

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].isInit) {
            if (!variablesData_[i].isUniform) {
                disableAttributeArray(variablesData_[i].location);
            }

            variablesData_.remove(i);
        }
    }

    return _errCode;
}

int CGShaderProgram::refreshVariables()
{
    return refreshVariables(variablesData_);
}

void CGShaderProgram::operator=(const CGShaderProgram& other)
{
    variablesData_ = other.variablesData_;
}

const CGShaderProgram::CGVariableData* CGShaderProgram::getVariableData(unsigned int mask) const
{
    if (mask <= 0)
        return nullptr;

    for (int i = 0; i < variablesData_.size(); ++i) {
        if (variablesData_[i].mask == mask) {
            return &variablesData_[i];
        }
    }

    return nullptr;
}



CGStdShaderProgram::CGStdShaderProgram()
    : matrix_(0, nullptr),
      vector3D_(0, nullptr),
      vector4D_(0, nullptr) {}

CGStdShaderProgram::CGStdShaderProgram(const CGStdShaderProgram& other)
    : matrix_(0, nullptr),
      vector3D_(0, nullptr),
      vector4D_(0, nullptr) {}

CGStdShaderProgram::~CGStdShaderProgram() {}

int CGStdShaderProgram::setMatrix(unsigned int mask, const QMatrix4x4* matrix)
{
    if (mask <= 0)
        return -1;

    matrix_.first = mask;
    matrix_.second = matrix;

    int _errCode = CGShaderProgram::refreshVariables();

    matrix_.first = 0;
    matrix_.second = nullptr;

    return _errCode;
}

int CGStdShaderProgram::setVector(unsigned int mask, const QVector3D* vector)
{
    if (mask <= 0)
        return -1;

    vector3D_.first = mask;
    vector3D_.second = vector;

    int _errCode = CGShaderProgram::refreshVariables();

    vector3D_.first = 0;
    vector3D_.second = nullptr;

    return _errCode;
}

int CGStdShaderProgram::setVector(unsigned int mask, const QVector4D* vector)
{
    if (mask <= 0)
        return -1;

    vector4D_.first = mask;
    vector4D_.second = vector;

    int _errCode = CGShaderProgram::refreshVariables();

    vector4D_.first = 0;
    vector4D_.second = nullptr;

    return _errCode;
}

CGPair<unsigned int, const QMatrix4x4*> CGStdShaderProgram::getMatrix() const
{
    return matrix_;
}

CGPair<unsigned int, const QVector3D*> CGStdShaderProgram::getVector3D() const
{
    return vector3D_;
}

CGPair<unsigned int, const QVector4D*> CGStdShaderProgram::getVector4D() const
{
    return vector4D_;
}

int CGStdShaderProgram::initializeVariables(const QVector<CGVariableData>& variablesData)
{
    for (int i = 0; i < variablesData.size(); ++i) {
        if (variablesData[i].mask == vector4D_.first &&
                vector4D_.first == CG_SHADER_U_COLOR &&
                vector4D_.second) {
            setUniformValue(variablesData[i].location, *vector4D_.second);
            continue;
        }
    }

    return 0;
}

int CGStdShaderProgram::refreshVariables(const QVector<CGVariableData>& variablesData)
{
    for (int i = 0; i < variablesData.size(); ++i) {
        if (variablesData[i].mask == matrix_.first && matrix_.second) {
            setUniformValue(variablesData[i].location, *matrix_.second);
            continue;
        }

        if (variablesData[i].mask == vector3D_.first && vector3D_.second) {
            setUniformValue(variablesData[i].location, *vector3D_.second);
            continue;
        }

        if (variablesData[i].mask == vector4D_.first && vector4D_.second) {
            setUniformValue(variablesData[i].location, *vector4D_.second);
            continue;
        }

        if (variablesData[i].mask == CG_SHADER_A_VERTICES) {
            setAttributeBuffer(variablesData[i].location, GL_FLOAT, 0, 3, sizeof(CGVertex));
            continue;
        }

        if (variablesData[i].mask == CG_SHADER_A_COLORS) {
            setAttributeBuffer(variablesData[i].location, GL_FLOAT, 3, 4, sizeof(CGVertex));
            continue;
        }

        if (variablesData[i].mask == CG_SHADER_A_NORMALS) {
            setAttributeBuffer(variablesData[i].location, GL_FLOAT, 7, 3, sizeof(CGVertex));
            continue;
        }
    }

    return 0;
}
