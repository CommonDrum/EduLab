//
// Created by Karol Hetman on 15/03/2023.
//

#include "Scene2D.h"

Scene2D::Scene2D(std::string name) {
    world_ = new b2World(b2Vec2(0, 9.81f));
    name_ = std::move(name);

}

void Scene2D::add_object(b2Body *body, ImVec4 color) {
    auto* object = new Object2D(body, color);
    objects_.push_back(object);
}

void Scene2D::delete_object(b2Body *body) {
    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
        if ((*it)->get_body() == body) {
            world_->DestroyBody(body);
            objects_.erase(it);
            break;
        }
    }

}


Object2D::Object2D(b2Body *body, ImColor color) { // because i will probably need body to store more inforamtion  like forces etc
    this->body = body;
    this->color = color;

}
