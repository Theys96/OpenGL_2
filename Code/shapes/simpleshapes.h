#ifndef SIMPLESHAPES_H
#define SIMPLESHAPES_H

#include "shapes/shape.h"

class SimpleShapes
{
public:
    static Shape getCube();
    static Shape getPyramid();

private:
    SimpleShapes();
};

#endif // SIMPLESHAPES_H
