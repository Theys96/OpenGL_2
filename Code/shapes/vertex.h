#ifndef VERTEX
#define VERTEX

struct vertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;

    vertex() = default;
    vertex(float x, float y, float z, float r, float g, float b)
        : x(x), y(y), z(z), r(r), g(g), b(b)
    {}
    vertex(float x, float y, float z)
        : x(x), y(y), z(z), r((x+1)/2), g((y+1)/2), b((z+1)/2)
    {}

};


#endif // VERTEX
