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

QVector<float> MultiPolygon::getArrayData() {
    QVector<float> buffer;
    for (int i = 0; i < polygons.size(); i++) {
        buffer.append(polygons[i].toArrayData());
    }
    return buffer;
}
