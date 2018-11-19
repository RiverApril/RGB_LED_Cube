

#include "perlin.hpp"

#include <math.h>
#include <stdio.h>

//  https://mrl.nyu.edu/~perlin/noise/

double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
double lerp(double a, double b, double t) { return a + t * (b - a); }

const int p[512] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180, 
   151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

double noise(double x, double y, double z) {
    int X = (int)floor(x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)floor(y) & 255,                  // CONTAINS POINT.
        Z = (int)floor(z) & 255;
    x -= floor(x);                                // FIND RELATIVE X,Y,Z
    y -= floor(y);                                // OF POINT IN CUBE.
    z -= floor(z);
    double u = fade(x),                                // COMPUTE FADE CURVES
           v = fade(y),                                // FOR EACH OF X,Y,Z.
           w = fade(z);

    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = p[p[p[ X ]+ Y ]+ Z ];
    aba = p[p[p[ X ]+Y+1]+ Z ];
    aab = p[p[p[ X ]+ Y ]+Z+1];
    abb = p[p[p[ X ]+Y+1]+Z+1];
    baa = p[p[p[X+1]+ Y ]+ Z ];
    bba = p[p[p[X+1]+Y+1]+ Z ];
    bab = p[p[p[X+1]+ Y ]+Z+1];
    bbb = p[p[p[X+1]+Y+1]+Z+1];

    double x1, x2, y1, y2, z1;
    x1 = lerp(grad(aaa, x  , y  , z), grad(baa, x-1, y  , z), u);
    x2 = lerp(grad(aba, x  , y-1, z), grad(bba, x-1, y-1, z), u);
    y1 = lerp(x1, x2, v);

    x1 = lerp(grad(aab, x  , y  , z-1), grad(bab, x-1, y  , z-1), u);
    x2 = lerp(grad(abb, x  , y-1, z-1), grad(bbb, x-1, y-1, z-1), u);
    y2 = lerp (x1, x2, v);

    z1 = lerp (y1, y2, w);
    
    return (z1+1)/2;
}
double grad(int hash, double x, double y, double z){
    switch(hash & 0xF) {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;

        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0;
    }
}

double noise(double x, double y, double z, double w) {
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255,
        W = (int)floor(w) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    w -= floor(w);
    double u = fade(x),
           v = fade(y),
           l = fade(z),
           m = fade(w);

    int aaaa, aaba, aaab, aabb, abaa, abba, abab, abbb,
        baaa, baba, baab, babb, bbaa, bbba, bbab, bbbb;

    aaaa = p[p[p[p[ X ]+ Y ]+ Z ]+ W ];
    baaa = p[p[p[p[X+1]+ Y ]+ Z ]+ W ];
    abaa = p[p[p[p[ X ]+Y+1]+ Z ]+ W ];
    bbaa = p[p[p[p[X+1]+Y+1]+ Z ]+ W ];
    aaba = p[p[p[p[ X ]+ Y ]+Z+1]+ W ];
    baba = p[p[p[p[X+1]+ Y ]+Z+1]+ W ];
    abba = p[p[p[p[ X ]+Y+1]+Z+1]+ W ];
    bbba = p[p[p[p[X+1]+Y+1]+Z+1]+ W ];

    aaab = p[p[p[p[ X ]+ Y ]+ Z ]+W+1];
    baab = p[p[p[p[X+1]+ Y ]+ Z ]+W+1];
    abab = p[p[p[p[ X ]+Y+1]+ Z ]+W+1];
    bbab = p[p[p[p[X+1]+Y+1]+ Z ]+W+1];
    aabb = p[p[p[p[ X ]+ Y ]+Z+1]+W+1];
    babb = p[p[p[p[X+1]+ Y ]+Z+1]+W+1];
    abbb = p[p[p[p[ X ]+Y+1]+Z+1]+W+1];
    bbbb = p[p[p[p[X+1]+Y+1]+Z+1]+W+1];

    double x1, x2, y1, y2, z1, z2, w1;
    x1 = lerp(grad(aaaa, x  , y  , z, w), grad(baaa, x-1, y  , z, w), u);
    x2 = lerp(grad(abaa, x  , y-1, z, w), grad(bbaa, x-1, y-1, z, w), u);
    y1 = lerp(x1, x2, v);

    x1 = lerp(grad(aaba, x  , y  , z-1, w), grad(baba, x-1, y  , z-1, w), u);
    x2 = lerp(grad(abba, x  , y-1, z-1, w), grad(bbba, x-1, y-1, z-1, w), u);
    y2 = lerp (x1, x2, v);

    z1 = lerp (y1, y2, l);


    x1 = lerp(grad(aaab, x  , y  , z, w-1), grad(baab, x-1, y  , z, w-1), u);
    x2 = lerp(grad(abab, x  , y-1, z, w-1), grad(bbab, x-1, y-1, z, w-1), u);
    y1 = lerp(x1, x2, v);

    x1 = lerp(grad(aabb, x  , y  , z-1, w-1), grad(babb, x-1, y  , z-1, w-1), u);
    x2 = lerp(grad(abbb, x  , y-1, z-1, w-1), grad(bbbb, x-1, y-1, z-1, w-1), u);
    y2 = lerp (x1, x2, v);

    z2 = lerp (y1, y2, l);


    w1 = lerp (z1, z2, m);
    
    return (w1+1)/2;

}
double grad(int hash, double x, double y, double z, double w){
    switch(hash & 0x1F) {
        case 0x00: return   x + y + z;
        case 0x01: return   x + y - z;
        case 0x02: return   x - y + z;
        case 0x03: return   x - y - z;
        case 0x04: return - x + y + z;
        case 0x05: return - x + y - z;
        case 0x06: return - x - y + z;
        case 0x07: return - x - y - z;

        case 0x08: return   x + y + w;
        case 0x09: return   x + y - w;
        case 0x0A: return   x - y + w;
        case 0x0B: return   x - y - w;
        case 0x0C: return - x + y + w;
        case 0x0D: return - x + y - w;
        case 0x0E: return - x - y + w;
        case 0x0F: return - x - y - w;

        case 0x10: return   x + z + w;
        case 0x11: return   x + z - w;
        case 0x12: return   x - z + w;
        case 0x13: return   x - z - w;
        case 0x14: return - x + z + w;
        case 0x15: return - x + z - w;
        case 0x16: return - x - z + w;
        case 0x17: return - x - z - w;

        case 0x18: return   y + z + w;
        case 0x19: return   y + z - w;
        case 0x1A: return   y - z + w;
        case 0x1B: return   y - z - w;
        case 0x1C: return - y + z + w;
        case 0x1D: return - y + z - w;
        case 0x1E: return - y - z + w;
        case 0x1F: return - y - z - w;
        default: return 0;
    }
}