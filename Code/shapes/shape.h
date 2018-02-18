#ifndef SHAPE_H
#define SHAPE_H

#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>

#include "shapes/multipolygon.h"
#include "shapes/vertex.h"
#include "model.h"

class Shape
{
public:
    Shape();
    Shape(QString filename, float scale);
    Shape(MultiPolygon object);
    void* getBufferData();
    GLuint getBufferSize();
    GLuint numTriangles();
    GLuint* vboPtr();
    GLuint* vaoPtr();

    //QMatrix4x4 transformMatrix;

private:
    GLuint vbo;
    GLuint vao;
    QVector<float> floats;
    QVector<vertex> vertices;
    enum StorageMode : GLuint
        { FLOATS, VERTICES };
    StorageMode storageMode;
};

#endif // SHAPE_H
