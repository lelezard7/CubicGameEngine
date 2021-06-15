#include "CG_Vertex.h"


CGVertex::CGVertex() {}

CGVertex::CGVertex(const CGVertex& other)
    : position(other.position),
      color(other.color),
      normal(other.normal) {}

CGVertex::CGVertex(const QVector3D& position)
    : position(position),
      color(0.0f, 0.0f, 0.0f, 1.0f),
      normal(0.0f, 0.0f, 0.0f) {}

CGVertex::CGVertex(const QVector3D& position, const QVector4D& color)
    : position(position),
      color(color),
      normal(0.0f, 0.0f, 0.0f) {}

CGVertex::CGVertex(const QVector3D& position, const QVector4D& color, const QVector3D& normal)
    : position(position),
      color(color),
      normal(normal) {}

void CGVertex::operator=(const CGVertex& other)
{
    position = other.position;
    color = other.color;
    normal = other.normal;
}

bool CGVertex::operator==(const CGVertex& other) const
{
    return position == other.position && color == other.color && normal == other.normal;
}

bool CGVertex::operator!=(const CGVertex& other) const
{
    return position != other.position || color != other.color || normal != other.normal;
}
