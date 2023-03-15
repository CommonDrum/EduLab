//
// Created by Karol Hetman on 11/03/2023.
//

#ifndef EDULAB_UTILITY_H
#define EDULAB_UTILITY_H

// Implemented to make the scene more compatible
struct Color {
    float r, g, b;
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

// implement 2d vector for int and float
template<typename T>
class Vector2D {
public:
    T x, y;

    Vector2D(T x, T y) : x(x), y(y) {}

    Vector2D<T> operator+(const Vector2D<T>& other) const {
        return Vector2D<T>(x + other.x, y + other.y);
    }

    Vector2D<T> operator-(const Vector2D<T>& other) const {
        return Vector2D<T>(x - other.x, y - other.y);
    }

    Vector2D<T> operator*(const T& scalar) const {
        return Vector2D<T>(x * scalar, y * scalar);
    }

    T dot(const Vector2D<T>& other) const {
        return x * other.x + y * other.y;
    }

    T magnitude() const {
        return sqrt(x * x + y * y);
    }

    Vector2D<T> normalized() const {
        T mag = magnitude();
        return Vector2D<T>(x / mag, y / mag);
    }
};


#endif //EDULAB_UTILITY_H
