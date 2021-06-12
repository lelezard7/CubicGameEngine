#include "ShaderProgram.h"
#include <QMessageBox>

#include <iostream>
#include <sstream>


template <typename T>
bool ShaderProgram::setUniform(std::string name, const T* value_0, const T* value_1, const T* value_2, const T* value_3)
{
    if (value_0 == nullptr) {
        QMessageBox t;
        t.setText("value_0 в функции setUniform не должен быть nullptr!");
        return false;
    }

    GLint _location = glGetUniformLocation(ID_, (char*)name.c_str());
    if(_location == -1) {
//        std::stringstream d;
//        d << "uniform переменная с именем " << name << " не найдена!" << std::endl;
        QMessageBox t;
        t.setText("d.str()");
        return false;
    }

    if (std::is_same<T, GLfloat>::value) {
        if (value_1 == nullptr) {
            glUniform1f(_location, (GLfloat)*value_0);
            return true;
        }
        else if (value_2 == nullptr) {
            glUniform2f(_location, (GLfloat)*value_0, (GLfloat)*value_1);
            return true;
        }
        else if (value_3 == nullptr) {
            glUniform3f(_location, (GLfloat)*value_0, (GLfloat)*value_1, (GLfloat)*value_2);
            return true;
        }
        else {
            glUniform4f(_location, (GLfloat)*value_0, (GLfloat)*value_1, (GLfloat)*value_2, (GLfloat)*value_3);
            return true;
        }
    }

    if (std::is_same<T, GLdouble>::value) {
        if (value_1 == nullptr) {
            glUniform1d(_location, (GLdouble)*value_0);
            return true;
        }
        else if (value_2 == nullptr) {
            glUniform2d(_location, (GLdouble)*value_0, (GLdouble)*value_1);
            return true;
        }
        else if (value_3 == nullptr) {
            glUniform3d(_location, (GLdouble)*value_0, (GLdouble)*value_1, (GLdouble)*value_2);
            return true;
        }
        else {
            glUniform4d(_location, (GLdouble)*value_0, (GLdouble)*value_1, (GLdouble)*value_2, (GLdouble)*value_3);
            return true;
        }
    }

    if (std::is_same<T, GLuint>::value) {
        if (value_1 == nullptr) {
            glUniform1ui(_location, (GLuint)*value_0);
            return true;
        }
        else if (value_2 == nullptr) {
            glUniform2ui(_location, (GLuint)*value_0, (GLuint)*value_1);
            return true;
        }
        else if (value_3 == nullptr) {
            glUniform3ui(_location, (GLuint)*value_0, (GLuint)*value_1, (GLuint)*value_2);
            return true;
        }
        else {
            glUniform4ui(_location, (GLuint)*value_0, (GLuint)*value_1, (GLuint)*value_2, (GLuint)*value_3);
            return true;
        }
    }

    if (std::is_same<T, GLint>::value) {
        if (value_1 == nullptr) {
            glUniform1i(_location, (GLint)*value_0);
            return true;
        }
        else if (value_2 == nullptr) {
            glUniform2i(_location, (GLint)*value_0, (GLint)*value_1);
            return true;
        }
        else if (value_3 == nullptr) {
            glUniform3i(_location, (GLint)*value_0, (GLint)*value_1, (GLint)*value_2);
            return true;
        }
        else {
            glUniform4i(_location, (GLint)*value_0, (GLint)*value_1, (GLint)*value_2, (GLint)*value_3);
            return true;
        }
    }

//    std::cout << "Функция glUniform для типа '" << typeid(T).name() << "' не найдена!" << std::endl;
    QMessageBox t;
    t.setText("d.str()");
    return false;
}

