#ifndef POLYGON_H
#define POLYGON_H

#include "vertex.h"
#include <QVector>

class Polygon
{
public:
    Polygon();
    Polygon(vertex v1, vertex v2, vertex v3);
    QVector<float> toArrayData();

private:
    vertex v1;
    vertex v2;
    vertex v3;
};

#endif // POLYGON_H
