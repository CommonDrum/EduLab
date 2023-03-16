//
// Created by Karol Hetman on 15/03/2023.
//
#include "Scene2DManager.h"

void Scene2DManager::create_scene(std::string name) {
    auto *new_scene = new Scene2D(std::move(name));
    if (scenes_.empty()) {
        current_scene_ = new_scene;
    }
}

std::vector<std::string> Scene2DManager::get_scene_names() const {
    std::vector<std::string> names;
    names.reserve(scenes_.size());
for (auto &scene : scenes_) {
        names.push_back(scene->get_name());
    }
    return names;
}

b2Body *Scene2DManager::create_body(ImVec4 color, int shape, b2BodyType type) {
    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set(0, 0);
    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);

    if (shape == 0) {
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1, 1);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        body->CreateFixture(&fixtureDef);
    } else if (shape == 1) {
        b2CircleShape circle;
        circle.m_radius = 1;
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        body->CreateFixture(&fixtureDef);
    }
    return body;
}

b2Body *Scene2DManager::delete_body(b2Body *body) {
//          remove body from vector

    for (auto it = current_scene_->get_bodies().begin(); it != current_scene_->get_bodies().end(); ++it) {
        if (it->first == body) {
            current_scene_->get_bodies().erase(it);
            break;
        }
    }
    current_scene_->get_world()->DestroyBody(body);
    return nullptr;
}

void Scene2DManager::load_drawlist(ImDrawList *draw_list) {
    for (auto &item : current_scene_->get_bodies()) {
        ImVec4 color = item.second;
        b2Body* body = item.first;
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

    }

}

b2Body *Scene2DManager::CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    return body;
}

b2Body * Scene2DManager::CreateCircle(float x, float y, float radius, b2BodyType bodyType) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    return body;
}

