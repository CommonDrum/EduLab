//
// Created by Karol Hetman on 25/02/2023.
//

#ifndef EDULAB_ENTITY_H
#define EDULAB_ENTITY_H

#include "core.h"

class Entity {

public:
    explicit Entity(b2Vec2 position ={0,0}, b2Vec2 size ={1,1});
    void SetPosition(b2Vec2 position);
    void SetSize(b2Vec2 size);
    b2Vec2 GetPosition();
    b2Vec2 GetSize();
    std::vector<SDL_Vertex> GetVertices();
private:
    //Pointers to the position and size in the box2d body
    b2Vec2 * m_position{};
    b2Vec2 * m_size{};
    //Box2d body
    b2Body * m_body{};
    //Box2d fixture
    b2Fixture * m_fixture{};



};


#endif //EDULAB_ENTITY_H
