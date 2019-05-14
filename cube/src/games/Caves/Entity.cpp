#include "Entity.hpp"
#include "World.hpp"

void Entity::update(World* world, double delta){
    if(world->getBlock(pos + Pos3(0, 0, -1)) == air){
        pos.z--;
    }
}

void Player::update(World* world, double delta){
    Entity::update(world, delta);
}