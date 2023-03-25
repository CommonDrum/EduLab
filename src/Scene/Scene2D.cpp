//
// Created by Karol Hetman on 15/03/2023.
//

#include "Scene2D.h"

Scene2D::Scene2D(std::string name) {
    world_ = new b2World(b2Vec2(0, 9.81f));
    name_ = std::move(name);

}


