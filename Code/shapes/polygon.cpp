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
    buffer.append(v1.nx);
    buffer.append(v1.ny);
    buffer.append(v1.nz);
    buffer.append(v2.x);
    buffer.append(v2.y);
    buffer.append(v2.z);
    buffer.append(v2.nx);
    buffer.append(v2.ny);
    buffer.append(v2.nz);
    buffer.append(v3.x);
    buffer.append(v3.y);
    buffer.append(v3.z);
    buffer.append(v3.nx);
    buffer.append(v3.ny);
    buffer.append(v3.nz);

    return buffer;
}
