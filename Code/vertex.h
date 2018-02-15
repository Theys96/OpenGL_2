#ifndef VERTEX
#define VERTEX

#endif // VERTEX

struct vertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;

    vertex() = default;
    vertex(float x, float y, float z, float r, float g, float b)
        :
        x(x),
        y(y),
        z(z),
        r(r),
        g(g),
        b(b)
    {
    }

};
