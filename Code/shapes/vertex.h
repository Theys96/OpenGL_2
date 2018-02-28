#ifndef VERTEX
#define VERTEX

struct vertex {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;

    vertex() = default;
    vertex(float x, float y, float z, float nx, float ny, float nz)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz)
    {}
    vertex(float x, float y, float z)
        : x(x), y(y), z(z), nx(0), ny(0), nz(0)
    {}

};


#endif // VERTEX
