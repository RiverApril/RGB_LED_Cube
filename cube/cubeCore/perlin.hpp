
#ifndef perlin_hpp
#define perlin_hpp


double fade(double t);
double lerp(double a, double b, double t);
double noise(double x, double y, double z);
double noise(double x, double y, double z, double w);
double grad(int hash, double x, double y, double z);
double grad(int hash, double x, double y, double z, double w);
extern const int p[512];


#endif