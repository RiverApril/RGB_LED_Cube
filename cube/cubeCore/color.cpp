

#include "color.hpp"

color_t hsv2rgb(double hue, double sat, double val) { // degrees, 0-1, 0-1
    double hh, p, q, t, ff;
    long i;

    double outR, outG, outB;

    if(sat <= 0.0) {
        outR = val;
        outG = val;
        outB = val;
        return RGB((color_t)(outR*0xFF), (color_t)(outG*0xFF), (color_t)(outB*0xFF));
    }
    hh = hue;
    if(hh >= 360.0) hh -= 360.0;
    if(hh < 0.0) hh += 360.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = val * (1.0 - sat);
    q = val * (1.0 - (sat * ff));
    t = val * (1.0 - (sat * (1.0 - ff)));

    switch(i) {
    case 0:
        outR = val;
        outG = t;
        outB = p;
        break;
    case 1:
        outR = q;
        outG = val;
        outB = p;
        break;
    case 2:
        outR = p;
        outG = val;
        outB = t;
        break;
    case 3:
        outR = p;
        outG = q;
        outB = val;
        break;
    case 4:
        outR = t;
        outG = p;
        outB = val;
        break;
    case 5:
    default:
        outR = val;
        outG = p;
        outB = q;
        break;
    }
    return RGB((color_t)(outR*0xFF), (color_t)(outG*0xFF), (color_t)(outB*0xFF));;     
}