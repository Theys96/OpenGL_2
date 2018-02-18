#ifndef SHAPE_H
#define SHAPE_H

#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>

#include "shapes/multipolygon.h"
#include "shapes/vertex.h"
#include "shapes/model.h"

class Shape
{
public:
    Shape();
    Shape(QString filename);
    Shape(QString filename, float scale);
    Shape(MultiPolygon object);
    void* getBufferData();
    GLuint getBufferSize();
    GLuint numTriangles();
    GLuint vbo;
    GLuint vao;

    QMatrix4x4 transformation;

private:
    QVector<float> floats;
    QVector<vertex> vertices;
    enum StorageMode : GLuint
        { FLOATS, VERTICES };
    StorageMode storageMode;
};

#endif // SHAPE_H
