#pragma once

struct Pos3{
    int x, y, z;

    Pos3() : x(0), y(0), z(0){}

    Pos3(int x, int y, int z) : x(x), y(y), z(z){}

    Pos3 operator+(Pos3 other) {
        return Pos3(this->x + other.x, this->y + other.y, this->z + other.z);
    }
};