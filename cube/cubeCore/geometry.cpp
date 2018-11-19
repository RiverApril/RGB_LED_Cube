

#include "geometry.hpp"

#include <math.h>

double dot(v3 a, v3 b){
    return (b.x*a.x)+(b.y*a.y)+(b.z*a.z);
}

v3 cross(v3 a, v3 b){
    return v3(a.y*b.z - a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y - a.y*b.x);
}

double length(v3 a){
    return sqrt(length2(a));
}

double length2(v3 a){
    return a.x*a.x + a.y*a.y + a.z*a.z;
}

v3 normalize(v3 a){
    return a/length(a);
}

v3 operator-(const v3 a, const v3 b){
    return v3(a.x-b.x, a.y-b.y, a.z-b.z);
}

v3 operator-(const v3 a, const double b){
    return v3(a.x-b, a.y-b, a.z-b);
}

v3 operator-(const double a, const v3 b){
    return v3(a-b.x, a-b.y, a-b.z);
}

v3 operator+(const v3 a, const v3 b){
    return v3(a.x+b.x, a.y+b.y, a.z+b.z);
}

v3 operator+(const v3 a, const double b){
    return v3(a.x+b, a.y+b, a.z+b);
}

v3 operator+(const double a, const v3 b){
    return v3(a+b.x, a+b.y, a+b.z);
}

v3 operator*(const double a, const v3 b){
    return v3(a*b.x, a*b.y, a*b.z);
}

v3 operator*(const v3 a, const double b){
    return v3(a.x*b, a.y*b, a.z*b);
}

v3 operator/(const double a, const v3 b){
    return v3(a/b.x, a/b.y, a/b.z);
}

v3 operator/(const v3 a, const double b){
    return v3(a.x/b, a.y/b, a.z/b);
}