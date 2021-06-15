#ifndef _CG_VERTEX_
#define _CG_VERTEX_

#include <QVector3D>
#include <QVector4D>


class CGVertex
{
public:
    CGVertex();
    CGVertex(const CGVertex& other);

    CGVertex(const QVector3D& position);
    CGVertex(const QVector3D& position, const QVector4D& color);
    CGVertex(const QVector3D& position, const QVector4D& color, const QVector3D& normal);

    QVector3D position;
    QVector4D color;
    QVector3D normal;

    void operator=(const CGVertex& other);

    bool operator==(const CGVertex& other) const;
    bool operator!=(const CGVertex& other) const;
};

#endif
