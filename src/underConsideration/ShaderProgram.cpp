#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>





ShaderProgram::ShaderProgram()
{
    initializeOpenGLFunctions();

    ID_ = 0;

    vertexShaderPath_ = "";
    fragmentShaderPath_ = "";

    vertexShaderCode_ = "";
    fragmentShaderCode_ = "";
}

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader, bool isFiles)
{
    initializeOpenGLFunctions();

    create(vertexShader, fragmentShader, isFiles);
}

ShaderProgram::ShaderProgram(const ShaderProgram& other)
{
    initializeOpenGLFunctions();

    if (other.vertexShaderPath_ == "" || other.fragmentShaderPath_ == "") {
        create(other.vertexShaderCode_, other.fragmentShaderCode_, false);
    }
    else {
        create(other.vertexShaderPath_, other.fragmentShaderPath_, true);
    }
}

ShaderProgram::~ShaderProgram()
{
    del();
}

bool ShaderProgram::setShadersFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    std::ifstream _vertexShader;
    std::ifstream _fragmentShader;

    _vertexShader.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    _fragmentShader.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        _vertexShader.open(vertexShaderPath);
        _fragmentShader.open(fragmentShaderPath);

        std::stringstream _vertexShaderCodeStream;
        std::stringstream _fragmentShaderCodeStream;

        _vertexShaderCodeStream << _vertexShader.rdbuf();
        _fragmentShaderCodeStream << _fragmentShader.rdbuf();

        _vertexShader.close();
        _fragmentShader.close();

        vertexShaderPath_   = vertexShaderPath;
        fragmentShaderPath_ = fragmentShaderPath;
        vertexShaderCode_   = _vertexShaderCodeStream.str();
        fragmentShaderCode_ = _fragmentShaderCodeStream.str();

        return true;
    }
    catch (const std::ifstream::failure& ex)
    {
//        std::cout << "Ошибка открытия файла:" << std::endl;
//        std::cout << " Code: " << ex.code() << std::endl;
//        std::cout << " What: " << ex.what() << std::endl;
        QMessageBox t;
        t.setText("d.str()");

        return false;
    }
}

void ShaderProgram::clear()
{
    ID_ = 0;

    vertexShaderPath_.clear();
    fragmentShaderPath_.clear();

    vertexShaderCode_.clear();
    fragmentShaderCode_.clear();
}

std::string ShaderProgram::getVertexShaderPath()
{
    return vertexShaderPath_;
}

std::string ShaderProgram::getFragmentShaderPath()
{
    return fragmentShaderPath_;
}

std::string ShaderProgram::getVertexShaderCode()
{
    return vertexShaderCode_;
}

std::string ShaderProgram::getFragmentShaderCode()
{
    return fragmentShaderCode_;
}

GLuint ShaderProgram::getId()
{
    return ID_;
}

bool ShaderProgram::create(std::string vertexShader, std::string fragmentShader, bool isFiles)
{


    if (ID_ != 0) {
//        std::cout << "Шейдерная программа уже задана! Перед повторным вызовом функции create "
//                        "удалите уже существующую шейдерную программу вызовом функции del." << std::endl;
        QMessageBox t;
        t.setText("d.str()");
        return false;
    }

    if (isFiles) {
        if (!setShadersFromFiles(vertexShader, fragmentShader)) {
            clear();
            return false;
        }
    }
    else {
        vertexShaderCode_ = vertexShader;
        fragmentShaderCode_ = fragmentShader;
    }

    const GLchar* _code;
    GLint _errCode;
    GLchar* _log;
    GLsizei _LogLength;

    GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    _code = vertexShaderCode_.c_str();
    glShaderSource(_vertexShader, 1, &_code, NULL);
    glCompileShader(_vertexShader);

    GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    _code = fragmentShaderCode_.c_str();
    glShaderSource(_fragmentShader, 1, &_code, NULL);
    glCompileShader(_fragmentShader);

    glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &_errCode);
    if (!_errCode) {
        glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &_LogLength);
        _log = new GLchar[_LogLength];

        glGetShaderInfoLog(_vertexShader, _LogLength, NULL, _log);
//        std::cout << "Ошибка компиляции вершинного шейдера:" << std::endl;
//        std::cout << " " << _log << std::endl;
        QMessageBox t;
        t.setText("d.str()");

        delete[] _log;
        _log = nullptr;
        clear();

        return false;
    }

    glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &_errCode);
    if (!_errCode) {
        glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &_LogLength);
        _log = new GLchar[_LogLength];

        glGetShaderInfoLog(_fragmentShader, _LogLength, NULL, _log);
//        std::cout << "Ошибка компиляции фрагментного шейдера:" << std::endl;
//        std::cout << " " << _log << std::endl;
        QMessageBox t;
        t.setText("d.str()");

        delete[] _log;
        _log = nullptr;
        clear();

        return false;
    }

    ID_ = glCreateProgram();
    glAttachShader(ID_, _vertexShader);
    glAttachShader(ID_, _fragmentShader);
    glLinkProgram(ID_);

    glGetProgramiv(ID_, GL_LINK_STATUS, &_errCode);
    if (!_errCode) {
        glGetProgramiv(ID_, GL_INFO_LOG_LENGTH, &_LogLength);
        _log = new GLchar[_LogLength];

        glGetProgramInfoLog(ID_, _LogLength, NULL, _log);
//        std::cout << "Ошибка связывания шейдерной программы:" << std::endl;
//        std::cout << " " << _log << std::endl;
        QMessageBox t;
        t.setText("d.str()");

        delete[] _log;
        _log = nullptr;
        clear();

        return false;
    }

    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);

    return true;
}

void ShaderProgram::del()
{
    glDeleteProgram(ID_);
    clear();
}

bool ShaderProgram::use()
{
    if (ID_ == 0) {
//        std::cout << "Прежде чем использовать шейдерную программу ее нужно создать!" << std::endl;
        QMessageBox t;
        t.setText("d.str()");
        return false;
    }

    glUseProgram(ID_);

    return  true;
}

bool ShaderProgram::isCreated()
{
    if (ID_ == 0)
        return false;

    return true;
}
