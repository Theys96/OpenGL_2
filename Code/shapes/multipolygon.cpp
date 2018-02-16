#include "multipolygon.h"

MultiPolygon::MultiPolygon(int numPoints) : numPoints(numPoints)
{
    points = new vertex[numPoints];
}

void MultiPolygon::setPoint(int i, float x, float y, float z) {
    points[i] = {x, y, z};
}

void MultiPolygon::addPolygon(int i1, int i2, int i3) {
    Polygon polygon = Polygon(points[i1], points[i2], points[i3]);
    polygons.append(polygon);
}

void MultiPolygon::addFace(int i1, int i2, int i3, int i4) {
    Polygon polygon1 = Polygon(points[i1], points[i2], points[i3]);
    Polygon polygon2 = Polygon(points[i3], points[i4], points[i1]);
    polygons.append(polygon1);
    polygons.append(polygon2);
}

QVector<float> MultiPolygon::getArrayVector() {
    QVector<float> buffer;
    for (int i = 0; i < polygons.size(); i++) {
        buffer.append(polygons[i].toArrayData());
    }
    return buffer;
}

int MultiPolygon::numFloats() {
    return polygons.size()*3*6;
}
