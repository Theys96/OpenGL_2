#include "shape.h"

Shape::Shape() {}

Shape::Shape(QString filename) {
    Model model = Model(filename);
    model.unitize();
    vertices = model.getVertexStructs();
    storageMode = VERTICES;
}

Shape::Shape(QString filename, float scale) {
    Model model = Model(filename, scale);
    vertices = model.getVertexStructs();
    storageMode = VERTICES;
}

Shape::Shape(MultiPolygon object) {
    floats = object.getArrayVector();
    storageMode = FLOATS;
}

void* Shape::getBufferData() {
    if (storageMode == VERTICES) {
        return vertices.data();
    }
    if (storageMode == FLOATS) {
        return floats.data();
    }
    return NULL;
}

GLuint Shape::getBufferSize() {
    if (storageMode == VERTICES) {
        return vertices.length() * sizeof(vertex);
    }
    if (storageMode == FLOATS) {
        return floats.length() * sizeof(float);
    }
    return 0;
}

GLuint Shape::numTriangles() {
    if (storageMode == VERTICES) {
        return vertices.length() * 3;
    }
    if (storageMode == FLOATS) {
        return floats.length() * 3 * (sizeof(vertex)/sizeof(float));
    }
    return 0;
}

