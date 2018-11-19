#ifndef geometry_hpp
#define geometry_hpp

#define V3_COORD_8(i) (v3(X_COORD_8((int)(i)), Y_COORD_8((int)(i)), Z_COORD_8((int)(i))))

struct v3{
    v3(double x, double y, double z): x(x), y(y), z(z){}
    double x, y, z;
};

double dot(v3 a, v3 b);
v3 cross(v3 a, v3 b);
double length(v3 a);
double length2(v3 a);
v3 normalize(v3 a);

v3 operator-(const v3 a, const v3 b);
v3 operator-(const v3 a, const double b);
v3 operator-(const double a, const v3 b);
v3 operator+(const v3 a, const v3 b);
v3 operator+(const v3 a, const double b);
v3 operator+(const double a, const v3 b);

v3 operator*(const double a, const v3 b);
v3 operator*(const v3 a, const double b);
v3 operator/(const double a, const v3 b);
v3 operator/(const v3 a, const double b);


#endif