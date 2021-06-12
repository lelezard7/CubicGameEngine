#include "Figures.h"
#include <QMessageBox>

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
bool
Figure<T, __Vertices, __Indices, __Step>::
applyData()
{
    if (__Vertices == 0 || __Indices == 0 || __Step == 0) {
        QMessageBox t;
        t.setText("Количество вершин, их индексы или количество компонентов на общий атрибут вершины равно 0!");
        return false;
    }

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ebo_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    T buffer_1[__Vertices];
    for (GLuint i = 0; i < __Vertices; ++i) {
        buffer_1[i] = vertices_[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_1), &buffer_1, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, __Step, GL_FLOAT, GL_FALSE, __Step * sizeof(T), (void*)0);
    glEnableVertexAttribArray(0);
    
    GLuint buffer_2[__Indices];
    for (GLuint i = 0; i < __Indices; ++i) {
        buffer_2[i] = indices_[i];
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(buffer_2), &buffer_2, GL_STATIC_DRAW);

    glBindVertexArray(0);
    return true;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
Figure<T, __Vertices, __Indices, __Step>::
Figure()
{
    initializeOpenGLFunctions();

    vertices_.resize(__Vertices);
    indices_.resize(__Indices);

    color_ = glm::vec<3, T, glm::defaultp>((T)1);

    vbo_ = 0;
    vao_ = 0;
    ebo_ = 0;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
Figure<T, __Vertices, __Indices, __Step>::
Figure(const Figure& other)
{
    initializeOpenGLFunctions();

    vbo_ = 0;
    vao_ = 0;
    ebo_ = 0;

    color_ = other.color_;
    vertices_ = other.vertices_;
    indices_ = other.indices_;

    applyData();
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
Figure<T, __Vertices, __Indices, __Step>::
~Figure()
{
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);   //TODO: Доработать!
    glDeleteBuffers(1, &vbo_);

    vertices_.clear();
    indices_.clear();
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
GLuint
Figure<T, __Vertices, __Indices, __Step>::
getVbo()
{
    return vbo_;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
GLuint
Figure<T, __Vertices, __Indices, __Step>::
getVao()
{
    return vao_;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
GLuint
Figure<T, __Vertices, __Indices, __Step>::
getEbo()
{
    return ebo_;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
getVertices(GLuint size, T* buffer)
{
    if (!buffer)
        return;

    for (GLuint i = 0; i < __Vertices && i < size; ++i) {
        buffer[i] = vertices_[i];
    }
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
getIndices(GLuint size, GLuint* buffer)
{
    if (!buffer)
        return;

    for (GLuint i = 0; i < __Indices && i < size; ++i) {
        buffer[i] = indices_[i];
    }
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
GLuint
Figure<T, __Vertices, __Indices, __Step>::
getVerticesCount()
{
    return __Vertices;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
GLuint
Figure<T, __Vertices, __Indices, __Step>::
getIndicesCount()
{
    return __Indices;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
setColor(T r, T g, T b)
{
    color_ = glm::vec<3, T, glm::defaultp>(r, g, b);
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
glm::vec<3, T, glm::defaultp>
Figure<T, __Vertices, __Indices, __Step>::
getColor()
{
    return color_;
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
bind()
{
    glBindVertexArray(vao_);
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
draw()
{
    if (vao_ == 0)
        return;

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, __Indices, GL_UNSIGNED_INT, nullptr);
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
clear()
{
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);

    vbo_ = 0;
    vao_ = 0;
    ebo_ = 0;

    color_ = glm::vec<3, T, glm::defaultp>((T)1);

    vertices_.clear();
    indices_.clear();

    vertices_.resize(__Vertices);
    indices_.resize(__Indices);
}

template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
void
Figure<T, __Vertices, __Indices, __Step>::
operator=(const Figure& other)
{
    clear();

    color_ = other.color_;
    vertices_ = other.vertices_;
    indices_ = other.indices_;

    setData();
}


template <typename T>
void Cube<T>::createCube(T x, T y, T z, T width, T height, T depth)
{
    this->vertices_[0]  = x;         this->vertices_[1]  = y;          this->vertices_[2]  = z;         // 0) LBF
    this->vertices_[3]  = x;         this->vertices_[4]  = y + height; this->vertices_[5]  = z;         // 1) LTF
    this->vertices_[6]  = x + width; this->vertices_[7]  = y + height; this->vertices_[8]  = z;         // 2) RTF
    this->vertices_[9]  = x + width; this->vertices_[10] = y;          this->vertices_[11] = z;         // 3) RBF

    this->vertices_[12] = x;         this->vertices_[13] = y;          this->vertices_[14] = z - depth; // 4) LBB
    this->vertices_[15] = x;         this->vertices_[16] = y + height; this->vertices_[17] = z - depth; // 5) LTB
    this->vertices_[18] = x + width; this->vertices_[19] = y + height; this->vertices_[20] = z - depth; // 6) RTB
    this->vertices_[21] = x + width; this->vertices_[22] = y;          this->vertices_[23] = z - depth; // 7) RBB

    //Front
    this->indices_[0] = 0;  this->indices_[1] = 1;  this->indices_[2] = 2;
    this->indices_[3] = 0;  this->indices_[4] = 2;  this->indices_[5] = 3;

    //Back
    this->indices_[6] = 4;  this->indices_[7]  = 5; this->indices_[8]  = 6;
    this->indices_[9] = 4;  this->indices_[10] = 5; this->indices_[11] = 7;

    //Top
    this->indices_[12] = 1; this->indices_[13] = 2; this->indices_[14] = 5;
    this->indices_[15] = 5; this->indices_[16] = 6; this->indices_[17] = 2;

    //Bottom
    this->indices_[18] = 0; this->indices_[19] = 3; this->indices_[20] = 4;
    this->indices_[21] = 4; this->indices_[22] = 7; this->indices_[23] = 3;

    //Left
    this->indices_[24] = 0; this->indices_[25] = 1; this->indices_[26] = 5;
    this->indices_[27] = 4; this->indices_[28] = 5; this->indices_[29] = 0;

    //Right
    this->indices_[30] = 2; this->indices_[31] = 3; this->indices_[32] = 7;
    this->indices_[33] = 6; this->indices_[34] = 7; this->indices_[35] = 2;


//    //Front
//    this->normals_[0] = 0; this->normals_[0] = 0; this->normals_[0] =  1;
//    //Back
//    this->normals_[0] = 0; this->normals_[0] = 0; this->normals_[0] = -1;

//    //Top
//    this->normals_[0] = 0; this->normals_[0] =  1; this->normals_[0] = 0;
//    //Bottom
//    this->normals_[0] = 0; this->normals_[0] = -1; this->normals_[0] = 0;

//    //Left
//    this->normals_[0] = -1; this->normals_[0] = 0; this->normals_[0] = 0;
//    //Right
//    this->normals_[0] =  1; this->normals_[0] = 0; this->normals_[0] = 0;

    applyData();
}

template <typename T>
void Cube<T>::create()
{
    createCube(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f);
}

template <typename T>
void Cube<T>::create(T size)
{
    createCube(-(size / 2), -(size / 2), size / 2, size, size, size);
}

template <typename T>
void Cube<T>::create(T width, T height, T depth)
{
    createCube(-(width / 2), -(height / 2), depth / 2, width, height, depth);
}

template <typename T>
void Cube<T>::create(T x, T y, T z, T size)
{
    createCube(x, y, z, size, size, size);
}

template <typename T>
void Cube<T>::create(T x, T y, T z, T width, T height, T depth)
{
    createCube(x, y, z, width, height, depth);
}