template <typename T>
bool ShaderProgram::setUniformV(std::string name, unsigned int size, GLsizei count, const T* value)
{
    GLint _location = glGetUniformLocation(ID_, (char*)name.c_str());
    if(_location == -1) {
//        std::cout << "uniform переменная с именем " << name << " не найдена!" << std::endl;
        QMessageBox t;
        t.setText("d.str()");
        return false;
    }

    if (std::is_same<T, GLfloat>::value) {
        switch (size)
        {
        case 1:    glUniform1fv(_location, count, (GLfloat*)value);    return true;
        case 2:    glUniform2fv(_location, count, (GLfloat*)value);    return true;
        case 3:    glUniform3fv(_location, count, (GLfloat*)value);    return true;
        case 4:    glUniform4fv(_location, count, (GLfloat*)value);    return true;

        default: {
//            std::cout << "Функция glUniform" << size << "fv не найдена!" << std::endl;
            QMessageBox t;
            t.setText("d.str()");
            return false;
        }
        }
    }

//    if (std::is_same<T, GLdouble>::value) {
//        switch (size)
//        {
//        case 1:    glUniform1dv(_location, count, (GLdouble*)value);    return true;
//        case 2:    glUniform2dv(_location, count, (GLdouble*)value);    return true;
//        case 3:    glUniform3dv(_location, count, (GLdouble*)value);    return true;
//        case 4:    glUniform4dv(_location, count, (GLdouble*)value);    return true;

//        default: {
////            std::cout << "Функция glUniform" << size << "dv не найдена!" << std::endl;
//            QMessageBox t;
//            t.setText("d.str()");
//            return false;
//        }
//        }
//    }

//    if (std::is_same<T, GLuint>::value) {
//        switch (size)
//        {
//        case 1:    glUniform1uiv(_location, count, (GLuint*)value);    return true;
//        case 2:    glUniform2uiv(_location, count, (GLuint*)value);    return true;
//        case 3:    glUniform3uiv(_location, count, (GLuint*)value);    return true;
//        case 4:    glUniform4uiv(_location, count, (GLuint*)value);    return true;

//        default: {
////            std::cout << "Функция glUniform" << size << "uiv не найдена!" << std::endl;
//            QMessageBox t;
//            t.setText("d.str()");
//            return false;
//        }
//        }
//    }

    if (std::is_same<T, GLint>::value) {
        switch (size)
        {
        case 1:    glUniform1iv(_location, count, (GLint*)value);    return true;
        case 2:    glUniform2iv(_location, count, (GLint*)value);    return true;
        case 3:    glUniform3iv(_location, count, (GLint*)value);    return true;
        case 4:    glUniform4iv(_location, count, (GLint*)value);    return true;

        default: {
//            std::cout << "Функция glUniform" << size << "iv не найдена!" << std::endl;
            QMessageBox t;
            t.setText("d.str()");
            return false;
        }
        }
    }

//    std::cout << "Функция glUniform для типа '" << typeid(T).name() << "' не найдена!" << std::endl;
    QMessageBox t;
    t.setText("d.str()");
    return false;
}

template <typename T>
bool ShaderProgram::setUniformMatrix(std::string name, std::string size, GLsizei count, GLboolean transpose, const T* value)
{
    GLint _location = glGetUniformLocation(ID_, (char*)name.c_str());
    if(_location == -1) {
//        std::cout << "uniform переменная с именем " << name << " не найдена!" << std::endl;
        QMessageBox t;
        t.setText("d.str()");
        return false;
    }

    if (std::is_same<T, GLfloat>::value) {
        if (size == "2") {
            glUniformMatrix2fv(_location, count, transpose, (GLfloat*)value);
        }
        else if (size == "3") {
            glUniformMatrix3fv(_location, count, transpose, (GLfloat*)value);
        }
        else if (size == "4") {
            glUniformMatrix4fv(_location, count, transpose, (GLfloat*)value);
        }
//        else if (size == "2x3") {
//            glUniformMatrix2x3fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else if (size == "2x4") {
//            glUniformMatrix2x4fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else if (size == "3x2") {
//            glUniformMatrix3x2fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else if (size == "3x4") {
//            glUniformMatrix3x4fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else if (size == "4x2") {
//            glUniformMatrix4x2fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else if (size == "4x3") {
//            glUniformMatrix4x3fv(_location, count, transpose, (GLfloat*)value);
//        }
//        else {
////            std::cout << "Функция glUniformMatrix" << size << "fv не найдена!" << std::endl;
//            QMessageBox t;
//            t.setText("d.str()");
//            return false;
//        }

        return true;
    }

//    if (std::is_same<T, GLdouble>::value) {
//        if (size == "2") {
//            glUniformMatrix2dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "3") {
//            glUniformMatrix3dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "4") {
//            glUniformMatrix4dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "2x3") {
//            glUniformMatrix2x3dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "2x4") {
//            glUniformMatrix2x4dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "3x2") {
//            glUniformMatrix3x2dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "3x4") {
//            glUniformMatrix3x4dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "4x2") {
//            glUniformMatrix4x2dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else if (size == "4x3") {
//            glUniformMatrix4x3dv(_location, count, transpose, (GLdouble*)value);
//        }
//        else {
////            std::cout << "Функция glUniformMatrix" << size << "dv не найдена!" << std::endl;
//            QMessageBox t;
//            t.setText("d.str()");
//            return false;
//        }

//        return true;
//    }

//    std::cout << "Функция glUniformMatrix для типа '" << typeid(T).name() << "' не найдена!" << std::endl;
    QMessageBox t;
    t.setText("d.str()");
    return false;
}
