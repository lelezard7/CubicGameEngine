#ifndef _SHADER_PROGRAM_
#define _SHADER_PROGRAM_

#include <QOpenGLFunctions>
#include <string>


class ShaderProgram : public QOpenGLFunctions
{
    GLuint ID_;

    std::string vertexShaderPath_;
    std::string fragmentShaderPath_;

    std::string vertexShaderCode_;
    std::string fragmentShaderCode_;

    bool setShadersFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath);
    void clear();

public:

    ShaderProgram();
    ShaderProgram(std::string vertexShader, std::string fragmentShader, bool isFiles);
    ShaderProgram(const ShaderProgram& other);
    ~ShaderProgram();

    std::string getVertexShaderPath();
    std::string getFragmentShaderPath();

    std::string getVertexShaderCode();
    std::string getFragmentShaderCode();

    GLuint getId();

    bool create(std::string vertexShader, std::string fragmentShader, bool isFiles);
    void del();

    bool use();
    bool isCreated();

    template <typename T>
    bool setUniform(std::string name, const T* value_0, const T* value_1 = nullptr, const T* value_2 = nullptr, const T* value_3 = nullptr);

    template <typename T>
    bool setUniformV(std::string name, unsigned int size, GLsizei count, const T* value);

    template <typename T>
    bool setUniformMatrix(std::string name, std::string size, GLsizei count, GLboolean transpose, const T* value);

};

#include "ShaderProgram.inl"

#endif
