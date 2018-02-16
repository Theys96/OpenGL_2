#include "polygon.h"

Polygon::Polygon() {}

Polygon::Polygon(vertex v1, vertex v2, vertex v3) : v1(v1), v2(v2), v3(v3)
{

}

QVector<float> Polygon::toArrayData() {
    QVector<float> buffer;

    for (int i = 0; i < 3; i++) {
        buffer.append(v1.x);
        buffer.append(v1.y);
        buffer.append(v1.z);
        buffer.append(v1.r);
        buffer.append(v1.g);
        buffer.append(v1.b);
    }

    return buffer;
}
