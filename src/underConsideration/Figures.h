#ifndef _FIGURES_
#define _FIGURES_

//#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
//#include <QOpenGLWidget>
//#include <QOpenGLContext>

#include "glm.hpp"
#include "detail/qualifier.hpp"

#include <vector>


template <typename T, GLuint __Vertices, GLuint __Indices, GLuint __Step>
class Figure : public QOpenGLExtraFunctions
{
protected:
    std::vector<T> vertices_;
    std::vector<GLuint> indices_;
    std::vector<T> normals_;

    glm::vec<3, T, glm::defaultp> color_;

    GLuint vbo_;
    GLuint vao_;
    GLuint ebo_;

    bool applyData();

public:
    Figure();
    Figure(const Figure& other);
    ~Figure();

    GLuint getVbo();
    GLuint getVao();
    GLuint getEbo();

    void getVertices(GLuint size, T* buffer);
    void getIndices(GLuint size, GLuint* buffer);

    GLuint getVerticesCount();
    GLuint getIndicesCount();

    void setColor(T r, T g, T b);
    glm::vec<3, T, glm::defaultp> getColor();

    void bind();
    void draw();

    void clear();

    void operator=(const Figure& other);

};


template <typename T>
class Cube : public Figure<T, 24, 36, 3>
{
    void createCube(T x, T y, T z, T width, T height, T depth);

public:
    void create();
    void create(T size);
    void create(T width, T height, T depth);
    void create(T x, T y, T z, T size);
    void create(T x, T y, T z, T width, T height, T depth);

};

#include "Figures.inl"

#endif
