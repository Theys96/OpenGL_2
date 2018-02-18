#include "simpleshapes.h"
#include "shapes/multipolygon.h"

SimpleShapes::SimpleShapes() {}

Shape SimpleShapes::getCube() {
    MultiPolygon cube = MultiPolygon(8);
    cube.setPoint(0, -1, -1, 1);
    cube.setPoint(1, 1, -1, 1);
    cube.setPoint(2, 1, -1, -1);
    cube.setPoint(3, -1, -1, -1);
    cube.setPoint(4, -1, 1, 1);
    cube.setPoint(5, 1, 1, 1);
    cube.setPoint(6, 1, 1, -1);
    cube.setPoint(7, -1, 1, -1);
    cube.addFace(0, 3, 2, 1);
    cube.addFace(0, 1, 5, 4);
    cube.addFace(7, 4, 5, 6);
    cube.addFace(7, 6, 2, 3);
    cube.addFace(7, 3, 0, 4);
    cube.addFace(5, 1, 2, 6);
    Shape cubeShape = Shape(cube);
    return cubeShape;
}

Shape SimpleShapes::getPyramid() {
    MultiPolygon pyramid = MultiPolygon(5);
    pyramid.setPoint(0, -1, -1, 1);
    pyramid.setPoint(1, 1, -1, 1);
    pyramid.setPoint(2, 1, -1, -1);
    pyramid.setPoint(3, -1, -1, -1);
    pyramid.setPoint(4, 0, 1, 0);
    pyramid.addFace(0, 3, 2, 1);
    pyramid.addPolygon(0, 1, 4);
    pyramid.addPolygon(1, 2, 4);
    pyramid.addPolygon(2, 3, 4);
    pyramid.addPolygon(3, 0, 4);
    Shape pyramidShape = Shape(pyramid);
    return pyramidShape;
}
