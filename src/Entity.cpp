//
// Created by Karol Hetman on 25/02/2023.
//

#include "Entity.h"

b2Vec2 Entity::GetSize() {
    return *m_size;
}

b2Vec2 Entity::GetPosition() {
    return *m_position;
}

Entity::Entity(b2Vec2 position , b2Vec2 size) {

    m_position = new b2Vec2( position.x,position.y);


}

std::vector<SDL_Vertex> Entity::GetVertices() {
    return std::vector<SDL_Vertex>() = {
        { SDL_FPoint{ 400 + m_position->x, 150 + m_position->y }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
        { SDL_FPoint{ 200 + m_position->x, 450 + m_position->y }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
        { SDL_FPoint{ 600 + m_position->x, 450 + m_position->y }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
    };;
}
