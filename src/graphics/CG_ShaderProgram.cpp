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

CGShaderProgram::~CGShaderProgram() {}

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

const CGShaderProgram& CGShaderProgram::operator=(const CGShaderProgram& other)
{
    variablesData_ = other.variablesData_;

    return *this;
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



CGStdShaderProgram::Storage::Storage()
    : matrix_(0, nullptr),
      vector3D_(0, nullptr),
      vector4D_(0, nullptr) {}

CGStdShaderProgram::Storage::Storage(const CGStdShaderProgram::Storage& other)
    : matrix_(other.matrix_),
      vector3D_(other.vector3D_),
      vector4D_(other.vector4D_) {}

CGStdShaderProgram::CGStdShaderProgram() : CGShaderProgram() {}

CGStdShaderProgram::CGStdShaderProgram(const CGStdShaderProgram& other) : CGShaderProgram(other)
{
    permanentStorage_ = other.permanentStorage_;
}

CGStdShaderProgram::~CGStdShaderProgram() {}

int CGStdShaderProgram::setMatrix(unsigned int mask, const QMatrix4x4* matrix, StorageType storageType)
{
    if (mask <= 0 || !matrix)
        return -1;

    if (storageType == PERMANENT_STORAGE) {
        permanentStorage_.matrix_.first = mask;
        permanentStorage_.matrix_.second = matrix;
        return 0;
    }

    shortTermStorage_.matrix_.first = mask;
    shortTermStorage_.matrix_.second = matrix;

    int _errCode = CGShaderProgram::refreshVariables();

    shortTermStorage_.matrix_.first = 0;
    shortTermStorage_.matrix_.second = nullptr;

    return _errCode;
}

int CGStdShaderProgram::setVector(unsigned int mask, const QVector3D* vector, StorageType storageType)
{
    if (mask <= 0 || !vector)
        return -1;

    if (storageType == PERMANENT_STORAGE) {
        permanentStorage_.vector3D_.first = mask;
        permanentStorage_.vector3D_.second = vector;
        return 0;
    }

    shortTermStorage_.vector3D_.first = mask;
    shortTermStorage_.vector3D_.second = vector;

    int _errCode = CGShaderProgram::refreshVariables();

    shortTermStorage_.vector3D_.first = 0;
    shortTermStorage_.vector3D_.second = nullptr;

    return _errCode;
}

int CGStdShaderProgram::setVector(unsigned int mask, const QVector4D* vector, StorageType storageType)
{
    if (mask <= 0 || !vector)
        return -1;

    if (storageType == PERMANENT_STORAGE) {
        permanentStorage_.vector4D_.first = mask;
        permanentStorage_.vector4D_.second = vector;
        return 0;
    }

    shortTermStorage_.vector4D_.first = mask;
    shortTermStorage_.vector4D_.second = vector;

    int _errCode = CGShaderProgram::refreshVariables();

    shortTermStorage_.vector4D_.first = 0;
    shortTermStorage_.vector4D_.second = nullptr;

    return _errCode;
}

CGStdShaderProgram::AnonMatrix CGStdShaderProgram::getMatrix(StorageType storageType) const
{
    if (storageType == SHORT_TERM_STORAGE)
        return shortTermStorage_.matrix_;

    return permanentStorage_.matrix_;
}

CGStdShaderProgram::AnonVector3D CGStdShaderProgram::getVector3D(StorageType storageType) const
{
    if (storageType == SHORT_TERM_STORAGE)
        return shortTermStorage_.vector3D_;

    return permanentStorage_.vector3D_;
}

CGStdShaderProgram::AnonVector4D CGStdShaderProgram::getVector4D(StorageType storageType) const
{
    if (storageType == SHORT_TERM_STORAGE)
        return shortTermStorage_.vector4D_;

    return permanentStorage_.vector4D_;
}

void CGStdShaderProgram::removePermanentMatrix()
{
    permanentStorage_.matrix_.first = 0;
    permanentStorage_.matrix_.second = nullptr;
}

void CGStdShaderProgram::removePermanentVector3D()
{
    permanentStorage_.vector3D_.first = 0;
    permanentStorage_.vector3D_.second = nullptr;
}

void CGStdShaderProgram::removePermanentVector4D()
{
    permanentStorage_.vector4D_.first = 0;
    permanentStorage_.vector4D_.second = nullptr;
}

int CGStdShaderProgram::initializeVariables(const QVector<CGVariableData>& variablesData)
{
    for (int i = 0; i < variablesData.size(); ++i) {
        if (variablesData[i].mask == permanentStorage_.vector4D_.first &&
                permanentStorage_.vector4D_.first == CG_SHADER_U_COLOR &&
                permanentStorage_.vector4D_.second) {

            setUniformValue(variablesData[i].location, *permanentStorage_.vector4D_.second);
            removePermanentVector4D();
            continue;
        }
    }

    return 0;
}

int CGStdShaderProgram::refreshVariables(const QVector<CGVariableData>& variablesData)
{
    for (int i = 0; i < variablesData.size(); ++i) {
        if (variablesData[i].mask == shortTermStorage_.matrix_.first &&
                shortTermStorage_.matrix_.second) {
            setUniformValue(variablesData[i].location, *shortTermStorage_.matrix_.second);
            continue;
        }

        if (variablesData[i].mask == shortTermStorage_.vector3D_.first &&
                shortTermStorage_.vector3D_.second) {
            setUniformValue(variablesData[i].location, *shortTermStorage_.vector3D_.second);
            continue;
        }

        if (variablesData[i].mask == shortTermStorage_.vector4D_.first &&
                shortTermStorage_.vector4D_.second) {
            setUniformValue(variablesData[i].location, *shortTermStorage_.vector4D_.second);
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


