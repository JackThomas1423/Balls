#include "renderer.hpp"

class Vec2 {
    float x, y;
    Vec2(float _x, float _y) : x(_x), y(_y) {};
    Vec2(const Vec2& vec) : x(vec.x), y(vec.y) {};
    Vec2() : x(0.0f), y(0.0f) {}
};

class Object {
    public:
    Vec2 position;
    Renderer renderer;
    Object();
    void translate(Vec2 translation);
    void rotate();
};