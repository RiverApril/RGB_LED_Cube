#pragma once

#include "../../core/cubeCore.hpp"
#include "World.hpp"
#include "Geometry.hpp"

class World;

class Entity{
public:
    Pos3 pos;
    int height;
    color_t color;
    bool hasGravity;

    Entity() : pos(0, 0, 0), height(0), color(0xFFFFFF), hasGravity(true) {}

    virtual void update(World* world, double delta);
};

class Player : public Entity{
public:

    Player() : Entity(){}

    void update(World* world, double delta);
};