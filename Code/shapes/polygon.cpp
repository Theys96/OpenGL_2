#include "polygon.h"

Polygon::Polygon() {}

Polygon::Polygon(vertex v1, vertex v2, vertex v3) : v1(v1), v2(v2), v3(v3)
{

}

QVector<float> Polygon::toArrayData() {
    QVector<float> buffer;

    buffer.append(v1.x);
    buffer.append(v1.y);
    buffer.append(v1.z);
    buffer.append(v1.r);
    buffer.append(v1.g);
    buffer.append(v1.b);
    buffer.append(v2.x);
    buffer.append(v2.y);
    buffer.append(v2.z);
    buffer.append(v2.r);
    buffer.append(v2.g);
    buffer.append(v2.b);
    buffer.append(v3.x);
    buffer.append(v3.y);
    buffer.append(v3.z);
    buffer.append(v3.r);
    buffer.append(v3.g);
    buffer.append(v3.b);

    return buffer;
}
