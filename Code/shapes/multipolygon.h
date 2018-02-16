#ifndef MULTIPOLYGON_H
#define MULTIPOLYGON_H

#include "polygon.h"
#include "vertex.h"
#include <QVector>

class MultiPolygon
{
public:
    MultiPolygon(int numPoints);
    void setPoint(int i, float x, float y, float z);
    void addPolygon(int i1, int i2, int i3);
    QVector<float> getArrayData();

private:
    int numPoints;
    QVector<Polygon> polygons;
    vertex* points;
};

#endif // MULTIPOLYGON_H
